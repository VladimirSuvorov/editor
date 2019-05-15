#ifndef GRAPHICS_EDITOR_PROJECT_IMPLEMENTATION_DETAILS_INL
#include <iostream>
#define GRAPHICS_EDITOR_PROJECT_IMPLEMENTATION_DETAILS_INL
//Tiny implementation.
//Beware, you can get some very curious knowledges about the typization. ;)
//
//
//Brief explaination
//All the following code is a tricky implementation, provided TO DEMONSTRATE THE GRAPHICS EDITOR WORKS ONLY.
//So, you have no need to understand exactly what is the hell is that coming on here, in that *.inl file.
//
//If you want to understand it anyway I've put these notes in hope it would help to you:
//
//Some assumptions that are used in the implementation:
//	0)There a lot of 'stuct *##Traits' are used as template parameters of classes, 
//	and all these traits are implemented in this *.inl.
//		NOTE: All these traits are stubs in fact. They are used to provide type consistency and minimal implementation.
//		Most of them have a static member functions - this is for simplification purposes only and is not mandatory by design at all,
//		but this is a little bit of implicit limitations of the current design that make sence to any implementation. 
//		This is about a functionality, that must be implemented someway in somewhere (therefore it is implemented in traits),
//		but to avoid of little bit strange data-members like 'SomeTraits someTrait_' and issues with traits constructors,
//		I've used static methods in these traits and invoke these methods where they are required as stubs.
//
//	1)There is no difference between an interface and exact type T, they are both the same type.  
//		NOTE: This is for simplicity only. The difference between T and I, 
//		where T implements interface I is not implemented by the following implementation, 
//		but such difference is assumed by design, 
//		therefore you have to imagine that T and I are not the same type T.
//		Usages of 'UnwrappedInterface<T> instance' are based on the such simplification and wont work without it.
//	
//	2)The most useful and least selfexpressive is DemoEditorTraits struct. 
//		First of all, it is required to instantiate the editor template class.
//		Secondary, you can (and should, to simplify your and others life) use the following code,
//		because the following classes are required to create the editor:
//
//	using demo_editor_traits_t = DemoEditorTraits<int, std::vector<int>>; //<--1
//
//  using DocumentFactory_t = UnwrappedInterface<demo_editor_traits_t::document_factory_interface_t>; //<--2
//  using FileFactory_t = UnwrappedInterface<DocumentFactory_t::file_factory_interface_t>;
//  using GraphicsFactory_t = UnwrappedInterface<demo_editor_traits_t::graphics_factory_interface_t>;
//
//  FileFactory_t fileFactory; //<--3
//  DocumentFactory_t documentFactory{ fileFactory };
//  GraphicsFactory_t graphicsFactory;
//
//	Editor<demo_editor_traits_t> deditor(&documentFactory, &graphicsFactory); //<--4
//
//		Let me explain how does it workes.
//		At point 1 you instantiate DemoEditorTraits with int as a graphics primitive and std::vector<int> as a container for the primitive.
//		Despite that int is not a graphics primitive, we will assume that it is and you're not going to be mad ;).
//		At the paragraph 2, you extract the required types from DemoEditorTraits you've instantiated before to use these types at paragraph 3.
//		(You should do so, to simplify your life, cos it is guaranteed,
//		that types extracted from demo_editor_traits_t are compatible to each other and to the editor.)
//		At the paragraph 3, you just create the required types in a belief that documentFactory really requires fileFactory to work with
//		and I'am talking truth.
//		An then, you just create the editor at the point 4.
//		Also, if InterfaceWrapper = ptr_view, 
//		you can use '(documentFactory, graphicsFactory)' instead of '(&documentFactory, &graphicsFactory)' in the Editor constructor,
//		but this don't work with CStyle pointers.
//
//		What if you pass nullptr into the Editor constructor?
//		- This depends on what are the types of 
//			Editor<demo_editor_traits_t>::document_factory_interface_t and
//			Editor<demo_editor_traits_t>::graphics_factory_interface_t exactly?
//			Therefore there different cases, short answer is that you should assume, that they are a pointer-like types and,
//			as far as this is required by the editor, there should never be a nullptr.
//		On the other hand, these types could be references, therefore, your code with nullptr,
//		passed into the Editor's constructor even won't compile at all (and protect you from UB).
namespace editor_project {
	namespace stub_implementation {
		using utilities::filepath_t;
		using utilities::UnwrappedInterface;
		using utilities::WrappedInterface;
		template<typename DocumentInterface>
		struct DemoFileTraits {
			using document_interface_t = DocumentInterface;
			static document_interface_t from(filepath_t filepath, document_interface_t document) {
				using namespace std;
				static UnwrappedInterface<document_interface_t> documentInstance;
				cout
					<< "saving file from the document" << endl
					<< "at: " << filepath << endl;
				return documentInstance;
			}
			static document_interface_t to(filepath_t filepath, document_interface_t document) {
				using namespace std;
				static UnwrappedInterface<document_interface_t> documentInstance;
				cout
					<< "loading file to the document" << endl
					<< "at: " << filepath << endl;
				return documentInstance;
			}
		};


