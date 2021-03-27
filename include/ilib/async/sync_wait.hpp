#ifndef H_4747488489949_ % $DUEUEIEIK_7383839_GEURURIRIIR
#define H_4747488489949_ % $DUEUEIEIK_7383839_GEURURIRIIR
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
#include <condition_variable>
#include <exception>
#include <mutex>
namespace ilib {
struct sync_wait_impl {
    struct promise_type {
        sync_wait_impl get_return_object() {
            return sync_wait_impl{
                std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        std::suspend_always initial_suspend() { return {}; }

        void return_void() noexcept {}
        auto final_suspend() noexcept {
            struct awaiter {
                bool await_ready() noexcept { return false; }
                auto await_suspend(
                    std::coroutine_handle<promise_type> h) noexcept {
                    auto& promise = h.promise();
                    std::lock_guard lk{promise.mtx};
                    promise.done = true;
                    promise.cv.notify_one();
                }
                void await_resume() noexcept {}
            };
            return awaiter{};
        }

        void unhandled_exception() noexcept {
            error = std::current_exception();
        }

        void wait() {
            std::unique_lock lk{mtx};
            while (!done) {
                cv.wait(lk);
            }
            if (error) std::rethrow_exception(error);
        }

       private:
        std::mutex mtx{};
        std::condition_variable cv{};
        std::exception_ptr error{};
        bool done{};
    };
    using handle_t = std::coroutine_handle<promise_type>;
    sync_wait_impl(handle_t c) : coro{c} {}
    sync_wait_impl(sync_wait_impl&& sw) noexcept
        : coro{std::exchange(sw.coro, {})} {}

    void wait() {
        coro.resume();
        coro.promise().wait();
    }

   private:
    handle_t coro;
};

template <class A>
void sync_wait(A&& a) {
    [&]() -> sync_wait_impl { co_await std::move(a); }().wait();
}
}  // namespace ilib
#endif
