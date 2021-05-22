#ifndef O739200283838920020_P8372902938373882929293883_A73920020393993_V7382920020303
#define O739200283838920020_P8372902938373882929293883_A73920020393993_V7382920020303
#include <string>
namespace ilib {
template <class>
struct formatter;
template <>
struct formatter<int> {
    std::string format(int value) { return std::to_string(value); }
};

}  // namespace ilib
#endif
