#ifndef GRAPHICS_EDITOR_PROJECT_EDITOR_HPP
#include "utilities.hpp"
#define GRAPHICS_EDITOR_PROJECT_EDITOR_HPP
namespace editor_project {
	namespace editor {
		template<typename EditorTraits>
		struct Editor;//Controller
	}
	namespace document {
		template<typename GraphicsTraits>
		struct Document;//Model
	}
	namespace file {
		template<typename FileTraits>
		struct File;//View
	}

	namespace editor {
		using utilities::filepath_t;
		/// \class Editor
		/// \brief Editor aka Controller
		template<typename EditorTraits>
		struct Editor {
			using document_factory_interface_t = typename EditorTraits::document_factory_interface_t;
			using document_interface_t = typename EditorTraits::document_interface_t;
			using graphics_factory_interface_t = typename EditorTraits::graphics_factory_interface_t;
			using graphics_interface_t = typename EditorTraits::graphics_interface_t;

			Editor(document_factory_interface_t documentFactory, graphics_factory_interface_t graphicsFactory) :
				current_document_factory_(std::move(documentFactory)), current_graphics_factory_(std::move(graphicsFactory))
			{}
			///\brief UI event handler for makeDocument
			void makeDocument() {
				current_document_ = current_document_factory_->make(current_document_);
			}
			///\brief UI event handler for loadDocument
			///\param filepath - where from load a document?
			void loadDocument(filepath_t filepath) {
				current_document_ = current_document_factory_->load(current_document_, filepath);
			}
			///\brief UI event handler for saveDocument
			///\param filepath - where to store a document?
			void saveDocument(filepath_t filepath) {
				current_document_ = current_document_factory_->save(current_document_, filepath);
			}
			///\brief UI event handler for makePrimitive
			///\return made primitive
			graphics_interface_t makePrimitive() {
				return current_document_->insert(current_graphics_factory_->make());
			}			
			///\brief UI event handler for removePrimitive
			///\param primitive to remove
			void removePrimitive(graphics_interface_t primitiveToRemove) {
				current_document_->remove(primitiveToRemove);
			}
		private:
			document_factory_interface_t current_document_factory_;
			document_interface_t current_document_;
			graphics_factory_interface_t current_graphics_factory_;
		};
	}
	namespace document {
		using utilities::filepath_t;		
		/// \class DocumentFactory
		/// \brief DocumentFactory that creates Documents
		/// \see Document
		template<typename DocumentFactoryTraits>
		struct DocumentFactory {
			using file_factory_interface_t = typename DocumentFactoryTraits::file_factory_interface_t;
			using document_interface_t = typename DocumentFactoryTraits::document_interface_t;
			DocumentFactory(file_factory_interface_t fileFactory) :
				current_file_factory_(std::move(fileFactory))
			{}
			
			///\brief  makes a document. Performs remove|clear on a given one, if given one!=nullptr.
			///\param  current_document to remake|nullptr
			///\return ready to use document of document_interface_t type.
			document_interface_t make(document_interface_t current_document) {
				return DocumentFactoryTraits::make(current_document);//may be a stub
			}
			///\brief  loads a document from a given filepath
			///\param current_document - document to reload|nullptr
			///\param filepath - where to load from?
			///\return loaded document
			document_interface_t load(document_interface_t current_document, filepath_t filepath) {
				auto file = current_file_factory_->load(filepath);
				return (file) ? file->to(current_document) : nullptr;
			}
			///\brief  saves a document to a given filepath
			///\param current_document - document to save|nullptr (if you would save an empty doc) 
			///\param filepath - where to save?
			///\return saved document|nullptr (if current_document==nullptr | on error)
			document_interface_t save(document_interface_t current_document, filepath_t filepath) {
				auto file = current_file_factory_->save(filepath);
				return (file) ? file->from(current_document) : nullptr;
			}
		private:
			file_factory_interface_t current_file_factory_;
		};
		
		
		/// \class Document
		/// \brief Document aka Model
		template<typename GraphicsTraits>
		struct Document {
			using graphics_interface_t = typename GraphicsTraits::graphics_interface_t;
			using graphics_container_t = typename GraphicsTraits::graphics_container_t;
			using size_type = typename graphics_container_t::size_type;
		private:
			graphics_container_t graphics_container_;
		public:
			///\brief inserts a given graphics primitive
			///\param primitive to insert
			///\return inserted graphics primitive
			graphics_interface_t insert(graphics_interface_t primitive) {
				return GraphicsTraits::insert(graphics_container_, primitive);//may be a stub
			}
			///\brief removes a given graphics primitive
			///\param primitive to remove
			void remove(graphics_interface_t primitive) {
				GraphicsTraits::remove(graphics_container_, primitive);//may be a stub
			}
			///\brief STL-like methods
			///\return correspondent iterator|size dependent on what do u invoke 
			constexpr auto begin()GRAPHICS_EDITOR_PROJECT_DELEGATE_INVOKE(std::begin(graphics_container_));
			constexpr auto end()GRAPHICS_EDITOR_PROJECT_DELEGATE_INVOKE(std::end(graphics_container_));
			constexpr auto cbegin()const noexcept GRAPHICS_EDITOR_PROJECT_DELEGATE_INVOKE(std::cbegin(graphics_container_));
			constexpr auto cend()const noexcept GRAPHICS_EDITOR_PROJECT_DELEGATE_INVOKE(std::cend(graphics_container_));
			constexpr auto size()GRAPHICS_EDITOR_PROJECT_DELEGATE_INVOKE(std::size(graphics_container_));			
		};
	}
	
	/// \class GraphicsFactory
	/// \brief GraphicsFactory creates graphic elements of graphics_interface_t type
	template<typename GraphicsFactoryTraits>
	struct GraphicsFactory {
		using graphics_interface_t = typename GraphicsFactoryTraits::graphics_interface_t;
		///\return some graphics
		graphics_interface_t make() {
			return GraphicsFactoryTraits::make();//may be a stub
		}
	};

	namespace file {
		using utilities::filepath_t;
		/// \class FileFactory
		/// \brief FileFactory creates Files of file_interface_t type 
		/// \see File
		template<typename FileFactoryTraits>
		struct FileFactory {
			using file_interface_t = typename FileFactoryTraits::file_interface_t;
			///\return a file able to load data from a given filepath (as instance of file_interface_t)
			file_interface_t load(filepath_t filepath) {
				return FileFactoryTraits::load(filepath);//may be a stub
			}
			///\return a file able to save data to a given filepath (as instance of file_interface_t)
			file_interface_t save(filepath_t filepath) {
				return FileFactoryTraits::save(filepath);//may be a stub
			}
		};

		/// \class File
		/// \brief File aka View, can fill the model (Document) with data from file as well as can be filled from the model (
		template<typename FileTraits>
		struct File {
			using file_interface_t = utilities::WrappedInterface<File>;
			using document_interface_t = typename FileTraits::document_interface_t;

			File(filepath_t filepath) : filepath_(filepath) {}

			///\brief read data FROM file in the filesystem to the instance of the class
			///\return instance of loaded document of document_interface_t type 
			document_interface_t from(document_interface_t document) {
				return FileTraits::from(filepath_, document);//may be a stub
			}
			///\brief save data TO file in the filesystem from the constant instance of the class
			///\return instance of saveed document of document_interface_t type 
			document_interface_t to(document_interface_t document)const {
				return FileTraits::to(filepath_, document);//may be a stub
			}
		private:
			filepath_t filepath_;
		};
	}
}
#endif // !GRAPHICS_EDITOR_PROJECT_EDITOR_HPP
