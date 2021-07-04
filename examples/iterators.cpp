#include <ilib/iterators.hpp>
#include <iostream>

int main() {
    ilib::iota rng{ilib::_, ilib::_};
    for (std::int64_t i : rng) {
        if (i > 10'000)
            break;
        std::cout << i << ' ';
    }
}
