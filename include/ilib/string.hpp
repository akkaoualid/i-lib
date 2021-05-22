#ifndef H738930302938838838_D63783930_C7389202038488383_V63839030393993
#define H738930302938838838_D63783930_C7389202038488383_V63839030393993
#include <string>
#include <string_view>
#include <vector>
namespace ilib {
template <class Container>
constexpr Container split(std::string_view sv, std::string_view delim) {
    Container cnt;
    std::size_t pos = sv.find(delim);
    std::size_t strt = 0;
    while (pos != std::string_view::npos) {
        cnt.insert(sv.substr(strt, pos - strt));
        strt = pos + delim.size();
        pos = sv.find(delim, strt);
    }
    cnt.insert(sv.substr(strt, pos - strt));
    return cnt;
}
template <class Iter>
constexpr std::string join(Iter iter, std::string_view delim) {
    std::string s;
    for (const auto& elm : iter) (s += elm) += delim;
    s.pop_back();  // remove the additional delim
    return s;
}
template <class Iter>
constexpr std::string join(Iter start, Iter end, std::string_view delim) {
    std::string s;
    for (; start != end; ++start) (s += *start) += delim;
    s.pop_back();  // remove the additional delim
    return s;
}
}  // namespace ilib
#endif
