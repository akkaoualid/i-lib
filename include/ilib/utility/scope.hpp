#ifndef G37390303000_U738393030300_F3930204737273839
#define G37390303000_U738393030300_F3930204737273839
#include <exception>
#include <type_traits>
namespace ilib {
template <class T>
struct scope_exit {
    template <class U>
    scope_exit(U&& fnc) noexcept(
        noexcept(std::is_nothrow_constructible_v<T, U> ||
                 std::is_nothrow_constructible_v<T, U&>))
        : m_is_active{true} {
        if constexpr (std::is_nothrow_constructible_v<T, U>) {
            m_func = std::forward<U>(fnc);
        } else {
            m_func = fnc;
        }
    }

    scope_exit& operator=(const scope_exit&) = delete;

    scope_exit(scope_exit&& se) noexcept(
        noexcept(std::is_nothrow_move_constructible_v<T> ||
                 std::is_nothrow_copy_constructible_v<T>)) {
        if constexpr (std::is_nothrow_move_constructible_v<T> ||
                      std::is_copy_constructible_v<T>) {
            m_func = se.m_func;
            m_is_active = se.m_is_active;
            se.release();
        }
    }
    scope_exit(const scope_exit&) = delete;

    void release() { m_is_active = false; }

    ~scope_exit() noexcept {
        if (m_is_active) m_func();
    }

   private:
    bool m_is_active;
    T m_func;
};
template <class T>
struct scope_success {
    template <class U>
    scope_success(U&& fnc) noexcept(
        noexcept(std::is_nothrow_constructible_v<T, U> ||
                 std::is_nothrow_constructible_v<T, U&>))
        : m_is_active{true}, m_exc_count{std::uncaught_exceptions()} {
        if constexpr (std::is_nothrow_constructible_v<T, U>) {
            m_func = std::forward<U>(fnc);
        } else {
            m_func = fnc;
        }
    }

    scope_success& operator=(const scope_success&) = delete;

    scope_success(scope_success&& se) noexcept(
        noexcept(std::is_nothrow_move_constructible_v<T> ||
                 std::is_nothrow_copy_constructible_v<T>)) {
        if constexpr (std::is_nothrow_move_constructible_v<T> ||
                      std::is_copy_constructible_v<T>) {
            m_func = se.m_func;
            m_is_active = se.m_is_active;
            se.release();
        }
    }
    scope_success(const scope_success&) = delete;

    void release() { m_is_active = false; }

    ~scope_success() noexcept {
        if (m_is_active && (std::uncaught_exceptions() <= m_exc_count))
            m_func();
    }

   private:
    bool m_is_active;
    int m_exc_count;
    T m_func;
};
template <class T>
struct scope_fail {
    template <class U>
    scope_fail(U&& fnc) noexcept(
        noexcept(std::is_nothrow_constructible_v<T, U> ||
                 std::is_nothrow_constructible_v<T, U&>))
        : m_is_active{true}, m_exc_count{std::uncaught_exceptions()} {
        if constexpr (std::is_nothrow_constructible_v<T, U>) {
            m_func = std::forward<U>(fnc);
        } else {
            m_func = fnc;
        }
    }

    scope_fail& operator=(const scope_fail&) = delete;

    scope_fail(scope_fail&& se) noexcept(
        noexcept(std::is_nothrow_move_constructible_v<T> ||
                 std::is_nothrow_copy_constructible_v<T>)) {
        if constexpr (std::is_nothrow_move_constructible_v<T> ||
                      std::is_copy_constructible_v<T>) {
            m_func = se.m_func;
            m_is_active = se.m_is_active;
            se.release();
        }
    }
    scope_fail(const scope_fail&) = delete;

    void release() { m_is_active = false; }

    ~scope_fail() noexcept {
        if (m_is_active && (std::uncaught_exceptions() > m_exc_count)) m_func();
    }

   private:
    bool m_is_active;
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
