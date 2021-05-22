#ifndef H73839300303_F733903030030_G83939300303
#define H73839300303_F733903030030_G83939300303
#include <string_view>
#include <type_traits>
#include <utility>

#include "option.hpp"

namespace ilib {
namespace detail {

struct res_exception : std::exception {
    res_exception(std::string_view str) : msg{str} {}

    const char* what() const noexcept override {
        std::string full{"ilib::result error: '"};
        (full += msg) += '\'';
        return full.c_str();
    }

   private:
    std::string_view msg;
};
}  // namespace detail
template <class T>
struct Ok {
    // clang-format off
    template<class U> //requires std::is_constructible_v<T, U>
    explicit constexpr Ok(U&& arg) noexcept(std::is_nothrow_constructible_v<T, U>)
        : value{std::forward<U>(arg)} {}

    template<class U>// requires std::is_constructible_v<T, U>
    explicit constexpr Ok(const Ok<U>& other) noexcept
        : value{other.value} {}

    explicit constexpr Ok() noexcept(std::is_nothrow_default_constructible_v<T>)
        : value{} {}

    operator T() { return value; }

    T value;
};
template <class T>
struct Err {
    // clang-format off
    template<class U> requires std::is_constructible_v<T, U>
    explicit constexpr Err(U&& arg) noexcept(std::is_nothrow_constructible_v<T, U>)
        : value{std::forward<U>(arg)} {}


    template<class U> requires std::is_constructible_v<T, U>
    explicit constexpr Err(const Err<U>& other) noexcept
        : value{other.value} {}

    // clang-format on
    explicit constexpr Err() noexcept(std::is_nothrow_default_constructible_v<T>) : value{} {}

    operator T() {
        return value;
    }

    T value;
};

template <class T>
Err(T) -> Err<T>;

template <class T>
Ok(T) -> Ok<T>;

template <class T, class U>
struct Result {
    // clang-format off
    template<class R> requires std::is_convertible_v<R, T>
    constexpr Result(Ok<R>&& ok) noexcept(
        std::is_nothrow_constructible_v<Ok<T>, R>)
        : m_is_ok{true}, m_ok_val{std::exchange(ok.value, {})}, m_is_err{} {}

    template<class R> requires std::is_convertible_v<R, T>
    constexpr Result(const Ok<R>& ok) noexcept(
        std::is_nothrow_constructible_v<Ok<T>, R>)
        : m_is_ok{true}, m_ok_val{ok.value}, m_is_err{} {}


    template<class R> requires std::is_convertible_v<R, U>
    constexpr Result(Err<R>&& err) noexcept(
        std::is_nothrow_constructible_v<Err<U>, R>)
        : m_is_ok{}, m_is_err{true}, m_err_val{std::exchange(err.value, {})} {}


    template<class R> requires std::is_convertible_v<U, R>
    constexpr Result(const Err<R>& err) noexcept(
        std::is_nothrow_constructible_v<Err<U>, R>)
        : m_is_ok{}, m_is_err{true}, m_err_val{err} {}

    constexpr bool is_ok() const noexcept { return m_is_ok; }

    constexpr bool is_err() const noexcept { return m_is_err; }

    template<class E> requires std::is_convertible_v<E, T>
    constexpr bool contains(E&& value) const noexcept {
        if (is_ok())
            return m_ok_val == value;
        else
            return false;
    }

    template<class D> requires std::is_convertible_v<D, U>
    constexpr bool contains_err(D&& value) const noexcept {
        if (is_err())
            return m_err_val == value;
        else
            return false;
    }

    template<class E> requires std::is_convertible_v<E, T>
    constexpr bool operator==(const Ok<E>& t_ok) const noexcept {
        if (is_ok() && contains(t_ok.value))
            return true;
        else
            return false;
    }
    template<class D> requires std::is_convertible_v<D, U>
    constexpr bool operator==(const Err<D>& t_err) const noexcept {
        if (is_err() && contains_err(t_err.value))
            return true;
        else
            return false;
    }

    constexpr bool operator==(const Result& other) const noexcept {
        if(is_ok() && other.is_ok()) {
            return contains(other.unwrap());
        } else if(is_err() && other.is_err()) {
            return contains_err(other.unwrap_err());
        } else {
            return false;
        }
    }

    template<class R, class E> requires std::is_convertible_v<R, T> && std::is_convertible_v<E, U>
    constexpr void swap(Result<R, E>&& other) noexcept {
        using std::swap;
        if(is_ok() && other.is_ok()) {
            swap(m_ok_val, other.m_ok_val);
        } else if(is_err() && other.is_err()) {
            swap(m_err_val, other.m_err_val);
        }
        swap(m_is_ok, other.m_is_ok);
        swap(m_is_err, other.m_is_err);
    }


    template<class R, class E> requires std::is_convertible_v<R, T> 
        && std::is_convertible_v<E, U>
    constexpr Result& operator=(Result<R, E>&& other) noexcept {
        swap(std::move(other));
        return *this;
    }

