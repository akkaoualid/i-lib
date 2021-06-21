#ifndef JPRP47383893_KDJRJRJ_48483883
#define JPRP47383893_KDJRJRJ_48483883
#include <exception>
#include <ilib/lifetime.hpp>
#include <utility>

#include "includes.hpp"
namespace ilib {
template <class T>
struct generator {
    struct promise_type {
        ilib::lifetime<T> cur_value;
        auto yield_value(T value) {
            cur_value.construct(value);
            return coro::suspend_always{};
        }
        auto initial_suspend() noexcept {
            return coro::suspend_always{};
        }
        auto final_suspend() noexcept {
            return coro::suspend_always{};
        }
        void return_void() noexcept {}

        generator get_return_object() noexcept {
            return generator{this};
        }
        void unhandled_exception() {
            throw;
        }

        ~promise_type() {
            cur_value.destruct();
        }
    };

    using handle_t = coro::coroutine_handle<promise_type>;
    generator(generator&& gen) noexcept : coro_{std::exchange(gen.coro_, nullptr)} {}
    ~generator() noexcept {
        if (coro_) { coro_.destroy(); }
    }

    struct iterator {
        bool done{};
        handle_t coro_{};
        explicit iterator() : done(true) {}
        iterator(handle_t c_, bool d_) : coro_(c_), done(d_) {}
        iterator& operator++() {
            coro_.resume();
            done = coro_.done();
            return *this;
        }

        bool operator==(const iterator& rhs) const {
            return done == rhs.done;
        }
        bool operator!=(const iterator& rhs) const {
            return done != rhs.done;
        }

        const T* operator->() const {
            return std::addressof(*this);
        }
        const T& operator*() const {
            return coro_.promise().cur_value.get();
        }
    };
    iterator begin() {
        coro_.resume();
        return {coro_, coro_.done()};
    }
    iterator end() {
        return iterator{};
    }

   private:
    explicit generator(promise_type* pt) : coro_(handle_t::from_promise(*pt)) {}
    handle_t coro_{};
};
}  // namespace ilib
#endif
