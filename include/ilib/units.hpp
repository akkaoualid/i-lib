#ifndef H738393903020393882883838_Z739390303039390303030_A739399393839399330
#define H738393903020393882883838_Z739390303039390303030_A739399393839399330
namespace ilib::units {
struct terabyte;
struct gigabyte;
struct megabyte;
struct kilobyte;
struct byte;
#define DEF_UNIT(name)                                   \
    struct name {                                        \
        constexpr name(long double v) : value{v} {}      \
        constexpr terabyte to_terabyte() const noexcept; \
        constexpr gigabyte to_gigabyte() const noexcept; \
        constexpr megabyte to_megabyte() const noexcept; \
        constexpr kilobyte to_kilobyte() const noexcept; \
        constexpr byte to_byte() const noexcept;         \
                                                         \
       private:                                          \
        long double value;                               \
    };

DEF_UNIT(terabyte)
DEF_UNIT(gigabyte)
DEF_UNIT(megabyte)
DEF_UNIT(kilobyte)
DEF_UNIT(byte)

#define DEF_FUNC(ret, name, func, body)         \
    constexpr ret name::func() const noexcept { \
        return body;                            \
    }

// urgh

DEF_FUNC(terabyte, terabyte, to_terabyte, value);
DEF_FUNC(gigabyte, terabyte, to_gigabyte, value * 1000);
DEF_FUNC(megabyte, terabyte, to_megabyte, value * 1000'000);
DEF_FUNC(kilobyte, terabyte, to_kilobyte, value * 1000'000'000);
DEF_FUNC(byte, terabyte, to_byte, value * 1000'000'000'000);

DEF_FUNC(terabyte, gigabyte, to_terabyte, value / 1000);
DEF_FUNC(gigabyte, gigabyte, to_gigabyte, value);
DEF_FUNC(megabyte, gigabyte, to_megabyte, value * 1000);
DEF_FUNC(kilobyte, gigabyte, to_kilobyte, value * 1000'000);
DEF_FUNC(byte, gigabyte, to_byte, value * 1000'000'000);

DEF_FUNC(terabyte, megabyte, to_terabyte, value / 1000'000);
DEF_FUNC(gigabyte, megabyte, to_gigabyte, value / 1000);
DEF_FUNC(megabyte, megabyte, to_megabyte, value);
DEF_FUNC(kilobyte, megabyte, to_kilobyte, value * 1000);
DEF_FUNC(byte, megabyte, to_byte, value * 1000'000);

DEF_FUNC(terabyte, kilobyte, to_terabyte, value / 1000'000'000);
DEF_FUNC(gigabyte, kilobyte, to_gigabyte, value / 1000'000);
DEF_FUNC(megabyte, kilobyte, to_megabyte, value / 1000);
DEF_FUNC(kilobyte, kilobyte, to_kilobyte, value);
DEF_FUNC(byte, kilobyte, to_byte, value * 1000);

DEF_FUNC(terabyte, byte, to_terabyte, value / 1000'000'000'000);
DEF_FUNC(gigabyte, byte, to_gigabyte, value / 1000'000'000);
DEF_FUNC(megabyte, byte, to_megabyte, value / 1000'000);
DEF_FUNC(kilobyte, byte, to_kilobyte, value / 1000);
DEF_FUNC(byte, byte, to_byte, value);

#undef DEF_UNIT
#undef DEF_FUNC
}  // namespace ilib::units
#endif
