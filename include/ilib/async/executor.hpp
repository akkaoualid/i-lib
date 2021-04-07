#ifndef F_47489494999_H748493030303
#define F_47489494999_H748493030303
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
#include <ilib/async/task.hpp>
#include <ilib/utility/utility.hpp>
#include <mutex>
#include <thread>
#include <type_traits>
namespace ilib {
template <class FT>
struct async_executor {
    async_executor(const FT& f) : func{f} {}

    using result_type = std::invoke_result_t<FT>;

    bool await_ready() { return false; }

    bool await_suspend(std::coroutine_handle<void> c) {
        coro = c;
        ft = std::async{std::launch::async, [&, this] {
                            coro.resume();
                            return func();
                        }};
        return true;
    }

    std::conditional_t<std::is_same_v<result_type, void>, void, result_type>
    await_resume() {
        if constexpr (std::is_same_v<result_type, void>)
            return;
        else
            return ilib::move(ft.get());
    }

   private:
    FT func;
    std::future<result_type> ft;
    std::coroutine_handle<void> coro;
    std::mutex mtx;
};
template <class F>
async_executor(F) -> async_executor<F>;

template <class F>
ilib::task<std::invoke_result_t<F>> async_exec(F&& fnc) {
    co_return co_await async_executor{fnc};
}
}  // namespace ilib
#endif
