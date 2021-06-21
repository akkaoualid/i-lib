#include <ilib/static_vector.hpp>
#include <ilib/utility.hpp>
#include <iostream>

int main() {
    ilib::static_vector<int, 20> vec{17, 6, 3, 8, 0, 62, 69};
    std::cout << "elements before: ";
    std::cout << ilib::pretty(vec);
    vec.push_back(19);
    vec.push_back(30);
    vec.push_back(12);
    vec.pop_back();
    std::cout << "\nelements after pushing and poping: ";
    std::cout << ilib::pretty(vec);
}
