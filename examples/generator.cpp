#include <ilib/async/generator.hpp>
#include <iostream>

ilib::generator<int> fib() {
    int a = 0;
    int b = 1;
    while (true) {
        co_yield b;
        int t = a;
        a = b;
        b += t;
    }
}

int main() {
    for (auto i : fib()) {
        if (i >= 1'000) break;
        std::cout << "i: " << i << '\n';
    }
}
