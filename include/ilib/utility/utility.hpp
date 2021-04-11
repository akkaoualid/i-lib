#ifndef U473903030300_T83930030_I739303030
#define U473903030300_T83930030_I739303030

#include <cstdio>
#include <cstdlib>

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
    if (!c_) {
        std::fprintf(stderr,
                     "[ilib::assert] %s:%zu: assertion failed '%s' in function "
                     "'%s' %s\n",
                     sl.file_, sl.line_, sl.expression_, sl.func_, msg);
        std::abort();
    }
}

inline void do_log_(source_loc_ sl, const char* msg) {
    std::printf("[ilib::log] %s:%zu: %s\n", sl.file_, sl.line_, msg);
}

#define ILIB_ASSERT(expr, msg) \
    ilib::detail::do_assert_(expr, {__FILE__, __LINE__, __func__, #expr}, msg)

#define ILIB_ASSERT_N(expr) \
    ilib::detail::do_assert_(expr, {__FILE__, __LINE__, __func__, #expr}, "")

#define ILIB_LOG(msg) ilib::detail::do_log_({__FILE__, __LINE__}, msg);
}  // namespace detail
}  // namespace ilib
#define ILIB_FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
#define ILIB_LIFT(...)                    \
    [](auto&&... args) noexcept(noexcept( \
                           __VA_ARGS__(ILIB_FWD(args...))) -> decltype(__VA_ARGS__(ILIB_FWD(args...))) { return __VA_ARGS__(ILIB_FWD(args...)); }
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
