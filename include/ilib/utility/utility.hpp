#ifndef U473903030300_T83930030_I739303030
#define U473903030300_T83930030_I739303030

#include <cstdio>

#include "traits.hpp"

namespace ilib {
namespace detail {
struct source_loc_ {
    const char* file_;
    unsigned long line_;
    const char* func_;
    const char* expression_;
};
inline void do_assert_(bool c_, source_loc_ sl, const char* msg) {
    if (!c_)
        std::fprintf(stderr,
                     "[ilib::assert] %s:%zu: assertion failed '%s' in function "
                     "'%s' - %s",
                     sl.file_, sl.line_, sl.expression_, sl.func_, msg);
}

#define ILIB_ASSERT(expr, msg) \
    ilib::detail::do_assert_(expr, {__FILE__, __LINE__, __func__, #expr}, msg)

}  // namespace detail
}  // namespace ilib
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
