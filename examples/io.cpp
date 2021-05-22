#include <ilib/async/io/io.hpp>
#include <ilib/async/sync_wait.hpp>
#include <ilib/async/task.hpp>

ilib::task<void> c_main() {
    ilib::io::path current = ilib::io::get_current_path();
    ilib::io::print("current directory is: ", current);
}

int main() {
    ilib::sync_wait(c_main());
}
