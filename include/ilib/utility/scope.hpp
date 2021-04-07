#ifndef G37390303000_U738393030300_F3930204737273839
#define G37390303000_U738393030300_F3930204737273839
#include <exception>
#include <type_traits>
namespace ilib {
template <class T>
struct scope_exit {
    /*! \brief constructs the callable object.
     *
     * noexcept if the initialzation of T from U is nothrow. Constructs the
     * callable if no exception is thrown otherwise calls the callable during
     * initialization.
     *
     * @param fnc the callable object
     */
    template <class U>
    // clang-format off
    scope_exit(U&& fnc) noexcept(std::is_nothrow_constructible_v<T, U> ||
                                 std::is_nothrow_constructible_v<T, U&>) 
    try : m_func{std::forward<U>(fnc)}
    {}
    catch (...) {
        fnc();
        throw;
    }
    // clang-format on

    scope_exit& operator=(const scope_exit&) = delete;

    scope_exit(scope_exit&& se) noexcept(
        std::is_nothrow_move_constructible_v<T> ||
        std::is_nothrow_copy_constructible_v<T>)
        : m_func{std::forward<T>(se.m_funv)} {}

    scope_exit(const scope_exit&) = delete;

    /*! \brief calls the function object with attention to ref-qualifiers
     */
    ~scope_exit() { std::forward<T>(m_func)(); }

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
    // clang-format off
    scope_fail(U&& fnc) noexcept(std::is_nothrow_constructible_v<T, U> ||
                                 std::is_nothrow_constructible_v<T, U&>) 
    try : m_exc_count {std::uncaught_exceptions()}
    , m_func{std::forward<U>(fnc)} {}
    catch (...) {
        fnc();
        throw;
    }
    // clang-format on

    scope_fail& operator=(const scope_fail&) = delete;

    scope_fail(scope_fail&& se) noexcept(
        std::is_nothrow_move_constructible_v<T> ||
        std::is_nothrow_copy_constructible_v<T>)
        : m_func{std::forward<T>(se.m_func)} {}

    scope_fail(const scope_fail&) = delete;

    ~scope_fail() {
        if (std::uncaught_exceptions() > m_exc_count) std::forward<T>(m_func)();
    }

   private:
    int m_exc_count;
    T m_func;
};
template <class R, class D>
struct unique_resource {
    unique_resource() noexcept(std::is_nothrow_default_constructible_v<R>&&
                                   std::is_nothrow_default_constructible_v<D>)
        : m_src{}, m_deleter{} {}

    template <class RR, class DD>
    // clang-format off
    unique_resource(RR&& r, DD&& d) noexcept(
        (std::is_nothrow_copy_constructible_v<R, RR> ||
         std::is_nothrow_copy_constructible_v<D, DD>)
        &&
        (std::is_nothrow_move_constructible_v<R, RR> ||
         std::is_nothrow_move_constructible_v<D, DD>))

    : m_src{std::forward<RR>(r)}, m_deleter{std::forward<DD>(d)} {}
    // clang-format on

    const R& get() const { return m_src; }

    ~unique_resource() { m_deleter(m_src); }

   private:
    R m_src;
    D m_deleter;
};
template <class F>
scope_exit(F) -> scope_exit<F>;
template <class F>
scope_success(F) -> scope_success<F>;
template <class F>
scope_fail(F) -> scope_fail<F>;
}  // namespace ilib
#endif
