#ifndef G37390303000_U738393030300_F3930204737273839
#define G37390303000_U738393030300_F3930204737273839
#include <exception>
#include <type_traits>
namespace ilib {
template <class T>
struct scope_exit {
    template <class U>
    scope_exit(U&& fnc) noexcept(std::is_nothrow_constructible_v<T, U> ||
                                 std::is_nothrow_constructible_v<T, U&>)
        : m_func{std::forward<U>(fnc)} {}

    scope_exit& operator=(const scope_exit&) = delete;

    scope_exit(scope_exit&& se) noexcept(
        std::is_nothrow_move_constructible_v<T> ||
        std::is_nothrow_copy_constructible_v<T>)
        : m_func{std::forward<T>(se.m_funv)} {}

    scope_exit(const scope_exit&) = delete;

    ~scope_exit() noexcept(noexcept(std::forward<T>(m_func))) {
        std::forward<T>(m_func)();
    }

   private:
    T m_func;
};
template <class T>
struct scope_success {
    template <class U>
    scope_success(U&& fnc) noexcept(std::is_nothrow_constructible_v<T, U> ||
                                    std::is_nothrow_constructible_v<T, U&>)
        : m_exc_count{std::uncaught_exceptions()},
          m_func{std::forward<U>(fnc)} {}

    scope_success& operator=(const scope_success&) = delete;

    scope_success(scope_success&& se) noexcept(
        std::is_nothrow_move_constructible_v<T> ||
        std::is_nothrow_copy_constructible_v<T>)
        : m_func{std::forward<T>(se.m_func)} {}

    scope_success(const scope_success&) = delete;

    ~scope_success() noexcept(noexcept(std::forward<T>(m_func))) {
        if (std::uncaught_exceptions() <= m_exc_count)
            std::forward<T>(m_func)();
    }

   private:
    int m_exc_count;
    T m_func;
};
template <class T>
struct scope_fail {
    template <class U>
    scope_fail(U&& fnc) noexcept(std::is_nothrow_constructible_v<T, U> ||
                                 std::is_nothrow_constructible_v<T, U&>)
        : m_exc_count{std::uncaught_exceptions()},
          m_func{std::forward<U>(fnc)} {}

    scope_fail& operator=(const scope_fail&) = delete;

    scope_fail(scope_fail&& se) noexcept(
        std::is_nothrow_move_constructible_v<T> ||
        std::is_nothrow_copy_constructible_v<T>)
        : m_func{std::forward<T>(se.m_func)} {}

    scope_fail(const scope_fail&) = delete;

    ~scope_fail() noexcept(noexcept(std::forward<T>(m_func))) {
        if (std::uncaught_exceptions() > m_exc_count) std::forward<T>(m_func)();
    }

   private:
    int m_exc_count;
    T m_func;
};
template <class F>
scope_exit(F) -> scope_exit<F>;
template <class F>
scope_success(F) -> scope_success<F>;
template <class F>
scope_fail(F) -> scope_fail<F>;
}  // namespace ilib
#endif
