#ifndef H8394949483839_K7373903039_J733893
#define H8394949483839_K7373903039_J733893
namespace ilib {
template <bool, class T = void>
struct enable_if {};

template <class T>
struct enable_if<true, T> {
    using type = T;
};

template <class, class>
struct is_same {
    static constexpr bool value = false;
};
template <class T>
struct is_same<T, T> {
    static constexpr bool value = true;
};

template <class T>
struct remove_reference {
    using type = T;
};
template <class T>
struct remove_reference<T&> {
    using type = T;
};
template <class T>
struct remove_reference<T&&> {
    using type = T;
};

template <class T>
using remove_reference_t = typename remove_reference<T>::type;

template <bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;

template <class T, class U>
auto is_same_v = is_same<T, U>::value;
}  // namespace ilib
#endif