		template<typename GraphicsInterface, typename GraphicsContainer>
		struct DemoGraphicsTraits {
			using graphics_interface_t = GraphicsInterface;
			using graphics_container_t = GraphicsContainer;
			static graphics_interface_t insert(graphics_container_t container, graphics_interface_t primitive) {
				using namespace std;
				cout << "inserting a primitive" << endl;
				return primitive;
			}
			static void remove(graphics_container_t& graphicsContainer, graphics_interface_t primitive) {
				using namespace std;
				cout << "removing a primitive" << endl;
			}
		};

		template<typename File>
		struct DemoFileFactoryTraits {
			using file_t = File;
			using file_interface_t =  utilities::WrappedInterface<file_t>;

			static file_interface_t load(filepath_t filepath) {
				using namespace std;
				cout << "preparing file to read data from " << filepath << endl;
				static file_t file_{ filepath };
				return file_;
			}
			static file_interface_t save(filepath_t filepath) {
				using namespace std;
				cout << "preparing file to write data to " << filepath << endl;
				static file_t file_{ filepath };
				return file_;
			}
		};
		template<typename GraphicsInterface>
		struct DemoGraphicsFactoryTraits {
			using graphics_interface_t = GraphicsInterface;
			static graphics_interface_t make() {
				using namespace std;
				cout << "making a graphics primitive" << endl;
				return graphics_interface_t();
			}
		};


		template<typename FileFactoryInterface, typename DocumentInterface>
		struct DemoDocumentFactoryTraits {
			using file_factory_interface_t = FileFactoryInterface;
			using document_interface_t = DocumentInterface;
			static document_interface_t make(document_interface_t current_document) {
				using namespace std;
				cout << "making a document" << endl;
				static UnwrappedInterface<DocumentInterface> document;
				return document;
			}
		};

		using editor_project::document::Document;
		using editor_project::document::DocumentFactory;
		using editor_project::file::File;
		using editor_project::file::FileFactory;
		using editor_project::GraphicsFactory;

		template<typename GraphicsInterface, typename GraphicsContainer>
		struct DemoEditorTraits {
			//This is not a bullshit, but a programming pattern, well known as "Crazzy Russian Matryoshka"
			//
			using document_interface_t = WrappedInterface<Document<DemoGraphicsTraits<GraphicsInterface, GraphicsContainer>>>;
			using document_factory_interface_t = WrappedInterface<DocumentFactory<DemoDocumentFactoryTraits<WrappedInterface<FileFactory<
				DemoFileFactoryTraits<File<DemoFileTraits<document_interface_t>>>>>, document_interface_t>>>;
			using graphics_factory_interface_t = WrappedInterface<GraphicsFactory<DemoGraphicsFactoryTraits<GraphicsInterface>>>;
			using graphics_interface_t = GraphicsInterface;
		};
	}
}
#endif // !GRAPHICS_EDITOR_PROJECT_IMPLEMENTATION_DETAILS_INL
