#ifndef GRAPHICS_EDITOR_PROJECT_EDITOR_HPP
#include "utilities.hpp"
#define GRAPHICS_EDITOR_PROJECT_EDITOR_HPP
namespace editor_project {
	namespace editor {
		template<typename EditorTraits>// Controller tests (mocks)
		struct Editor;//Controller
	}
	namespace document {
		template<typename GraphicsTraits>// Model tests (mocks)
		struct Document;//Model
	}
	namespace file {
		template<typename FileTraits>// View tests (mocks)
		struct File;//View
	}

	namespace editor {
		using utilities::filepath_t;
		template<typename EditorTraits>
		struct Editor {
			using document_factory_interface_t = typename EditorTraits::document_factory_interface_t;
			using document_interface_t = typename EditorTraits::document_interface_t;
			using graphics_factory_interface_t = typename EditorTraits::graphics_factory_interface_t;
			using graphics_interface_t = typename EditorTraits::graphics_interface_t;

			Editor(document_factory_interface_t documentFactory, graphics_factory_interface_t graphicsFactory) :
				current_document_factory_(std::move(documentFactory)), current_graphics_factory_(std::move(graphicsFactory))
			{}

			void makeDocument() {
				current_document_ = current_document_factory_->make(current_document_);
			}
			void loadDocument(filepath_t filepath) {
				current_document_ = current_document_factory_->load(current_document_, filepath);
			}
			void saveDocument(filepath_t filepath) {
				current_document_ = current_document_factory_->save(current_document_, filepath);
			}
			graphics_interface_t makePrimitive() {
				return current_document_->insert(current_graphics_factory_->make());
			}
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
		template<typename DocumentFactoryTraits>
		struct DocumentFactory {
			using file_factory_interface_t = typename DocumentFactoryTraits::file_factory_interface_t;
			using document_interface_t = typename DocumentFactoryTraits::document_interface_t;
			DocumentFactory(file_factory_interface_t fileFactory) :
				current_file_factory_(std::move(fileFactory))
			{}

			document_interface_t make(document_interface_t current_document) {
				return DocumentFactoryTraits::make(current_document);//may be a stub
			}
			document_interface_t load(document_interface_t current_document, filepath_t filepath) {
				auto file = current_file_factory_->load(filepath);
				return (file) ? file->to(current_document) : nullptr;
			}
			document_interface_t save(document_interface_t current_document, filepath_t filepath) {
				auto file = current_file_factory_->save(filepath);
				return (file) ? file->from(current_document) : nullptr;
			}
		private:
			file_factory_interface_t current_file_factory_;
		};

		template<typename GraphicsTraits>
		struct Document {
			using graphics_interface_t = typename GraphicsTraits::graphics_interface_t;
			using graphics_container_t = typename GraphicsTraits::graphics_container_t;
			using size_type = typename graphics_container_t::size_type;
		private:
			graphics_container_t graphics_container_;
		public:


			graphics_interface_t insert(graphics_interface_t primitive) {
				return GraphicsTraits::insert(graphics_container_, primitive);//may be a stub
			}
			void remove(graphics_interface_t primitive) {
				GraphicsTraits::remove(graphics_container_, primitive);//may be a stub
			}
			constexpr auto begin()GRAPHICS_EDITOR_PROJECT_DELEGATE_INVOKE(std::begin(graphics_container_));
			constexpr auto end()GRAPHICS_EDITOR_PROJECT_DELEGATE_INVOKE(std::end(graphics_container_));
			constexpr auto cbegin()const noexcept GRAPHICS_EDITOR_PROJECT_DELEGATE_INVOKE(std::cbegin(graphics_container_));
			constexpr auto cend()const noexcept GRAPHICS_EDITOR_PROJECT_DELEGATE_INVOKE(std::cend(graphics_container_));
			constexpr auto size()GRAPHICS_EDITOR_PROJECT_DELEGATE_INVOKE(std::size(graphics_container_));			
		};
	}
	template<typename GraphicsFactoryTraits>
	struct GraphicsFactory {
		using graphics_interface_t = typename GraphicsFactoryTraits::graphics_interface_t;
		graphics_interface_t make() {
			return GraphicsFactoryTraits::make();//may be a stub
		}
	};

	namespace file {
		using utilities::filepath_t;
		template<typename FileFactoryTraits>
		struct FileFactory {
			using file_interface_t = typename FileFactoryTraits::file_interface_t;
			file_interface_t load(filepath_t filepath) {
				return FileFactoryTraits::load(filepath);//may be a stub
			}
			file_interface_t save(filepath_t filepath) {
				return FileFactoryTraits::save(filepath);//may be a stub
			}
		};

		template<typename FileTraits>
		struct File {
			using file_interface_t = utilities::WrappedInterface<File>;
			using document_interface_t = typename FileTraits::document_interface_t;

			File(filepath_t filepath) : filepath_(filepath) {}

			//read data FROM file in the filesystem to the instance of the class
			document_interface_t from(document_interface_t document) {
				return FileTraits::from(filepath_, document);//may be a stub
			}
			//save data TO file in the filesystem from the constant instance of the class
			document_interface_t to(document_interface_t document)const {
				return FileTraits::to(filepath_, document);//may be a stub
			}
		private:
			filepath_t filepath_;
		};
	}
}
#endif // !GRAPHICS_EDITOR_PROJECT_EDITOR_HPP
