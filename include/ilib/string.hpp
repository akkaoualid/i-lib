#ifndef H738930302938838838_D63783930_C7389202038488383_V63839030393993
#define H738930302938838838_D63783930_C7389202038488383_V63839030393993
#include <cctype>
#include <string>
#include <string_view>
#include <vector>
namespace ilib {
template <class Container = std::vector<std::string_view>>
Container split(std::string_view sv, std::string_view delim) noexcept {
    Container cnt;
    std::size_t pos = sv.find(delim);
    std::size_t strt = 0;
    while (pos != std::string_view::npos) {
        cnt.insert(cnt.end(), sv.substr(strt, pos - strt));
        strt = pos + delim.size();
        pos = sv.find(delim, strt);
    }
    cnt.insert(cnt.end(), sv.substr(strt, pos - strt));
    return cnt;
}
std::string strip(std::string_view sv, std::string_view delim) noexcept {
    std::string cnt;
    std::size_t pos = sv.find(delim);
    std::size_t strt = 0;
    while (pos != std::string_view::npos) {
        cnt += sv.substr(strt, pos - strt);
        strt = pos + delim.size();
        pos = sv.find(delim, strt);
    }
    cnt += sv.substr(strt, pos - strt);
    return cnt;
}
template <class Iter>
std::string join(Iter iter, std::string_view delim) {
    std::string s;
    for (const auto& elm : iter) (s += elm) += delim;
    s.pop_back();  // remove the additional delim
    return s;
}
template <class Iter>
std::string join(Iter start, Iter end, std::string_view delim) noexcept {
    std::string s;
    for (; start != end; ++start) (s += *start) += delim;
    s.pop_back();  // remove the additional delim
    return s;
}
std::string lowercase(std::string_view sv) noexcept {
    std::string out;
    for (unsigned char c : sv) out += std::tolower(c);
    return out;
}
std::string uppercase(std::string_view sv) noexcept {
    std::string out;
    for (unsigned char c : sv) out += std::toupper(c);
    return out;
}

}  // namespace ilib
#endif
