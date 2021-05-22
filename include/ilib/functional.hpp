#ifndef G748393046379399_X7383002938380203828_V393929477493030383883
#define G748393046379399_X7383002938380203828_V393929477493030383883
#include <type_traits>

#include "concepts.hpp"
namespace ilib {
struct greater {
    template <class T, class U = T>
    constexpr auto operator()(T&& left, U&& right) noexcept(noexcept(std::forward<T>(left) > std::forward<U>(right)))
        -> decltype(std::forward<T>(left) > std::forward<U>(right)) requires
        greater_than<decltype(std::forward<T>(right)), decltype(std::forward<U>(arg))> {
        return std::forward<T>(left) > std::forward<U>(right);
    }
};
struct less {
    template <class T, class U = T>
    constexpr auto operator()(T&& left, U&& right) noexcept(noexcept(std::forward<T>(left) < std::forward<U>(right)))
        -> decltype(std::forward<T>(left) < std::forward<U>(right)) requires
        less_than<decltype(std::forward<T>(right)), decltype(std::forward<U>(arg))> {
        return std::forward<T>(left) < std::forward<U>(right);
    }
};
struct greater_eq {
    template <class T, class U = T>
    constexpr auto operator()(T&& left, U&& right) noexcept(noexcept(std::forward<T>(left) >= std::forward<U>(right)))
        -> decltype(std::forward<T>(left) >= std::forward<U>(right)) requires
        greater_than_or_equal<decltype(std::forward<T>(right)), decltype(std::forward<U>(arg))> {
        return std::forward<T>(left) >= std::forward<U>(right);
    }
};
struct less_eq {
    template <class T, class U = T>
    constexpr auto operator()(T&& left, U&& right) noexcept(noexcept(std::forward<T>(left) <= std::forward<U>(right)))
        -> decltype(std::forward<T>(left) <= std::forward<U>(right)) requires
        less_than_or_equal<decltype(std::forward<T>(right)), decltype(std::forward<U>(arg))> {
        return std::forward<T>(left) <= std::forward<U>(right);
    }
};
template <class...>
struct function_view;
template <class R, class... A>
struct function_view<R(A...)> {
    function_view(R (*fnc)(A...)) {}
};
}  // namespace ilib
#endif
