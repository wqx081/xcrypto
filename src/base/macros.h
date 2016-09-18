// This file contains macros and macro-like constructs (e.g., templates) that
// are commonly used throughout Chromium source. (It may also contain things
// that are closely related to things that are commonly used that belong in this
// file.)

#ifndef BASE_MACROS_H_
#define BASE_MACROS_H_

#include <stddef.h>  // For size_t.

#include <memory>

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
inline const void* const_cast_sockopt(const T* v) {
  return reinterpret_cast<const void *>(v);
}

template<typename T>
inline void* cast_sockopt(T* v) {
  return reinterpret_cast<void *>(v);
}

// Put this in the declarations for a class to be uncopyable.
#define DISALLOW_COPY(TypeName) \
  TypeName(const TypeName&) = delete

// Put this in the declarations for a class to be unassignable.
#define DISALLOW_ASSIGN(TypeName) \
  void operator=(const TypeName&) = delete

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

// A macro to disallow all the implicit constructors, namely the
// default constructor, copy constructor and operator= functions.
//
// This should be used in the private: declarations for a class
// that wants to prevent anyone from instantiating it. This is
// especially useful for classes containing only static methods.
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName() = delete;                           \
  DISALLOW_COPY_AND_ASSIGN(TypeName)

// The arraysize(arr) macro returns the # of elements in an array arr.  The
// expression is a compile-time constant, and therefore can be used in defining
// new arrays, for example.  If you use arraysize on a pointer by mistake, you
// will get a compile-time error.  For the technical details, refer to
// http://blogs.msdn.com/b/the1/archive/2004/05/07/128242.aspx.

// This template function declaration is used in defining arraysize.
// Note that the function doesn't need an implementation, as we only
// use its type.
template <typename T, size_t N> char (&ArraySizeHelper(T (&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))

// Used to explicitly mark the return value of a function as unused. If you are
// really sure you don't want to do anything with the return value of a function
// that has been marked WARN_UNUSED_RESULT, wrap it with this. Example:
//
//   scoped_ptr<MyType> my_var = ...;
//   if (TakeOwnership(my_var.get()) == SUCCESS)
//     ignore_result(my_var.release());
//
template<typename T>
inline void ignore_result(const T&) {
}

// The following enum should be used only as a constructor argument to indicate
// that the variable has static storage class, and that the constructor should
// do nothing to its state.  It indicates to the reader that it is legal to
// declare a static instance of the class, provided the constructor is given
// the base::LINKER_INITIALIZED argument.  Normally, it is unsafe to declare a
// static variable that has a constructor or a destructor because invocation
// order is undefined.  However, IF the type can be initialized by filling with
// zeroes (which the loader does for static variables), AND the destructor also
// does nothing to the storage, AND there are no virtual methods, then a
// constructor declared as
//       explicit MyClass(base::LinkerInitialized x) {}
// and invoked as
//       static MyClass my_variable_name(base::LINKER_INITIALIZED);
namespace base {
enum LinkerInitialized { LINKER_INITIALIZED };

// Use these to declare and define a static local variable (static T;) so that
// it is leaked so that its destructors are not called at exit. If you need
// thread-safe initialization, use base/lazy_instance.h instead.
#define CR_DEFINE_STATIC_LOCAL(type, name, arguments) \
  static type& name = *new type arguments

}  // base

#include <memory>
#include <type_traits>
#include <utility>

namespace std {
    template<class T> struct _Unique_if {
        typedef unique_ptr<T> _Single_object;
    };

    template<class T> struct _Unique_if<T[]> {
        typedef unique_ptr<T[]> _Unknown_bound;
    };

    template<class T, size_t N> struct _Unique_if<T[N]> {
        typedef void _Known_bound;
    };

    template<class T, class... Args>
        typename _Unique_if<T>::_Single_object
        make_unique(Args&&... args) {
            return unique_ptr<T>(new T(std::forward<Args>(args)...));
        }

    template<class T>
        typename _Unique_if<T>::_Unknown_bound
        make_unique(size_t n) {
            typedef typename remove_extent<T>::type U;
            return unique_ptr<T>(new U[n]());
        }

    template<class T, class... Args>
        typename _Unique_if<T>::_Known_bound
        make_unique(Args&&...) = delete;
}

#define MOVE_ONLY_TYPE_FOR_CPP_03(type) \
	  DISALLOW_COPY_AND_ASSIGN_WITH_MOVE_FOR_BIND(type)

#define DISALLOW_COPY_AND_ASSIGN_WITH_MOVE_FOR_BIND(type)       \
	 private:                                                       \
  type(const type&) = delete;                                   \
  void operator=(const type&) = delete;                         \
                                                                \
 public:                                                        \
  typedef void MoveOnlyTypeForCPP03;                            \
                                                                \
 private:


template <typename T, size_t N> char (&ArraySizeHelper(T (&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))
#define ARRAYSIZE(array) arraysize(array)

#define ALIGNAS(byte_alignment) __attribute__((aligned(byte_alignment)))
#define ALIGNOF(type) __alignof__(type)


#define GG_LONGLONG(x) x##LL
#define GG_ULONGLONG(x) x##ULL
#define GG_LL_FORMAT "ll"  // As in "%lld". Note that "q" is poor form also.
#define GG_LL_FORMAT_W L"ll"

#define UNALIGNED_LOAD16(_p) (*reinterpret_cast<const uint16_t *>(_p))
#define UNALIGNED_LOAD32(_p) (*reinterpret_cast<const uint32_t *>(_p))
#define UNALIGNED_LOAD64(_p) (*reinterpret_cast<const uint64_t *>(_p))

#define UNALIGNED_STORE16(_p, _val) (*reinterpret_cast<uint16_t *>(_p) = (_val))
#define UNALIGNED_STORE32(_p, _val) (*reinterpret_cast<uint32_t *>(_p) = (_val))
#define UNALIGNED_STORE64(_p, _val) (*reinterpret_cast<uint64_t *>(_p) = (_val))

#define DECLARE_POD(TypeName) typedef int Dummy_Type_For_DECLARE_POD
#define DECLARE_NESTED_POD(TypeName) DECLARE_POD(TypeName)
#define PROPAGATE_POD_FROM_TEMPLATE_ARGUMENT(TemplateName)             \
      typedef int Dummy_Type_For_PROPAGATE_POD_FROM_TEMPLATE_ARGUMENT
#define ENFORCE_POD(TypeName) typedef int Dummy_Type_For_ENFORCE_POD

#define PRINTF_ATTRIBUTE(string_index, first_to_check) \
    __attribute__((__format__ (__printf__, string_index, first_to_check)))
#define SCANF_ATTRIBUTE(string_index, first_to_check) \
    __attribute__((__format__ (__scanf__, string_index, first_to_check)))

#define PREDICT_FALSE(x) (__builtin_expect(x, 0))
#define PREDICT_TRUE(x) (__builtin_expect(!!(x), 1))

#endif  // BASE_MACROS_H_
