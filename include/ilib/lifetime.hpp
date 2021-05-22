#ifndef H7393947378393_G949383838383_X7393948392902938383
#define H7393947378393_G949383838383_X7393948392902938383
#include <type_traits>
#include <utility>
namespace ilib {
struct in_place_t {
    constexpr in_place_t(int) {}
};
constexpr inline in_place_t in_place{0};
template <class T>
struct lifetime {
    using value_type = T;

    constexpr lifetime() noexcept {}
    ~lifetime() noexcept {}
    template <class U>
    constexpr lifetime(U&& arg) noexcept(std::is_nothrow_constructible_v<T, decltype(std::forward<U>(arg))>) {
        ::new (static_cast<void*>(std::addressof(value_))) T(std::forward<U>(arg));
    }

    template <class... U>
    constexpr lifetime(in_place_t,
                       U&&... args) noexcept(std::is_nothrow_constructible_v<T, decltype(std::forward<U>(args))...>) {
        ::new (static_cast<void*>(std::addressof(value_))) T(std::forward<U>(args)...);
    }

    template <class U>
    constexpr void construct(U&& arg) noexcept(std::is_nothrow_constructible_v<T, decltype(std::forward<U>(arg))>) {
        ::new (static_cast<void*>(std::addressof(value_))) T(std::forward<U>(arg));
    }

    template <class... U>
    constexpr void construct(in_place_t, U&&... args) noexcept(
        std::is_nothrow_constructible_v<T, decltype(std::forward<U>(args))...>) {
        ::new (static_cast<void*>(std::addressof(value_))) T(std::forward<U>(args)...);
    }

    void destruct() {
        value_.~T();
    }

    constexpr const T& get() const& noexcept {
        return value_;
    }

    constexpr T& get() & noexcept {
        return value_;
    }

    constexpr T&& get() && noexcept {
        return std::move(value_);
    }

    constexpr operator const T&() const& noexcept {
        return value_;
    }

    constexpr operator T&() & noexcept {
        return value_;
    }

    constexpr operator T&&() && noexcept {
        return std::move(value_);
    }

   private:
    union {
        T value_;
        char* dummy;
    };
};
template <>
struct lifetime<void> {
    constexpr lifetime() {}
    ~lifetime() {}
    constexpr void construct() {}
    void destruct() {}
    constexpr void get() {}
};
}  // namespace ilib
#endif
