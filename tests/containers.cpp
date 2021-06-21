#include <gtest/gtest.h>

#include <ilib/static_vector.hpp>

namespace ns {
int x = 1;
}
TEST(static_vector, default_initialization) {
    struct A {
        A() {
            ++ns::x;
        }
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
    for (int x = 0; x < 5; ++x) vec.push_back(x);
    ASSERT_EQ(vec.size(), 10);
}

int main(int a, char** v) {
    testing::InitGoogleTest(&a, v);
    RUN_ALL_TESTS();
}
