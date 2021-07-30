#include <ilib/iterators.hpp>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec{1, 6, 6, 89, 3, 3};
    std::vector<int> vec2{1, 5, 7, 8, 6, 9};
    for (const auto& [x, y] : ilib::zipper(vec, vec2)) {
        std::cout << x << ' ' << y << '\n';
    }
}
