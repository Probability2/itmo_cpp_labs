#include "lib/Transform.hpp"
#include "lib/Filter.hpp"
#include "lib/Reverse.hpp"
#include "lib/Take.hpp"
#include "lib/Drop.hpp"
#include "lib/Keys.hpp"
#include "lib/Values.hpp"

#include "gtest/gtest.h"

#include <set>

TEST(AdapterSuiteTest, TransformTest) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6};
  std::vector<int> ans = {1, 4, 9, 16, 25, 36};
  auto adapter = v | MyTransform([](int i){return i * i;});
  int ind = 0;
  for (auto i: adapter) {
    ASSERT_EQ(i, ans[ind]);
    ++ind;
  }
}

TEST(AdapterSuiteTest, FilterTest) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6, 101};
  std::vector<int> ans = {1, 3, 5, 101};
  auto adapter = v | MyFilter([](int i){return i % 2;});
  int ind = 0;
  for (auto i: adapter) {
    ASSERT_EQ(i, ans[ind]);
    ++ind;
  }
}

TEST(AdapterSuiteTest, TransformFilterTest) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6, 101};
  std::vector<int> ans = {1, 9, 25, 10201};
  auto adapter = v | MyTransform([](int i){return i * i;}) | MyFilter([](int i){return i % 2;});
  int ind = 0;
  for (auto i: adapter) {
    ASSERT_EQ(i, ans[ind]);
    ++ind;
  }
}

TEST(AdapterSuiteTest, ReverseTest1) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6};
  auto adapter = v | MyReverse();
  std::size_t ind = v.size() - 1;
  for (auto i: adapter) {
    ASSERT_EQ(i, v[ind]);
    --ind;
  }
}

TEST(AdapterSuiteTest, ReverseTest2) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6};
  auto adapter = v | MyReverse() | MyReverse() | MyReverse() | MyReverse();
  std::size_t ind = 0;
  for (auto i: adapter) {
    ASSERT_EQ(i, v[ind]);
    ++ind;
  }
}

TEST(AdapterSuiteTest, TakeTest1) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6};
  std::vector<int> ans = {1, 2, 3};
  auto adapter = v | MyTake(3);
  std::size_t ind = 0;
  for (auto i: adapter) {
    ASSERT_EQ(i, ans[ind]);
    ++ind;
  }
}

TEST(AdapterSuiteTest, TakeTest2) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6};
  auto adapter = v | MyTake(100);
  std::size_t ind = 0;
  for (auto i: adapter) {
    ASSERT_EQ(i, v[ind]);
    ++ind;
  }
}

TEST(AdapterSuiteTest, TakeTransformTest) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6};
  std::vector<int> ans = {3, 6, 9, 12};
  auto adapter = v | MyTransform([](int i){return i * 3;}) | MyTake(4);
  std::size_t ind = 0;
  for (auto i: adapter) {
    ASSERT_EQ(i, ans[ind]);
    ++ind;
  }
}

TEST(AdapterSuiteTest, TakeReverseTest) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6};
  std::vector<int> ans = {6, 5, 4, 3};
  auto adapter = v | MyReverse() | MyTake(4);
  std::size_t ind = 0;
  for (auto i: adapter) {
    ASSERT_EQ(i, ans[ind]);
    ++ind;
  }
}

TEST(AdapterSuiteTest, DropTest) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6};
  std::vector<int> ans = {3, 4, 5, 6};
  auto adapter = v | MyDrop(2);
  std::size_t ind = 0;
  for (auto i: adapter) {
    ASSERT_EQ(i, ans[ind]);
    ++ind;
  }
}

TEST(AdapterSuiteTest, TakeDropTest) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6};
  std::vector<int> ans = {3, 4};
  auto adapter = v | MyDrop(2) | MyTake(2);
  std::size_t ind = 0;
  for (auto i: adapter) {
    ASSERT_EQ(i, ans[ind]);
    ++ind;
  }
}

TEST(AdapterSuiteTest, KeysTest1) {
  std::map<char, int> v{{'a', 1}, {'b', 3}, {'c', 2}, {'d', 4}, {'e', 5}, {'z', 201}};
  std::vector<char> keys = {'a', 'b', 'c', 'd', 'e', 'z'};
  auto res = v | MyKeys();
  std::size_t ind = 0;
  for (auto i: res) {
    ASSERT_EQ(i, keys[ind]);
    ++ind;
  }
}

TEST(AdapterSuiteTest, ValuesTest) {
  std::map<char, int> v{{'a', 1}, {'b', 3}, {'c', 2}, {'d', 4}, {'e', 5}, {'z', 201}, {'p', 33}};
  std::vector<int> values = {1, 3, 2, 4, 5, 33, 201};
  auto res = v | MyValues();
  std::size_t ind = 0;
  for (auto i: res) {
    ASSERT_EQ(i, values[ind]);
    ++ind;
  }
}

TEST(AdapterSuiteTest, ValuesTest2) {
  std::set<int> v = {1, 2, 3, 4, 5};
  std::vector<int> values = {1, 2, 3, 4, 5};
  auto res = v | MyValues();
  std::size_t ind = 0;
  for (auto i: res) {
    ASSERT_EQ(i, values[ind]);
    ++ind;
  }
}