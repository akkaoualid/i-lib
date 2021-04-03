#include <ilib/utility/scope.hpp>
#include <iostream>

int main() {
    auto fail = [] { std::cout << "fail\n"; };
    auto ext = [] { std::cout << "ext\n"; };
    auto suc = [] { std::cout << "success\n"; };

    ilib::scope_exit fe{ext};
    ilib::scope_success fs{suc};
    ilib::scope_fail ff{fail};
}
