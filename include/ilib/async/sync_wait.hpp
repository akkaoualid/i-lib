#ifndef H_4747488489949_DUEUEIEIK_7383839_GEURURIRIIR
#define H_4747488489949_DUEUEIEIK_7383839_GEURURIRIIR
#include <condition_variable>
#include <exception>
#include <mutex>

#include "includes.hpp"
namespace ilib {
struct sync_wait_impl {
    struct promise_type {
        sync_wait_impl get_return_object() {
            return sync_wait_impl{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        coro::suspend_always initial_suspend() {
            return {};
        }

        void return_void() noexcept {}
        auto final_suspend() noexcept {
            struct awaiter {
                bool await_ready() noexcept {
                    return false;
                }
                auto await_suspend(std::coroutine_handle<promise_type> h) noexcept {
                    auto& promise = h.promise();
                    std::lock_guard lk{promise.mtx};
                    promise.done = true;
                    promise.cv.notify_one();
                }
                void await_resume() noexcept {}
            };
            return awaiter{};
        }

        void unhandled_exception() {
            throw;
        }

        void wait() {
            coro::unique_lock lk{mtx};
            while (!done) { cv.wait(lk); }
        }

       private:
        std::mutex mtx{};
        std::condition_variable cv{};
        bool done = false;
    };
    using handle_t = std::coroutine_handle<promise_type>;

    sync_wait_impl(handle_t c) : coro_{c} {}

    sync_wait_impl(sync_wait_impl&& sw) noexcept : coro_{std::exchange(sw.coro_, {})} {}

    void wait() {
        coro_.resume();
        coro_.promise().wait();
    }

   private:
    handle_t coro_;
};

template <class A>
void sync_wait(A&& a) {
    [&]() -> sync_wait_impl { co_await std::move(a); }().wait();
}
}  // namespace ilib
#endif