    template<class R, class E> requires std::is_convertible_v<R, T> 
        && std::is_convertible_v<E, U>
    constexpr Result& operator=(Result<R, E> other) noexcept {
        swap(std::move(other));
        return *this;
    }

    constexpr Option<T> ok() const noexcept {
        if (is_ok()) {
            return ilib::Some(m_ok_val);
        } else {
            return ilib::None;
        }
    }

    constexpr const T& unwrap() const& {
        if (is_ok())
            return m_ok_val;
        else
            throw detail::res_exception("bad Result access");
    }
    constexpr T&& unwrap() && {
        if (is_ok())
            return std::move(m_ok_val);
        else
            throw detail::res_exception("bad Result access");
    }

    constexpr const U& unwrap_err() const& {
        if (is_err())
            return m_err_val;
        else
            throw detail::res_exception("bad Result access");
    }
    constexpr U&& unwrap_err() && {
        if (is_err())
            return std::move(m_err_val);
        else
            throw detail::res_exception("bad Result access");
    }
    template <class R> requires std::is_convertible_v<R, T> 
    constexpr const T& unwrap_or(R&& opt) const& noexcept {
        if (is_ok())
            return m_ok_val;
        else
            return std::forward<R>(opt);
    }

    template <class R> requires std::is_convertible_v<R, T> 
    constexpr T&& unwrap_or(R&& opt) && noexcept {
        if (is_ok())
            return std::move(m_ok_val);
        else
            return std::forward<R>(opt);
    }
    template <class F>
    requires std::is_convertible_v<std::invoke_result_t<F, U>, T> 
    constexpr const T& unwrap_or_else(F&& func) const& noexcept(std::is_nothrow_invocable_v<F, U>) {
        if (is_ok())
            return m_ok_val;
        else
            return 
                std::forward<F>(func)(m_err_val);
    }

    template <class F>
    requires std::is_convertible_v<std::invoke_result_t<F, U>, T> 
    constexpr T&& unwrap_or_else(F&& func) && noexcept(std::is_nothrow_invocable_v<F, U>) {
        if (is_ok())
            return std::move(m_ok_val);
        else
            return 
                std::forward<F>(func)(m_err_val);
    }

    // clang-format on

    constexpr auto as_ref() const noexcept -> Result<std::reference_wrapper<T>, std::reference_wrapper<U>> {
        if (is_ok())
            return {Ok<std::reference_wrapper<T>>{std::ref(m_ok_val)}};
        else
            return {Err<std::reference_wrapper<U>>{std::ref(m_err_val.get_value)()}};
    }
    constexpr auto as_cref() const noexcept
        -> Result<std::reference_wrapper<const T>, std::reference_wrapper<const U>> {
        if (is_ok())
            return {Ok<std::reference_wrapper<const T>>{std::cref(m_ok_val)}};
        else
            return {Err<std::reference_wrapper<const U>>{std::cref(m_err_val)}};
    }

    template <class F>
    constexpr auto map(F&& func) const
        noexcept(std::is_nothrow_invocable_r_v<Result<std::invoke_result_t<F, T>, U>, F, T>)
            -> Result<std::invoke_result_t<F, T>, U> {
        if (is_ok()) {
            return {Ok<std::invoke_result_t<F, T>>(std::forward<F>(func)(ok().unwrap()))};
        } else {
            return m_err_val;
        }
    }

    template <class F>
    constexpr auto map_err(F&& func) const
        noexcept(std::is_nothrow_invocable_r_v<Result<T, std::invoke_result_t<F, U>>, F, U>)
            -> Result<T, std::invoke_result_t<F, U>> {
        if (is_ok()) {
            return {Err<std::invoke_result_t<F, U>>(std::forward<F>(func)(m_err_val))};
        } else {
            return m_err_val;
        }
    }

    template <class E>
    constexpr auto and_(const Result<E, U>& res) const noexcept -> Result<E, U> {
        if (is_ok())
            return res;
        else
            return m_err_val;
    }

    template <class F>
    constexpr auto and_then(F&& func) const noexcept(std::is_nothrow_invocable_r_v<Result<T, U>, F, T>) -> Result<T, U>
    requires(std::is_invocable_r_v<Result<T, U>, F, T>) {
        if (is_ok())
            return {Ok{std::forward<F>(func)(ok().unwrap())}};
        else
            return m_err_val;
    }
    template <class E>
    constexpr auto or_(const Result<E, U>& res) const noexcept -> Result<E, U> {
        if (is_ok())
            return *this;
        else
            return res;
    }

    template <class F>
    constexpr auto or_else(F&& func) const noexcept(std::is_nothrow_invocable_v<F, U>)
        -> Result<std::invoke_result_t<F, U>, U> {
        if (is_ok())
            return *this;
        else
            return std::forward<F>(func)(m_err_val);
    }

   private:
    bool m_is_ok;
    T m_ok_val;
    bool m_is_err;
    U m_err_val;
};
}  // namespace ilib
#endif

