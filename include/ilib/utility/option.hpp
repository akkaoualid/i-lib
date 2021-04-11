#ifndef U9393020394873_A8303020372727_L839302003
#define U9393020394873_A8303020372727_L839302003
#include <functional>
#include <memory>
#include <new>
#include <string>
#include <type_traits>
#include <utility>

namespace ilib {
namespace detail {
struct opt_exception : std::exception {
    opt_exception(std::string_view str) : msg{str} {}

    const char* what() const noexcept override {
        std::string full{"ilib::option error: '"};
        (full += msg) += '\'';
        return full.c_str();
    }

   private:
    std::string_view msg;
};
}  // namespace detail
struct NoneT {};

static NoneT None{};
struct Empty {};

template <class T>
struct Some {
    using value_type = T;
    // clang-format off
    template<class U> requires std::is_convertible_v<U, T>
    constexpr explicit Some(U&& value) noexcept(
        std::is_nothrow_constructible_v<T, U>)
        : m_value{std::forward<U>(value)} {}

    constexpr explicit Some() noexcept(
            std::is_nothrow_default_constructible_v<T>)
    requires std::is_default_constructible_v<T> 
        : m_value{} {}

    // clang-format on

    constexpr const T& get_value() const noexcept { return m_value; }

   private:
    T m_value;
};

template <class T>
Some(T) -> Some<T>;

template <class T>
struct Option {
    using value_type = std::conditional_t<std::is_reference_v<T>,
                                          std::reference_wrapper<T>, T>;

    constexpr Option(NoneT) noexcept : m_has_value{false} {}

    // clang-format off
    template <class U> requires std::is_convertible_v<U, T> 
    constexpr Option(const Some<U>& some)
        : m_has_value{true} {
        m_construct_at(std::addressof(m_value), some.get_value());
    }

    template <class U> requires std::is_convertible_v<U, T> 
    constexpr Option(const Option<U>& other)
        : m_has_value{other.m_has_value}, m_value{other.m_value} {}

    template <class U> requires std::is_convertible_v<U, T> 
    constexpr Option(Option<U>&& other)
        : m_has_value{other.m_has_value}, m_value{std::move(other.m_value)} {}

    // clang-format on
    constexpr bool is_none() const noexcept { return !m_has_value; }

    constexpr bool is_some() const noexcept { return m_has_value; }

    constexpr const T& expect(std::string_view msg) const& {
        if (is_some()) {
            return m_value;
        } else {
            throw detail::opt_exception(msg);
        }
    }

    constexpr T&& expect(std::string_view msg) && {
        if (is_some()) {
            return std::move(m_value);
        } else {
            throw detail::opt_exception(msg);
        }
    }

    constexpr const T& unwrap() const& {
        if (is_some()) {
            return m_value;
        } else {
            throw detail::opt_exception("invalid Option access");
        }
    }

    constexpr T&& unwrap() && {
        if (is_some()) {
            return std::move(m_value);
        } else {
            throw detail::opt_exception("invalid Option access");
        }
    }

    template <class U>
    constexpr T unwrap_or(U&& other) const& noexcept {
        if (is_some()) {
            return m_value;
        } else {
            return std::forward<U>(other);
        }
    }
    template <class U>
    constexpr T unwrap_or(U&& other) && noexcept {
        if (is_some()) {
            return std::move(m_value);
        } else {
            return std::forward<U>(other);
        }
    }

    constexpr auto as_ref() noexcept -> Option<std::reference_wrapper<T>> {
        if (is_some())
            return Some(std::ref(m_value));
        else
            return None;
    }

    constexpr auto as_cref() noexcept
        -> Option<std::reference_wrapper<const T>> {
        if (is_some())
            return Some(std::cref(m_value));
        else
            return None;
    }

    template <class F>
    constexpr auto map(F&& func) -> Option<std::invoke_result_t<F, T>> {
        if (is_some()) {
            return Some(func(std::forward<T>(m_value)));
        } else {
            return None;
        }
    }

    constexpr T operator*() const noexcept(is_some()) { return unwrap(); }

