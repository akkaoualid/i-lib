#include <ilib/async/sync_wait.hpp>
#include <ilib/async/task.hpp>
#include <iostream>
#include <string_view>

static ilib::task<std::string_view> get_sth() { co_return "hello"; }

static ilib::task<void> print_sth() {
    auto out = co_await get_sth();
    std::cout << out;
}

int main() { ilib::sync_wait(print_sth()); }
