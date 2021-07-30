#ifndef O739200283838920020_P8372902938373882929293883_A73920020393993_V7382920020303
#define O739200283838920020_P8372902938373882929293883_A73920020393993_V7382920020303
#include "utility.hpp"
#include <string>
#include <string_view>
#include <vector>
namespace ilib {
/*
 * use ilib::pretty for getting string value
 * std::forware_as_tuple with std::get to get argument at a specific index: `ilib::format("{0}, {1}, {2}", 1, 2, 3);
 * goal and expected behaviour:
 * ilib::static_vector<int> thing{1, 8, 3, 9, 2, 6, 0};
 * std::string out = ilib::format("{}", thing);
 * > "[1,8,3,9,2,6,0]"
 * std::string out = ilib::format("{..}", ilib::range(0, 100));
 * std::string out = ilib::format("{'a':^30}
 * std::string out = ilib::format("{0} is:\n{0:#x} in hex.\n{0;#o} in octal\n{0:#b} in binary");
 */

namespace detail {
constexpr bool is_double_op(std::string_view sv, std::size_t pos = 0) {
    return sv.at(pos) == '{' && sv.at(pos + 1) == '{';
}
constexpr bool is_double_en(std::string_view sv, std::size_t pos = 0) {
    return sv.at(pos) == '}' && sv.at(pos + 1) == '}';
}
constexpr int parse_digit(std::string_view sv, std::size_t pos = 0) {
    std::size_t terminator = sv.find('}');
    std::string_view sub = sv.substr(pos, terminator);
    int out = 0;
    ILIB_ASSERT(sub.front() >= '0' && sub.front() <= '9', "invalid format specifier");
    for (std::size_t i = 0; i < sub.size(); ++i) {
        out *= 10;
        out += sub.at(i) - '0';
    }
    return out;
}
template <class... T> std::string format__(std::string str, std::vector<std::string> args) {
    std::size_t start_pos = str.find('{');
    if (start_pos == std::string::npos) {
        return str;
    }
    std::size_t end_pos = str.find('}');
    if (end_pos == std::string::npos)
        return str;
    std::vector<std::string> arguments{ilib::pretty(args)...};
    if (str.at(start_pos + 1) == '}') {
        static int index = 0;
        ++index;
        return format__
    } else {
        int index = detail::parse_digit(str, start_pos + 1);
        std::string arg = arguments.at(index);
        str.replace(start_pos, end_pos, arg);
    }
}
} // namespace detail
template <class... T> std::string format__(std::string str, T... args) {
    std::size_t start_pos = str.find('{');
    if (start_pos == std::string::npos) {
        return str;
    }
    std::size_t end_pos = str.find('}');
    if (end_pos == std::string::npos)
        return str;
    std::vector<std::string> arguments{ilib::pretty(args)...};
    if (str.at(start_pos + 1) == '}') {
        static int index = 0;
        ++index;
    } else {
        int index = detail::parse_digit(str, start_pos + 1);
        std::string arg = arguments.at(index);
        str.replace(start_pos, end_pos, arg);
    }
}
} // namespace ilib
#endif
