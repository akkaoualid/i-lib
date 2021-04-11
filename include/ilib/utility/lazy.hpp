#ifndef G37379393900_F738393903030_A738393020200
#define G37379393900_F738393903030_A738393020200
#include <functional>

#include "option.hpp"
#include "traits.hpp"
#include "utility.hpp"
namespace ilib {
template <class...>
struct lazy;
template <class T, class... U>
struct lazy<T(U...)> {
    lazy() noexcept : func{nullptr} {}
    /*! \brief constructs with a callable.*/
    template <class C>
    lazy(C&& callable) : func{ilib::forward<C>(callable)} {}

    ~lazy() noexcept { clear(); }

    /*! \brief returns true if the 'lazy' containts a function.*/
    operator bool() noexcept { return static_cast<bool>(func); }

    lazy& operator=(decltype(nullptr)) noexcept {
        func = nullptr;
        return *this;
    }

    /*! \brief assign by moving the operand function.*/
    lazy& operator=(const lazy& rhs) {
        func = ilib::move(rhs.func);
        last = ilib::forward<T>(rhs.last);
        return *this;
    }

    /*! \brief calls the stored function immediatly without checking for cached
     * returns.*/
    template <class... P>
    auto operator()(P&&... args) {
        return force_call(ilib::forward<P>(args)...);
    }

    /*! \brief calls the stored function if there is no cached return.*/
    template <class... P>
    T call(P&&... args) {
        if (last) {
            return last;
        } else {
            last = force_call(ilib::forward<P>(args)...);
            return last;
        }
    }

    /*! \brief calls the stored function immediatly without checking for cached
     * returns.*/
    template <class... P>
    auto force_call(P&&... args)
        -> ilib::enable_if_t<(ilib::is_same_v<P, U> && ...), T> {
        return func(ilib::forward<P>(args)...);
    }

    constexpr bool is_cached() noexcept { return last.is_some(); }

    constexpr void clear() noexcept {
        func = nullptr;
        last = ilib::None;
    }

    constexpr void clear_cache() noexcept { last = ilib::None; }

   private:
    T func{};
    ilib::Option<T> last{};
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
