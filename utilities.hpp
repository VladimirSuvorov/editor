#ifndef GRAPHICS_EDITOR_PROJECT_UTILITIES_HPP
#include <filesystem>
#define GRAPHICS_EDITOR_PROJECT_UTILITIES_HPP
namespace editor_project {
	namespace utilities {


		template<typename This>
		constexpr This* as_mutable(const This* t) {
			return const_cast<This*>(t);
		}


		//A view class (in opposite to an owner from C++ guideline support library) that not owns a pointer,
		//so neither allocation, nor deallocation must be performed with such pointer in a client code (by design).
		//Exceptional case for this rule is a pool or a factory class, that has methods, such as produce/recycle,
		//which are returns and takes as an argument such pointer respectively, can do peform allocation/deallocation internally.
		//But still, no clients code must perform allocation/deallocation. Just think about it like a handler, not a pointer.
		template<typename T>
		class ptr_view {
		public:
			using value_type = T;

			//constuctible from a pointer or a reference and nullptr-ed by default 
			ptr_view(T* data = nullptr) :ptr_(data) {}
			ptr_view(T& data) :ptr_(&data) {}

			T* operator=(T* oth) {
				return ptr_ = oth;
			}
			T& operator=(T& oth) {
				return *(ptr_ = &oth);
			}

			T* operator->() {
				return ptr_;
			}
			const T* operator->()const {
				return as_mutable(this)->operator->();
			}
			T& operator*() {
				return *ptr_;
			}
			const T& operator*()const {
				return as_mutable(this)->operator*();
			}
			operator T* () {
				return ptr_;
			}
			operator const T* ()const {
				return as_mutable(this)->operator T* ();
			}

		private:
			T* ptr_ = nullptr;
		};

		using filepath_t = std::filesystem::path;

		//An interface type should be wrapped into a somelike a pointer type. What the wrapper is used defined by the following alias.
		template<typename T>
		using WrappedInterface =  ptr_view<T>;

		//An interface type, wrapped into somelike a pointer type should be unwrapable. So, how to get an exact interface type is defined by the following alias.
		template<typename InterfaceWrapperT>
		using UnwrappedInterface = typename InterfaceWrapperT::value_type;
	}
}
#define GRAPHICS_EDITOR_PROJECT_DELEGATE_INVOKE(...)\
/*noexcept(__VA_ARGS__)*/->decltype(__VA_ARGS__) {\
	return __VA_ARGS__;\
}

#endif // !GRAPHICS_EDITOR_PROJECT_UTILITIES_HPP
