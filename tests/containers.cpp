#include <gtest/gtest.h>

#include <ilib/static_string.hpp>
#include <ilib/static_vector.hpp>

namespace ns {
int x = 1;
}
TEST(static_vector, initialization) {
    struct A {
        A() { ++ns::x; }
    };
    ilib::static_vector<A, 20> vec{};
    ASSERT_EQ(ns::x, 1);
    vec.emplace_back();
    vec.emplace_back();
    ASSERT_EQ(ns::x, 3);
}

TEST(static_vector, size) {
    ilib::static_vector<int, 11> vec{1, 2, 3, 4, 5};
    ASSERT_EQ(vec.size(), 5);
    for (int x = 0; x < 5; ++x)
        vec.push_back(x);
    ASSERT_EQ(vec.size(), 10);
}

TEST(static_string, initialization) {
    ilib::static_string<12> str{"hello world"};
    ilib::static_string<12> str2 = str;
    ASSERT_EQ(str, str2);
}

TEST(static_string, utilities) {
    ilib::static_string<12> str{"world"};
    str.insert(0, "hello ");
    ASSERT_EQ(str, "hello world");
    str.replace(str.find("world"), str.size(), "there");
    ASSERT_EQ(str, "hello there");
}

TEST(static_string, search) {
    ilib::static_string<12> str{"hello world"};
    ASSERT_EQ(str.find("hello"), 0);
    ASSERT_EQ(str.find('h'), 0);
    ASSERT_EQ(str.rfind("hello"), 5);
    ASSERT_EQ(str.rfind(" "), 6);
    ASSERT_EQ(str.rfind("world"), 0);
    ASSERT_EQ(str.rfind('h'), 11);
}

int main(int a, char** v) {
    testing::InitGoogleTest(&a, v);
    RUN_ALL_TESTS();
}
