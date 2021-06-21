#ifndef U473903030300_T83930030_I739303030
#define U473903030300_T83930030_I739303030

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <utility>

#include "iterators.hpp"
#include "traits.hpp"

namespace ilib::detail {
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

template <class T>
concept pretty_able = requires(T x, std::ostream o) {
    o << x;
};

using std::begin;
using std::end;

template <class...>
using void_t = void;

template <typename T, typename = void>
struct is_iterable : std::false_type {};

template <typename T>
struct is_iterable<T, void_t<decltype(begin(std::declval<T&>())), decltype(end(std::declval<T&>()))> >
    : std::true_type {};

template <typename T>
constexpr bool is_iterable_v = is_iterable<T>::value;

#define ILIB_ASSERT(expr, msg) ilib::detail::do_assert_(expr, {__FILE__, __LINE__, __func__, #expr}, msg)

#define ILIB_ASSERT_N(expr)    ilib::detail::do_assert_(expr, {__FILE__, __LINE__, __func__, #expr}, "")

#define ILIB_LOG(msg)          ilib::detail::do_log_({__FILE__, __LINE__}, msg);
}  // namespace ilib::detail
#define ILIB_FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
#define ILIB_LIFT(...)                    \
    [](auto&&... args) noexcept(noexcept( \
                           __VA_ARGS__(ILIB_FWD(args...))) -> decltype(__VA_ARGS__(ILIB_FWD(args...))) { return __VA_ARGS__(ILIB_FWD(args...)); }
namespace ilib {
std::string pretty(std::string str) {
    return str.insert(0, 1, '"').insert(str.size(), 1, '"');
}
template <class T, class U>
std::string pretty(const std::pair<T, U>& p) {
    return std::ostringstream{"{"} << pretty(p.first) << ',' << pretty(p.second) << '}';
}
template <class T>
std::string pretty(T arg) noexcept requires(detail::pretty_able<T>) {
    return (std::ostringstream{} << arg).str();
}

template <class Iter>
std::string pretty_range(Iter first, Iter last) {
    auto size = std::distance(first, last);
    std::string ss{"["};
    for (const auto& [index, element] : ilib::enumerator(first, last)) {
        ss += ilib::pretty(element);
        if (index != size) ss.push_back(',');
    }
    ss += "]";
    return ss;
}

template <class C>
std::string pretty(C c) noexcept requires(detail::is_iterable_v<C>) {
    using std::begin;
    using std::end;
    return pretty_range(begin(c), end(c));
}

// a pair that's not bloated
template <class T, class U>
struct simple_pair {
    T first;
    U second;
};

}  // namespace ilib
#endif
