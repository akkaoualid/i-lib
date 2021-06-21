#ifndef F_47489494999_H748493030303
#define F_47489494999_H748493030303
#include <ilib/async/task.hpp>
#include <ilib/lifetime.hpp>
#include <ilib/utility.hpp>
#include <mutex>
#include <thread>
#include <type_traits>

#include "includes.hpp"

namespace ilib {
template <class FT, class result_type = std::invoke_result_t<FT>>
struct async_executor {
    async_executor(const FT& f) : func{f} {}
    ~async_executor() {
        value.destruct();
    }

    bool await_ready() {
        return false;
    }

    void await_suspend(std::coroutine_handle<void> c) {
        std::thread([&, this] {
            std::unique_lock l{mtx};
            value.construct(func());
            c();
        }).join();
    }

    auto await_resume() {
        return std::move(value.get());
    }

   private:
    FT func;
    std::mutex mtx;
    ilib::lifetime<result_type> value;
};
template <class FT>
struct async_executor<FT, void> {
    async_executor(const FT& f) : func{f} {}

    bool await_ready() {
        return false;
    }

    void await_suspend(std::coroutine_handle<void> c) {
        std::thread([&, this] {
            std::unique_lock l{mtx};
            func();
            c();
        }).join();
    }

    void await_resume() {}

   private:
    FT func;
    std::mutex mtx;
};
template <class F>
async_executor(F) -> async_executor<F>;

template <class F>
ilib::task<std::invoke_result_t<F>> async_run(F&& fnc) {
    co_return co_await async_executor{std::forward<F>(fnc)};
}
}  // namespace ilib
#endif
