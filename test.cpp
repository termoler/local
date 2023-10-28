#include "flat_map.h"
#include <string>
#include "cmake-build-debug/_deps/googletest-src/googletest/include/gtest/gtest.h"



TEST(FlatMapTest, get_set) {
    flat_map<std::string, std::string> m;
    EXPECT_EQ(m.size(), 0);

    m["hello"] = "world";
    m["abc"] = "defg";
    m["123"] = "111";

    EXPECT_EQ(m.size(), 3);
    EXPECT_EQ(m["hello"], "world");
    EXPECT_EQ(m["abc"], "defg");
    EXPECT_EQ(m["123"], "111");
}
TEST(FlatMapTest, contains) {
    flat_map<std::string, std::string> m;
    m["2 + 2"] = "4";

    EXPECT_TRUE(m.contains("2 + 2"));
    EXPECT_FALSE(m.contains("2 + 3"));
}

TEST(FlatMapTest, copy) {
    flat_map<std::string, std::string> m1;
    m1["2 + 2"] = "4";
    m1["qwe"] = "rty";
    m1["hello"] = "world";

    flat_map<std::string, std::string> m2(m1);
    m1["qwe"] = "www";
    EXPECT_EQ(m1["qwe"], "www");
    EXPECT_EQ(m2.size(), 3);
    EXPECT_EQ(m2["2 + 2"], "4");
    EXPECT_EQ(m2["qwe"], "rty");
    EXPECT_EQ(m2["hello"], "world");

    m1 = m2;
    EXPECT_EQ(m1.size(), 3);
    EXPECT_EQ(m1["2 + 2"], "4");
    EXPECT_EQ(m1["qwe"], "rty");
    EXPECT_EQ(m1["hello"], "world");
}

TEST(FlatMapTest, move) {
    flat_map<std::string, std::string> m1;
    m1["2 + 2"] = "4";
    m1["qwe"] = "rty";
    m1["hello"] = "world";

    flat_map<std::string, std::string> m2(m1);
    EXPECT_EQ(m2.size(), 3);
    EXPECT_EQ(m2["2 + 2"], "4");
    EXPECT_EQ(m2["qwe"], "rty");
    EXPECT_EQ(m2["hello"], "world");

    m1 = std::move(m2);
    EXPECT_EQ(m1.size(), 3);
    EXPECT_EQ(m1["2 + 2"], "4");
    EXPECT_EQ(m1["qwe"], "rty");
    EXPECT_EQ(m1["hello"], "world");
}
TEST(FlatMapTest, erase) {
    flat_map<std::string, std::string> m;
    m["2 + 2"] = "4";

    EXPECT_EQ(m.erase("2 + 3"), 0);
    EXPECT_EQ(m.erase("2 + 2"), 1);
    EXPECT_EQ(m.erase("2 + 2"), 0);
    EXPECT_EQ(m.size(), 0);
}

TEST(FlatMapTest, iterator) {
    flat_map<std::string, std::string> m;
    m["hello"] = "world";
    m["abc"] = "defg";
    m["123"] = "111";

    auto it = m.begin();
    EXPECT_EQ(it->first, "123");
    EXPECT_EQ(it->second, "111");
    ++it;
    ++it;
    EXPECT_EQ(it->first, "hello");
    EXPECT_EQ(it->second, "world");
}
