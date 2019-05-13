
#include <vector>
#include "Editor.hpp"
#include "testing_core.hxx"
#include "ImplementationDetails.inl"

using editor_project::utilities::filepath_t;
template<typename Editor>
void makeDocument(Editor& editor) {
	editor.makeDocument();
}
template<typename Editor>
void loadDocument(Editor& editor, filepath_t filepath) {
	editor.loadDocument(filepath);
}
template<typename Editor>
void saveDocument(Editor& editor, filepath_t filepath) {
	editor.saveDocument(filepath);
}
template<typename Editor>
decltype(auto) makePrimitive(Editor& editor) {
	return editor.makePrimitive();
}
template<typename Editor, typename Primitive>
void removePrimitive(Editor& editor, Primitive&& primitiveToRemove) {
	editor.removePrimitive(std::forward<Primitive>(primitiveToRemove));
}


int main(int argc, char *argv[]) {
	using namespace std;
	using namespace editor_project;
	using utilities::UnwrappedInterface;
	using editor::Editor;
	
	using demo_editor_traits_t = stub_implementation::DemoEditorTraits<int, vector<int>>;
	//where GraphicsInterface=int, cos who cares if it is wrong?
	
	using DocumentFactory_t = UnwrappedInterface<demo_editor_traits_t::document_factory_interface_t>;
	using FileFactory_t = UnwrappedInterface<DocumentFactory_t::file_factory_interface_t>;
	using GraphicsFactory_t = UnwrappedInterface<demo_editor_traits_t::graphics_factory_interface_t>;


	FileFactory_t fileFactory;
	DocumentFactory_t documentFactory{ fileFactory };
	GraphicsFactory_t graphicsFactory;
	Editor<demo_editor_traits_t> deditor(documentFactory, graphicsFactory);
	//deditor = demo editor, i.e. Editor istantiated with demo_editor_traits_t
	
	makeDocument(deditor);
	loadDocument(deditor, "file.in");
	auto primitive = makePrimitive(deditor);
	removePrimitive(deditor, primitive);
	saveDocument(deditor, "file.out");

	system("PAUSE");
}