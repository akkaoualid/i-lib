#ifdef Y6383903038392083_Z526290393838838_Z63839392003939
#define Y6383903038392083_Z526290393838838_Z63839392003939
#if __has_include(<coroutine>)
using namespace coro {
    using std::coroutine_handle;
    using std::noop_coroutine;
    using std::suspend_always;
    using std::suspend_never;
}
#elif __has_include(<experimental/coroutine>)
using namespace coro {
    using std::experimental::coroutine_handle;
    using std::experimental::noop_coroutine;
    using std::experimental::suspend_always;
    using std::experimental::suspend_never;
}
#else
#error "cannot find coroutine header"

#endif
