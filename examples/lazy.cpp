#include <ilib/utility/lazy.hpp>
#include <iostream>

void get_sth() {
    std::cout << "get_sth\n";
}

int main() {
    ilib::lazy<void()> l{get_sth};
    std::cout << "lazy function isn't evaluated yet\n";
    l();  // l.force_call()
    std::cout << "lazy function is evaluated\n";
}