    constexpr void swap(Option& other) noexcept(
        std::is_nothrow_swappable_v<T>) {
        using std::swap;
        if (is_some() && other.is_some()) {
            swap(m_value, other.m_value);
        } else if (other.is_some()) {
            m_construct_at(std::addressof(m_value), std::move(other.unwrap()));
            other.reset();
        } else if (is_some()) {
            m_construct_at(std::addressof(other.m_value), std::move(unwrap()));
            reset();
        }
    }

    constexpr Option& operator=(Option&& other) noexcept(
        std::is_nothrow_move_assignable_v<T> ||
        std::is_nothrow_move_constructible_v<T>) {
        if (is_some()) {
            m_value = std::move(other.unwrap());
        } else if (other.is_some()) {
            m_construct_at(std::addressof(m_value), std::move(other.unwrap()));
        }
        return *this;
    }

    constexpr Option& operator=(const Option& other) {
        if (is_some()) {
            m_value = other.m_value;
        } else if (other.is_some()) {
            m_construct_at(std::addressof(m_value), other.m_value);
        }
        m_has_value = other.m_has_value;
        return *this;
    }

    constexpr Option& operator=(NoneT) noexcept { reset(); }

    constexpr bool operator==(const Some<T>& other) const noexcept {
        return contains(other.get_value());
    }

    constexpr bool operator==(const Option& other) const noexcept {
        if (is_some() && other.is_some())
            return contains(other.unwrap());
        else
            return false;
    }

    constexpr bool operator==(NoneT) const noexcept { return is_none(); }

    constexpr bool contains(const T& value) const noexcept {
        if (is_some())
            return m_value == value;
        else
            return false;
    }
    template <class... Args>
    constexpr auto emplace(Args&&... args)
        -> std::enable_if_t<std::is_constructible_v<T, Args...>> {
        if (is_some()) std::destroy_at(std::addressof(m_value));
        m_construct_at(std::addressof(m_value), args...);
    }

    constexpr void reset() noexcept {
        if (is_some()) {
            std::destroy_at(std::addressof(m_value));
            m_has_value = false;
        }
    }

    template <class U>
    constexpr Option& replace(U&& value) noexcept(
        std::is_nothrow_move_assignable_v<T, U>) {
        if (is_some())
            m_value = std::forward(value);
        else
            m_construct_at(std::addressof(m_value), std::forward(m_value));
        return *this;
    }

    template <class U>
    constexpr Option<U> and_(const Option<U>& opt) {
        if (is_some()) return opt;
        return None;
    }

    template <class F>
    constexpr auto and_then(F&& func) const
        noexcept(std::is_nothrow_invocable_v<F, T>)
            -> std::invoke_result_t<F, T> {
        if (is_some()) return std::forward<F>(func)(unwrap());
        return None;
    }

    constexpr Option<T> or_(const Option<T>& opt) const& noexcept {
        if (is_some())
            return *this;
        else
            return opt;
    }

    template <class F>
    constexpr Option<T> or_else(F&& func) const& noexcept(
        std::is_nothrow_invocable_v<F>) {
        if (is_some())
            return *this;
        else
            return func();
    }
    constexpr Option<T>&& or_(const Option<T>& opt) && noexcept {
        if (is_some())
            return std::move(*this);
        else
            return opt;
    }

    template <class F>
    constexpr Option<T> or_else(F&& func) && noexcept(
        std::is_nothrow_invocable_v<F>) {
        if (is_some())
            return std::move(*this);
        else
            return func();
    }

    ~Option() noexcept(std::is_nothrow_destructible_v<T>) {
        if (is_some()) std::destroy_at(std::addressof(m_value));
    }

   private:
    union {
        T m_value;
        char empty_;
    };
    bool m_has_value;
    template <class... Args>
    constexpr T* m_construct_at(T* p, Args&&... args) {
        return ::new (const_cast<void*>(static_cast<const volatile void*>(p)))
            T(std::forward<Args>(args)...);
    }
};
}  // namespace ilib
#endif
