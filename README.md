# i-lib
A custom library made for fun
[![c++](https://img.shields.io/badge/-C++20-00599C?style=flat-square&logo=c)]

## features:
### coroutines:
defined in header i-lib/async
#### generator:
```cpp
#include <iostream>
#include <ilib/async/generator.hpp>

ilib::generator<int> sequence(int start) {
    for(;start--;)
        co_yield start;
}
```

#### task:

```cpp
#include <iostream>
#include <string_view>
#include <ilib/async/task.hpp>

ilib::task<std::string_view> get_sth() {
    co_return "hello";
}

ilib::task<void> do_sth() {
    co_await get_sth();
}
```


#### sync_wait:

```cpp
#include <iostream>
#include <string_view>
#include <ilib/async/task.hpp>
#include <ilib/async/sync_wait.hpp>

ilib::task<std::string_view> get_sth() {
    co_return "hello";
}

ilib::task<void> do_sth() {
    co_await get_sth();
}
int main() {
    ilib::sync_wait(do_sth());
}
```


#### async_executor:
wrap a sync function in a future and make it awaitable

```cpp
#include <ilib/async/executor.hpp>
void sync_func() {
    // do stuff
}
ilib::task<void> async_func() {
    auto async_obj = ilib::async_executor(sync_func);
    co_await async_obj();
}
```


### iterators:
#### enumerator:
```cpp
#include <iostream>
#include <vector>
#include <ilib/iterators.hpp>

int main() {
    std::vector<int> seq{7, 8, 3, 6, 8};
    for(ilib::enumerator range{seq.begin(), seq.end()}; const auto& [idx, val] : range)
        std::cout << "index: " << idx << ", value: " << value << '\n';
}
```
