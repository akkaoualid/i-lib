#ifndef G748393046379399_X7383002938380203828_V393929477493030383883
#define G748393046379399_X7383002938380203828_V393929477493030383883
#include <type_traits>
#include <utility>

namespace ilib {
template <class F>
constexpr void call_times(F&& func, std::size_t n) noexcept(std::is_nothrow_invocable_v<F>) {
    while (--n) func();
}

// clang-format off
template <class FT, class FU>
constexpr auto compose_with(FT&& left, FU&& right) {
    return [&]<class... T>(T&&... args) constexpr noexcept(std::is_nothrow_invocable_v<FT, std::invoke_result_t<FU, T...>>) -> decltype(auto) {
        return std::forward<FU>(left)(std::forward<FU>(right)(std::forward<T>(args)...));
    };
}
// clang-format on

template <class F, class... A>
constexpr auto compose(F&& func, A&&... args) noexcept {
    return [&]() noexcept(std::is_nothrow_invocable_v<F, A...>) {
        return std::forward<F>(func)(std::forward<A>(args)...);
    };
}

template <class F, class... A>
constexpr auto negate(F&& func, A... args) noexcept
    requires(std::is_convertible_v<std::invoke_result_t<F, A...>, bool>) {
    return [&]() noexcept(std::is_nothrow_invocable_v<F, A...>) {
        return !std::forward<F>(func)(std::forward<A>(args)...);
    };
}

template <class B>
struct functor {
    template <class... A>
    constexpr auto operator()(A&&... args) noexcept {
        return static_cast<B*>(this)->operator()(std::forward<A>(args)...);
    }
};

struct count : functor<count> {
    template <class T>
    constexpr std::size_t operator()(T&& b) noexcept {
        return std::size(b);
    }
};

template <class F>
struct filter : functor<filter<F>> {
    constexpr filter(F f) noexcept : fn{f} {}

   private:
    F fn;
};

namespace operators {
template <class Ft, class Fu>
constexpr auto operator|(const functor<Fu>& left, const functor<Fu>& right) noexcept
    requires(std::is_invocable_v<Ft>&& std::is_invocable_v<Fu>) {
    return [&]<class A...>(A && ... args) {
        return left(right(std::forward<A>(args...)));
    };
}
}  // namespace operators

namespace detail {
namespace {
using namespace operators;
template <class... F>
constexpr auto compose_with(F&&... fns) noexcept {
    return (... | fun<F>{});
}
}  // namespace
}  // namespace detail

template <class... F>
constexpr auto compose_with(F&&... fns) noexcept {
    return detail::compose_with(std::forward<F>(fns)...);
}

}  // namespace ilib
#endif
