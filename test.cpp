#include <gtest/gtest.h>
#include "skip_list.hpp"
#include <string>
#include <vector>

TEST(SkipListTest, ConstructorAndEmpty) {
    skip_list<int, std::string> sl;
    EXPECT_TRUE(sl.empty());
    EXPECT_EQ(sl.size(), 0);
}

TEST(SkipListTest, InsertAndSize) {
    skip_list<int, std::string> sl;
    sl.insert(1, "one");
    sl.insert(2, "two");
    sl.insert(3, "three");
    
    EXPECT_FALSE(sl.empty());
    EXPECT_EQ(sl.size(), 3);
}

TEST(SkipListTest, FindExistingElement) {
    skip_list<int, std::string> sl;
    sl.insert(1, "one");
    sl.insert(2, "two");
    
    auto it = sl.find(1);
    EXPECT_TRUE(it);
    EXPECT_EQ(*it, "one");
    
    it = sl.find(2);
    EXPECT_TRUE(it);
    EXPECT_EQ(*it, "two");
}

TEST(SkipListTest, FindNonExistingElement) {
    skip_list<int, std::string> sl;
    sl.insert(1, "one");
    
    auto it = sl.find(42);
    EXPECT_FALSE(it);
}

TEST(SkipListTest, OperatorBrackets) {
    skip_list<int, std::string> sl;
    sl[1] = "one";
    sl[2] = "two";
    
    EXPECT_EQ(sl[1], "one");
    EXPECT_EQ(sl[2], "two");
    EXPECT_EQ(sl.size(), 2);
    
    // Test creating new element with operator[]
    sl[3];
    EXPECT_EQ(sl.size(), 3);
}

TEST(SkipListTest, RemoveExistingElement) {
    skip_list<int, std::string> sl;
    sl.insert(1, "one");
    sl.insert(2, "two");
    
    EXPECT_EQ(sl.size(), 2);
    sl.remove(1);
    EXPECT_EQ(sl.size(), 1);
    EXPECT_FALSE(sl.find(1));
    EXPECT_TRUE(sl.find(2));
}

TEST(SkipListTest, RemoveNonExistingElement) {
    skip_list<int, std::string> sl;
    sl.insert(1, "one");
    
    sl.remove(42); // Should do nothing
    EXPECT_EQ(sl.size(), 1);
}

TEST(SkipListTest, Clear) {
    skip_list<int, std::string> sl;
    sl.insert(1, "one");
    sl.insert(2, "two");
    
    EXPECT_EQ(sl.size(), 2);
    sl.clear();
    EXPECT_TRUE(sl.empty());
    EXPECT_EQ(sl.size(), 0);
}

TEST(SkipListTest, IteratorBasic) {
    skip_list<int, std::string> sl;
    sl.insert(1, "one");
    sl.insert(2, "two");
    sl.insert(3, "three");
    
    auto it = sl.begin();
    EXPECT_TRUE(it);
    EXPECT_EQ(*it, "one");
    
    ++it;
    EXPECT_EQ(*it, "two");
    
    ++it;
    EXPECT_EQ(*it, "three");
    
    ++it;
    EXPECT_FALSE(it);
}

TEST(SkipListTest, IteratorRangeBasedFor) {
    skip_list<int, std::string> sl;
    sl.insert(1, "one");
    sl.insert(2, "two");
    sl.insert(3, "three");
    
    std::vector<std::string> result;
    for (auto& value : sl) {
        result.push_back(value);
    }
    
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "one");
    EXPECT_EQ(result[1], "two");
    EXPECT_EQ(result[2], "three");
}

TEST(SkipListTest, IteratorPostIncrement) {
    skip_list<int, std::string> sl;
    sl.insert(1, "one");
    sl.insert(2, "two");
    
    auto it = sl.begin();
    auto old_it = it++;
    EXPECT_EQ(*old_it, "one");
    EXPECT_EQ(*it, "two");
}

TEST(SkipListTest, IteratorDecrement) {
    skip_list<int, std::string> sl;
    sl.insert(1, "one");
    sl.insert(2, "two");
    
    auto it = sl.begin();
    ++it;
    --it;
    EXPECT_EQ(*it, "one");
}

TEST(SkipListTest, IteratorRemove) {
    skip_list<int, std::string> sl;
    sl.insert(1, "one");
    sl.insert(2, "two");
    
    auto it = sl.find(1);
    it.remove();
    
    EXPECT_EQ(sl.size(), 1);
    EXPECT_FALSE(sl.find(1));
    EXPECT_TRUE(sl.find(2));
}

TEST(SkipListTest, LargeInsertAndFind) {
    skip_list<int, int> sl;
    const int N = 1000;
    
    for (int i = 0; i < N; ++i) {
        sl.insert(i, i * 10);
    }
    
    EXPECT_EQ(sl.size(), N);
    
    for (int i = 0; i < N; ++i) {
        auto it = sl.find(i);
        EXPECT_TRUE(it);
        EXPECT_EQ(*it, i * 10);
    }
    
    // Test non-existing elements
    EXPECT_FALSE(sl.find(-1));
    EXPECT_FALSE(sl.find(N));
}

TEST(SkipListTest, StringKeys) {
    skip_list<std::string, int> sl;
    sl.insert("apple", 1);
    sl.insert("banana", 2);
    sl.insert("cherry", 3);
    
    EXPECT_EQ(sl.size(), 3);
    EXPECT_EQ(sl["apple"], 1);
    EXPECT_EQ(sl["banana"], 2);
    EXPECT_EQ(sl["cherry"], 3);
}

TEST(SkipListTest, CustomComparator) {
    struct CaseInsensitiveCompare {
        bool operator()(const std::string& a, const std::string& b) const {
            return std::lexicographical_compare(
                a.begin(), a.end(),
                b.begin(), b.end(),
                [](char c1, char c2) {
                    return tolower(c1) < tolower(c2);
                });
        }
    };
    
    skip_list<std::string, int, CaseInsensitiveCompare> sl;
    sl.insert("Apple", 1);
    sl.insert("banana", 2);
    sl.insert("CHERRY", 3);
    
    EXPECT_EQ(sl["apple"], 1);
    EXPECT_EQ(sl["BANANA"], 2);
    EXPECT_EQ(sl["cherry"], 3);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
