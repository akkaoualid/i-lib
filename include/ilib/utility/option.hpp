#ifndef U9393020394873_A8303020372727_L839302003
#define U9393020394873_A8303020372727_L839302003
#include <functional>
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

template <class T>
struct Some {
    using value_type = T;
    constexpr explicit Some(const T& value) noexcept(
        std::is_nothrow_constructible_v<T>)
        : m_value{value} {}

    constexpr explicit Some() noexcept(
        std::is_nothrow_default_constructible_v<T>)
        : m_value{} {}

    constexpr T get_value() const noexcept { return m_value; }

   private:
    T m_value;
};

template <class T>
Some(T) -> Some<T>;

template <class T>
struct Option {
    using value_type = T;

    constexpr Option(NoneT) noexcept : m_has_value{} {}

    constexpr Option(const Some<T>& some) : m_has_value{true} {
        new (static_cast<void*>(std::addressof(m_value))) T{some.get_value()};
    }

    constexpr Option(const Option& other)
        : m_has_value{other.m_has_value}, m_value{other.m_value} {}

    constexpr bool is_none() const noexcept { return !m_has_value; }

    constexpr bool is_some() const noexcept { return m_has_value; }

    constexpr T expect(std::string_view msg) const {
        if (is_some()) {
            return m_value;
        } else {
            throw detail::opt_exception(msg);
        }
    }

    constexpr T unwrap() const {
        if (is_some()) {
            return m_value;
        } else {
            throw detail::opt_exception("invalid Option access");
        }
    }

    constexpr T unwrap_or(const T& other) const noexcept {
        if (is_some()) {
            return m_value;
        } else {
            return other;
        }
    }

    constexpr bool operator==(const T& value) const noexcept {
        if (is_some()) return m_value == value;
        return false;
    }

    constexpr auto as_ref() noexcept -> Option<std::reference_wrapper<T>> {
        if (is_some())
            return Some(std::ref(m_value));
        else
            return None;
    }

    constexpr auto as_cref() noexcept -> Option<std::reference_wrapper<T>> {
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

    constexpr T operator*() const noexcept(m_has_value) { return unwrap(); }

    constexpr void swap(Option&& other) noexcept {
        using std::swap;
        swap(m_has_value, other.m_has_value);
        if (is_some())
            swap(m_value, other.m_has_value);
        else
            new (static_cast<void*>(std::addressof(m_value)))
                T{std::move(other.m_value)};
    }

    constexpr Option& operator=(Option&& other) noexcept {
        swap(std::move(other));
        return *this;
    }

    constexpr Option& operator=(const Option& other) {
        m_has_value = other.m_has_value;
        if (is_some())
            m_value = other.m_value;
        else
            new (static_cast<void*>(std::addressof(m_value))) T{other.m_value};
        return *this;
    }

    constexpr bool operator==(const Some<T>& other) const noexcept {
        return contains(other.get_value());
    }

    constexpr bool operator==(NoneT) const noexcept { return false; }

    constexpr bool contains(const T& value) const noexcept {
        if (is_some())
            return m_value == value;
        else
            return false;
    }

    constexpr Option& replace(const T& value) noexcept(
        std::is_nothrow_copy_assignable_v<T>) {
        if (is_some())
            m_value = value;
        else
            new (static_cast<void*>(std::addressof(m_value))) T{value};
        return *this;
    }

    constexpr Option& replace(T&& value) noexcept(
        std::is_nothrow_move_assignable_v<T>) {
        if (is_some())
            m_value = std::move(value);
        else
            new (static_cast<void*>(std::addressof(m_value)))
                T{std::move(value)};
        return *this;
    }

   private:
    union {
        T m_value;
    };
    bool m_has_value;
};
}  // namespace ilib
#endif
