#include <ilib/async/executor.hpp>
#include <ilib/async/sync_wait.hpp>
#include <ilib/async/task.hpp>
#include <iostream>
int sync_func() {
    return 1;
}
ilib::task<void> amain() {
    std::cout << co_await ilib::async_run(sync_func);
}
int main() {
    ilib::sync_wait(amain());
}
