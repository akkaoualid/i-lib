#include <ilib/static_string.hpp>
#include <iostream>

int main() {
    ilib::static_string<20> str{"there"};
    std::cout << str << '\n';
    str.insert(0, "hello ");
    str.insert(str.size(), " bruh");
    std::cout << str << '\n';
    ilib::static_string<50> str2(5, 'a');
    str2.insert(str2.size(), 5, 'b');
    str2.insert(5, str2, 3);
    str2.replace(0, 5, "hello");
    std::cout << str2 << '\n';
    ilib::static_string str3{"hello world"};
    std::cout << str3 << '\n';
}
