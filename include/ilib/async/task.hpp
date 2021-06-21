#ifndef GH894884848483_J36473939949_K8484884949_C4747484884_V74848484
#define GH894884848483_J36473939949_K8484884949_C4747484884_V74848484
#include <exception>
#include <type_traits>
#include <utility>

#include "includes.hpp"
namespace ilib {
template <class>
struct task;

template <class T>
struct task_promise {
    task_promise() noexcept {}
    ~task_promise() noexcept {
        value.~T();
    }

    auto initial_suspend() {
        return coro::suspend_always{};
    }

    task<T> get_return_object() noexcept;

    auto final_suspend() noexcept {
        struct awaiter {
            bool await_ready() noexcept {
                return false;
            }
            coro::coroutine_handle<> await_suspend(coro::coroutine_handle<task_promise> ch) noexcept {
                if (ch.promise().cont) return ch.promise().cont;
                return coro::noop_coroutine();
            }
            void await_resume() noexcept {}
        };
        return awaiter{};
    }

    void unhandled_exception() {
        throw;
    }

    template <class U>
    auto return_value(U&& arg) -> std::enable_if_t<std::is_convertible<U, T>::value, void> {
        new (static_cast<void*>(std::addressof(value))) T(std::forward<U>(arg));
    }

    const T& get() noexcept {
        return value;
    }

   private:
    friend task<T>;
    coro::coroutine_handle<> cont;
    union {
        T value;
    };
};
template <>
struct task_promise<void> {
    task_promise() noexcept {}
    ~task_promise() noexcept {}

    auto initial_suspend() {
        return coro::suspend_always{};
    }

    task<void> get_return_object() noexcept;

    auto final_suspend() noexcept {
        struct awaiter {
            bool await_ready() noexcept {
                return false;
            }
            coro::coroutine_handle<> await_suspend(coro::coroutine_handle<task_promise> ch) noexcept {
                if (ch.promise().cont) return ch.promise().cont;
                return coro::noop_coroutine();
            }
            void await_resume() noexcept {}
        };
        return awaiter{};
    }

    void unhandled_exception() {
        throw;
    }

    void return_void() {
        return;
    }

    void get() noexcept {}

   private:
    friend task<void>;
    coro::coroutine_handle<> cont;
};

template <class T>
struct task {
    using promise_type = task_promise<T>;
    using handle_t = coro::coroutine_handle<promise_type>;

    explicit task(handle_t h) : coro_{h} {}
    task(task&& t) noexcept : coro_{std::exchange(t.coro_, {})} {}

    auto operator co_await() && noexcept {
        struct awaiter {
            explicit awaiter(handle_t c) : coro_{c} {}
            bool await_ready() noexcept {
                return false;
            }
            auto await_suspend(coro::coroutine_handle<void> ch) noexcept {
                coro_.promise().cont = ch;
                return coro_;
            }
            T await_resume() noexcept {
                return coro_.promise().get();
            }

           private:
            handle_t coro_;
        };
        return awaiter{coro_};
    }

    ~task() {
        if (coro_) coro_.destroy();
    }

   private:
    handle_t coro_;
};
template <>
struct task<void> {
    using promise_type = task_promise<void>;
    using handle_t = coro::coroutine_handle<promise_type>;

    explicit task(handle_t h) : coro_{h} {}
    task(task&& t) noexcept : coro_{std::exchange(t.coro_, {})} {}

    auto operator co_await() && noexcept {
        struct awaiter {
            explicit awaiter(handle_t c) : coro_{c} {}
            bool await_ready() noexcept {
                return false;
            }
            auto await_suspend(coro::coroutine_handle<void> ch) noexcept {
                coro_.promise().cont = ch;
                return coro_;
            }
            void await_resume() noexcept {
                return;
            }

           private:
            handle_t coro_;
        };
        return awaiter{coro_};
    }

    ~task() {
        if (coro_) coro_.destroy();
    }

   private:
    handle_t coro_;
};

template <class T>
task<T> task_promise<T>::get_return_object() noexcept {
    return task<T>{coro::coroutine_handle<task_promise<T>>::from_promise(*this)};
}

task<void> task_promise<void>::get_return_object() noexcept {
    return task<void>{coro::coroutine_handle<task_promise<void>>::from_promise(*this)};
}
}  // namespace ilib
#endif
