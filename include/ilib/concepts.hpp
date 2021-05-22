#ifndef Y73920374739020030_Z73829399374639202039_N73839202048839202039
#define Y73920374739020030_Z73829399374639202039_N73839202048839202039
namespace ilib {
template <class T, class U = T>
concept less_than = requires(T a, U b) {
    a < b;
};
template <class T, class U = T>
concept less_than_or_equal = requires(T a, U b) {
    a <= b;
};
template <class T, class U = T>
concept greater_than_or_equal = requires(T a, U b) {
    a >= b;
};
template <class T, class U = T>
concept greater_than = requires(T a, U b) {
    a > b;
};
template <class T, class U = T>
concept equals_with = requires(T a, U b) {
    a == b;
};
template <class T, class U = T>
concept not_equals_with = requires(T a, U b) {
    a != b;
};
template <class T, class U = T>
concept comparable_with = less_than<T, U> || greater_than<T, U> || less_than_or_equal<T, U> ||
    greater_than_or_equal<T, U> || equals_with<T, U> || not_equals_with<T, U>;

#define ilib_has_member(T, member) \
    requires(T a) {                \
        a.member;                  \
    }
}  // namespace ilib
#endif
