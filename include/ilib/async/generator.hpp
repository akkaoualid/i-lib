#ifndef JPRP47383893_KDJRJRJ_48483883
#define JPRP47383893_KDJRJRJ_48483883
#if __has_include(<coroutine>)
#include <coroutine>
#else
#include <experimental/coroutine>
namespace std {
using std::experimental::coroutine_handle;
using std::experimental::suspend_always;
using std::experimental::suspend_never;
}  // namespace std
#endif
#include <exception>
#include <utility>
namespace ilib {
template <class T>
struct generator {
    struct promise_type {
        T cur_value{};
        auto yield_value(T value) {
            cur_value = value;
            return std::suspend_always{};
        }
        auto initial_suspend() noexcept { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void return_void() noexcept {}
        generator get_return_object() noexcept { return generator{this}; }
        void unhandled_exception() { throw; }
    };

    using handle_t = std::coroutine_handle<promise_type>;
    generator(generator&& gen) noexcept
        : coro(std::exchange(gen.coro, nullptr)) {}
    ~generator() noexcept {
        if (coro) {
            coro.destroy();
        }
    }

    struct iterator {
        bool done{};
        handle_t coro{};
        explicit iterator() : done(true) {}
        iterator(handle_t c_, bool d_) : coro(c_), done(d_) {}
        iterator& operator++() {
            coro.resume();
            done = coro.done();
            return *this;
        }

        bool operator==(const iterator& rhs) const { return done == rhs.done; }
        bool operator!=(const iterator& rhs) const { return done != rhs.done; }

        const T* operator->() const { return std::addressof(*this); }
        const T& operator*() const { return coro.promise().cur_value; }
    };
    iterator begin() {
        coro.resume();
        return {coro, coro.done()};
    }
    iterator end() { return iterator{}; }

   private:
    explicit generator(promise_type* pt) : coro(handle_t::from_promise(*pt)) {}
    handle_t coro{};
};
}  // namespace ilib
#endif
