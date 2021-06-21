#include <gtest/gtest.h>

#include <ilib/string.hpp>
#include <string>

TEST(utility, string_manipulation) {
    std::string in{"split,me"};
    auto out = ilib::split(in, ",");
    ASSERT_EQ(out.at(0), "split");
    ASSERT_EQ(out.at(1), "me");
    std::string joined = ilib::join(out, " ");
    ASSERT_EQ(joined, "split me");
    std::string strip_ = ilib::strip(joined, " ");
    ASSERT_EQ(strip_, "splitme");
}

int main(int a, char** v) {
    testing::InitGoogleTest(&a, v);
    RUN_ALL_TESTS();
}
