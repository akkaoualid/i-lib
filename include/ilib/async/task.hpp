#ifndef GH894884848483_J36473939949_K8484884949_C4747484884_V74848484
#define GH894884848483_J36473939949_K8484884949_C4747484884_V74848484
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
#include <type_traits>
#include <utility>
namespace ilib {
template <class>
struct task;

template <class T>
struct task_promise {
    task_promise() noexcept {}
    ~task_promise() noexcept { value.~T(); }

    auto initial_suspend() { return std::suspend_always{}; }

    task<T> get_return_object() noexcept;

    auto final_suspend() noexcept {
        struct awaiter {
            bool await_ready() noexcept { return false; }
            auto await_suspend(
                std::coroutine_handle<task_promise> ch) noexcept {
                return ch.promise().cont;
            }
            void await_resume() noexcept {}
        };
        return awaiter{};
    }

    void unhandled_exception() noexcept { error = std::current_exception(); }

    template <class U>
    auto return_value(U&& arg)
        -> std::enable_if_t<std::is_convertible<U, T>::value, void> {
        new (static_cast<void*>(std::addressof(value))) T(std::forward<U>(arg));
    }

    T get() {
        if (error)
            std::rethrow_exception(error);
        else
            return value;
    }

   private:
    friend task<T>;
    std::exception_ptr error;
    std::coroutine_handle<void> cont;
    union {
        T value;
    };
};
template <>
struct task_promise<void> {
    task_promise() noexcept {}
    ~task_promise() noexcept {}

    auto initial_suspend() { return std::suspend_always{}; }

    task<void> get_return_object() noexcept;

    auto final_suspend() noexcept {
        struct awaiter {
            bool await_ready() noexcept { return false; }
            auto await_suspend(
                std::coroutine_handle<task_promise> ch) noexcept {
                return ch.promise().cont;
            }
            void await_resume() noexcept {}
        };
        return awaiter{};
    }

    void unhandled_exception() noexcept { error = std::current_exception(); }

    void return_void() { return; }

    void get() {
        if (error)
            std::rethrow_exception(error);
        else
            return;
    }

   private:
    friend task<void>;
    std::coroutine_handle<void> cont;
    std::exception_ptr error;
};

template <class T>
struct task {
    using promise_type = task_promise<T>;
    using handle_t = std::coroutine_handle<promise_type>;

    explicit task(handle_t h) : coro{h} {}
    task(task&& t) noexcept : coro{std::exchange(t.coro, {})} {}

    auto operator co_await() && noexcept {
        struct awaiter {
            explicit awaiter(handle_t c) : coro_{c} {}
            bool await_ready() noexcept { return false; }
            auto await_suspend(std::coroutine_handle<void> ch) noexcept {
                coro_.promise().cont = ch;
                return coro_;
            }
            T await_resume() noexcept { return coro_.promise().get(); }

           private:
            handle_t coro_;
        };
        return awaiter{coro};
    }

    ~task() {
        if (coro) coro.destroy();
    }

   private:
    handle_t coro;
};
template <>
struct task<void> {
    using promise_type = task_promise<void>;
    using handle_t = std::coroutine_handle<promise_type>;

    explicit task(handle_t h) : coro{h} {}
    task(task&& t) noexcept : coro{std::exchange(t.coro, {})} {}

    auto operator co_await() && noexcept {
        struct awaiter {
            explicit awaiter(handle_t c) : coro_{c} {}
            bool await_ready() noexcept { return false; }
            auto await_suspend(std::coroutine_handle<void> ch) noexcept {
                coro_.promise().cont = ch;
                return coro_;
            }
            void await_resume() noexcept { return; }

           private:
            handle_t coro_;
        };
        return awaiter{coro};
    }

    ~task() {
        if (coro) coro.destroy();
    }

   private:
    handle_t coro;
};

template <class T>
task<T> task_promise<T>::get_return_object() noexcept {
    return task<T>{std::coroutine_handle<task_promise<T>>::from_promise(*this)};
}

task<void> task_promise<void>::get_return_object() noexcept {
    return task<void>{
        std::coroutine_handle<task_promise<void>>::from_promise(*this)};
}
}  // namespace ilib
#endif
