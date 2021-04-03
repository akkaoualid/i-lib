#ifndef G37379393900_F738393903030_A738393020200
#define G37379393900_F738393903030_A738393020200
#include <functional>
#include <optional>

#include "traits.hpp"
#include "utility.hpp"
namespace ilib {
template <class T, class... U>
struct lazy;

template <class T, class... U>
struct lazy<T(U...)> {
    lazy() noexcept : func{nullptr} {}

    template <class C>
    lazy(C&& callable) : func{ilib::forward<C>(callable)} {}

    ~lazy() noexcept { clear(); }

    operator bool() noexcept { return static_cast<bool>(func); }

    lazy& operator=(decltype(nullptr)) noexcept {
        func = nullptr;
        return *this;
    }

    lazy& operator=(const lazy& rhs) {
        func = ilib::move(rhs.func);
        last = rhs.last;
        return *this;
    }

    template <class... P>
    auto operator()(P&&... args) {
        return force_call(ilib::forward<P>(args)...);
    }

    template <class... P>
    T call(P&&... args) {
        if (last) {
            return last;
        } else {
            last = force_call(ilib::forward<P>(args)...);
            return last;
        }
    }

    template <class... P>
    auto force_call(P&&... args)
        -> ilib::enable_if_t<(ilib::is_same_v<P, U> && ...), T> {
        return func(ilib::forward<P>(args)...);
    }

    constexpr bool is_cached() noexcept { return last.has_value(); }

    constexpr void clear() noexcept {
        func = nullptr;
        last = std::nullopt;
    }

    constexpr void clear_cache() noexcept { last = std::nullopt; }

   private:
    std::function<T(U...)> func{};
    std::optional<T> last{};
};
template <class... U>
struct lazy<void(U...)> {
    lazy() noexcept : func{nullptr} {}

    template <class C>
    lazy(C&& callable) : func{ilib::forward<C>(callable)} {}

    operator bool() noexcept { return static_cast<bool>(func); }

    lazy& operator=(decltype(nullptr)) noexcept {
        func = nullptr;
        return *this;
    }

    lazy& operator=(const lazy& rhs) {
        func = ilib::move(rhs.func);
        return *this;
    }

    template <class... P>
    void operator()(P&&... args) {
        call(ilib::forward<P>(args)...);
    }

    template <class... P>
    auto call(P&&... args)
        -> ilib::enable_if_t<(ilib::is_same_v<P, U> && ...)> {
        func(ilib::forward<P>(args)...);
    }

   private:
    std::function<void(U...)> func{};
};
}  // namespace ilib
#endif