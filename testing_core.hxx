#ifndef GRAPHICS_EDITOR_PROJECT_TESTING_CORE_HXX
#include <vector>
#include <algorithm>
#include <gtest/gtest.h>
#include "Editor.hpp"
#define GRAPHICS_EDITOR_PROJECT_TESTING_CORE_HXX


using editor_project::document::Document;	//M - Model
using editor_project::file::File;			//V - View
using editor_project::editor::Editor;		//C - Controller

namespace {//all the anonimous namespace is for testing implementation stuff
	//NOTE: All the implementation bullshit you can see below is very stupid but existing for testing purposes only. 
	using editor_project::document::DocumentFactory;
	using editor_project::file::FileFactory;

	using editor_project::utilities::filepath_t;
	using editor_project::utilities::UnwrappedInterface;
	using editor_project::utilities::WrappedInterface;
	
	using testing_graphics_interface_t = int;//int is a graphics type. Just believe me!)
	template<typename GraphicsInterface>
	struct DemoGraphicsFactoryTraits {
		using graphics_interface_t = GraphicsInterface;
		static graphics_interface_t make() {
			return graphics_interface_t();//semantically incorrect, but correct in fact: as far as graphics_interface_t=int, this is OK.
		}
	};
	using testing_graphics_factory_t = DemoGraphicsFactoryTraits<testing_graphics_interface_t>;

	struct TestDocumentTraits {
		using graphics_interface_t = testing_graphics_interface_t;
		using graphics_container_t = std::vector<testing_graphics_interface_t>;

		static graphics_interface_t insert(graphics_container_t& container, graphics_interface_t primitive) {
			container.push_back(primitive);
			last_inserted = primitive;
			return container.back();
		}
		static void remove(graphics_container_t& container, graphics_interface_t primitive) {
			using namespace std;
			last_removed = primitive;
			container.erase(find(begin(container), end(container), primitive));
		}
		inline static graphics_interface_t last_inserted;
		inline static graphics_interface_t last_removed;

	};
	struct TestFileTraits {
		using document_interface_t = WrappedInterface<Document<TestDocumentTraits>>;
		//from the given document to the given file
		static document_interface_t from(filepath_t filepath, document_interface_t document) {
			container.clear();
			for (auto& graphics : *document)
				container.push_back(graphics);
			return document;
		}
		//from the given file to the given document
		static document_interface_t to(filepath_t filepath, document_interface_t document) {
			container.clear();
			for (size_t index(0); index < 1000; ++index)
				document->insert(index);
			return document;
		}
		inline static typename TestDocumentTraits::graphics_container_t container;
	};

	struct TestDocumentFactory {
		using document_interface_t = WrappedInterface<Document<TestDocumentTraits>>;
		TestDocumentFactory(filepath_t filepath) :
			file_(std::move(filepath))
		{}

		document_interface_t make(document_interface_t current_document) {
			return document_;
		}
		document_interface_t load(document_interface_t current_document, filepath_t filepath) {
			return file_.to(document_);
		}
		document_interface_t save(document_interface_t current_document, filepath_t filepath) {
			return file_.from(document_);
		}
	private:
		Document<TestDocumentTraits> document_;
		File<TestFileTraits> file_;
	};
}
struct TestEditorTraits {
	using document_interface_t = WrappedInterface<Document<TestDocumentTraits>>;
	using document_factory_interface_t = WrappedInterface<TestDocumentFactory>;
	using graphics_factory_interface_t = WrappedInterface<testing_graphics_factory_t>;
	using graphics_interface_t = testing_graphics_interface_t;
};

bool is_model_correct() {
	//initialization
	TestDocumentTraits::last_inserted = TestDocumentTraits::last_removed = 0;//Make neither inserted nor removed elements in the model
	
	//actual test
	Document<TestDocumentTraits> model;
	for (size_t index(0); index < 1000; ++index) {
		if (model.insert(index) != index)//model.insert must return the inserted data
			return false;
		if (TestDocumentTraits::last_inserted != index)//last_inserted element must be equal to the element we've inserted before...
			return false;
	}

	for (size_t index(0); index < 1000; ++index) {
		model.remove(index);
		if (TestDocumentTraits::last_removed != index)//lase_removed element must be equal to the el. we've removed before.
			return false;
	}

	return true;
}

bool is_view_correct() {
	using namespace std;
	Document<TestDocumentTraits> model;

	File<TestFileTraits> view{ "file" };
	view.to(model);//fill the model with ints (i.e. graphics) with values [0; 999] according to the mock implementation

	auto from = begin(model);
	auto to = end(model);
	for (auto it(from); it != to; ++it)//each graphics element must be in [0; 999], cos it has been set to at line "view.to(model)"
		if (*it != (it - from))
			return false;
	model.insert(1000);//insert 1001-th element with value 1000...
	view.from(model);//...and write the model to the file
	//therefore, to the moment, in the file (view) we must have 1001 elements of range [0;1000]
	for (auto it(cbegin(TestFileTraits::container)); it != cend(TestFileTraits::container); ++it)
		if (*it != (it - cbegin(TestFileTraits::container)))
			return false;

	return true;
}

bool is_controller_correct() {
	//initialization
	using DocumentFactory_t = UnwrappedInterface<TestEditorTraits::document_factory_interface_t>;
	using GraphicsFactory_t = UnwrappedInterface<TestEditorTraits::graphics_factory_interface_t>;
	
	TestDocumentTraits::last_inserted = TestDocumentTraits::last_removed = 0;//Make neither inserted nor removed elements in the model
	TestFileTraits::container.clear();//and clear "file"

	//actual test
	DocumentFactory_t document_factory{ "file" };
	GraphicsFactory_t graphics_factory;

	Editor<TestEditorTraits> controller{ document_factory, graphics_factory };
	
	controller.loadDocument("file.in");
	if (TestDocumentTraits::last_inserted != 999)//the test implementation loads 1000 elements with [0; 999] values, via document.insert
		return false;

	auto primitive = controller.makePrimitive();
	if (primitive)//the test implementation returns primitive constructed via default constructor,
		return false;// therefore with int as graphics type it constucts int() == 0.
	
	controller.saveDocument("file.out");
	if (TestFileTraits::container.back() != 0)//as far as 0 is a value of the last element of the model, 
		return false;//it must be the last element of the file implementation's internal vector, after the document has been saved.
		   
	controller.removePrimitive(primitive);
	if (TestDocumentTraits::last_removed != 0)//as far as primitive == 0, when we remove it, must be last_removed == primitive == 0
		return false;
	   
	controller.makeDocument();
	if (TestDocumentTraits::last_removed != 0)//as far as current (test) factory returns the same document always,
		return false;//and last removed was == 0, we can reuse the condition for the current, (maked with makeDocument) document.
	
	return true;
}




int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
#endif // !GRAPHICS_EDITOR_PROJECT_TESTING_CORE_HXX
