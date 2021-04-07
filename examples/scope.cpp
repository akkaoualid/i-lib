#include <ilib/utility/scope.hpp>
#include <iostream>
#include <string>

int main() {
    std::string str{"hello\n"};
    auto fail = [&] { std::cout << str; };
    auto ext = [&] { std::cout << str; };
    auto suc = [&] { std::cout << str; };

    ilib::scope_exit fe{ext};
    ilib::scope_success fs{suc};
    ilib::scope_fail ff{fail};
}
