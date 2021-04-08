#ifndef H73839300303_F733903030030_G83939300303
#define H73839300303_F733903030030_G83939300303
#include <type_traits>
namespace ilib {
template <class T>
struct Ok {
    template <class... U>
    explicit constexpr Ok(U&&... args) noexcept(
        std::is_nothrow_constructible_v<T, U>)
        : m_value{std::forward<U>(args)...} {}

    explicit constexpr Ok() noexcept(std::is_nothrow_default_constructible_v<T>)
        : m_value{} {}

    constexpr T get_value() const noexcept { return m_value; }

   private:
    T m_value;
};
template <class T>
struct Err {
    template <class... U>
    explicit constexpr Err(U&&... args) noexcept(
        std::is_nothrow_constructible_v<T, U>)
        : m_value{std::forward<U>(args)...} {}

    explicit constexpr Err() noexcept(
        std::is_nothrow_default_constructible_v<T>)
        : m_value{} {}

    constexpr T get_value() const noexcept { return m_value; }

   private:
    T m_value;
};
template <class T, class U>
class Result {
    constexpr Result(Ok<T>&& ok) noexcept(noexcept(ok))
        : m_is_ok{true}, m_ok_ins{ok} {}

    constexpr Result(Err<U>&& err) noexcept(noexcept(err))
        : m_is_err{true}, m_err_ins{err} {}

   private:
    bool m_is_ok;
    Ok<T> m_ok_ins;
    bool m_is_err;
    Err<U> m_err_ins;
};
}  // namespace ilib
#endif

