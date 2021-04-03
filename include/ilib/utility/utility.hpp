#ifndef U473903030300_T83930030_I739303030
#define U473903030300_T83930030_I739303030
#include "traits.hpp"

namespace ilib {
template <class T>
constexpr T&& forward(ilib::remove_reference_t<T>& t) noexcept {
    return static_cast<T&&>(t);
}
template <class T>
constexpr T&& forward(ilib::remove_reference_t<T>&& t) noexcept {
    return static_cast<T&&>(t);
}
template <class T>
constexpr ilib::remove_reference_t<T>&& move(T&& t) noexcept {
    return static_cast<ilib::remove_reference_t<T>&&>(t);
}

}  // namespace ilib
#endif
