#include "lib/BST.hpp"
#include "gtest/gtest.h"

TEST(BSTSuiteTest, SizeTest1) {
  std::BST<int, Inorder> bst;
  for (int i = 0; i < 100; ++i) {
    bst.insert(i);
  }
  ASSERT_EQ(bst.size(), 100);
}

TEST(BSTSuiteTest, SizeTest2) {
  std::BST<int, Postorder> bst;
  int k = 0;
  for (int i = -100; i < 100; i += 5 && ++k) {
    bst.insert(i);
  }
  ASSERT_EQ(bst.size(), k);
}

TEST(BSTInorderTest, ContainsTest) {
  std::BST<int, Inorder> bst;
  for (int i = 0; i < 100; ++i) {
    bst.insert(i);
  }
  for (int i = 0; i < 100; ++i) {
    ASSERT_EQ(bst.contains(i), true);
  }
  for (int i = -100; i < 0; ++i) {
    ASSERT_EQ(bst.contains(i), false);
  }
  for (int i = 100; i < 10000; ++i) {
    ASSERT_EQ(bst.contains(i), false);
  }
}

TEST(BSTPreorderTest, ContainsTest) {
  std::BST<int, Preorder> bst;
  for (int i = 0; i < 100; ++i) {
    bst.insert(i);
  }
  for (int i = 0; i < 100; ++i) {
    ASSERT_EQ(bst.contains(i), true);
  }
  for (int i = -100; i < 0; ++i) {
    ASSERT_EQ(bst.contains(i), false);
  }
  for (int i = 100; i < 10000; ++i) {
    ASSERT_EQ(bst.contains(i), false);
  }
}

TEST(BSTPostorderTest, ContainsTest) {
  std::BST<int, Postorder> bst;
  for (int i = 0; i < 100; ++i) {
    bst.insert(i);
  }
  for (int i = 0; i < 100; ++i) {
    ASSERT_EQ(bst.contains(i), true);
  }
  for (int i = -100; i < 0; ++i) {
    ASSERT_EQ(bst.contains(i), false);
  }
  for (int i = 100; i < 10000; ++i) {
    ASSERT_EQ(bst.contains(i), false);
  }
}

TEST(BSTInorderTest, FindTest) {
  std::BST<int, Inorder> bst;
  for (int i = 0; i < 100; ++i) {
    bst.insert(i);
  }
  for (int i = -100; i < 0; ++i) {
    ASSERT_EQ(bst.find(i), bst.end());
  }
  for (int i = 100; i < 10000; ++i) {
    ASSERT_EQ(bst.find(i), bst.end());
  }
}


TEST(BSTPreorderTest, FindTest) {
  std::BST<int, Preorder> bst;
  for (int i = 0; i < 100; ++i) {
    bst.insert(i);
  }
  for (int i = -100; i < 0; ++i) {
    ASSERT_EQ(bst.find(i), bst.end());
  }
  for (int i = 100; i < 10000; ++i) {
    ASSERT_EQ(bst.find(i), bst.end());
  }
}

TEST(BSTPostorderTest, FindTest) {
  std::BST<int, Postorder> bst;
  for (int i = 0; i < 100; ++i) {
    bst.insert(i);
  }
  for (int i = -100; i < 0; ++i) {
    ASSERT_EQ(bst.find(i), bst.end());
  }
  for (int i = 100; i < 10000; ++i) {
    ASSERT_EQ(bst.find(i), bst.end());
  }
}

TEST(BSTInorderTest, IterTest) {
  std::BST<int, Inorder> bst;
  for (int i = 0; i < 100; ++i) {
    bst.insert(i);
  }
  Iterator<int, Inorder> iter = bst.begin();
  for (int i = 0; i < 100; ++i) {
    ASSERT_EQ(*iter, i);
    ++iter;
  }
}

TEST(BSTPreorderTest, IterTest) {
  std::BST<int, Preorder> bst;
  for (int i = 0; i < 100; ++i) {
    bst.insert(i);
  }
  Iterator iter = bst.begin();

  for (int i = 0; i < 100; ++i) {
    ASSERT_EQ(*iter, i);
    ++iter;
  }
}

TEST(BSTPostorderTest, IterTest) {
  std::BST<int, Postorder> bst;
  for (int i = 0; i < 100; ++i) {
    bst.insert(i);
  }
  Iterator iter = bst.begin();
  for (int i = 99; i >= 0; --i) {
    ASSERT_EQ(*iter, i);
    ++iter;
  }
}

TEST(BSTInorderTest, RevTest) {
  std::BST<int, Inorder> bst;
  for (int i = 0; i < 100; ++i) {
    bst.insert(i);
  }
  Reverse_iterator<int, Inorder> iter = bst.rbegin();
  for (int i = 99; i >= 0; --i) {
    ASSERT_EQ(*iter, i);
    ++iter;
  }
}

TEST(BSTPreorderTest, RevTest) {
  std::BST<int, Preorder> bst;
  for (int i = 0; i < 100; ++i) {
    bst.insert(i);
  }
  Reverse_iterator<int, Preorder> iter = bst.rbegin();
  for (int i = 99; i >= 0; --i) {
    ASSERT_EQ(*iter, i);
    ++iter;
  }
}

TEST(BSTPostorderTest, RevTest) {
  std::BST<int, Postorder> bst;
  for (int i = 0; i < 100; ++i) {
    bst.insert(i);
  }
  Reverse_iterator<int, Postorder> iter = bst.rbegin();
  for (int i = 0; i < 100; ++i) {
    ASSERT_EQ(*iter, i);
    ++iter;
  }
}

TEST(BSTSuiteTest, CountTest) {
  std::BST<int, Inorder> bst;
  bst.insert(57);
  bst.insert(2);
  bst.insert(99);
  bst.insert(3);
  int a[200] { 0 };
  a[102] = 1;
  a[103] = 1;
  a[157] = 1;
  a[199] = 1;
  for (int i = -100; i < 100; ++i) {
    ASSERT_EQ(bst.count(i), a[i + 100]);
  }
}

TEST(BSTInorderTest, EqualTest) {
  std::BST<char, Inorder> bst_1;
  bst_1.insert('d');
  bst_1.insert('a');
  bst_1.insert('e');
  bst_1.insert('z');
  bst_1.insert('b');
  bst_1.insert('k');
  std::BST<char, Inorder> bst_2;
  bst_2.insert('d');
  bst_2.insert('b');
  bst_2.insert('e');
  bst_2.insert('a');
  bst_2.insert('k');
  bst_2.insert('z');
  ASSERT_EQ(bst_1 == bst_2, true);
  ASSERT_EQ(bst_1 != bst_2, false);
}

TEST(BSTPreorderTest, EqualTest) {
  std::BST<char, Preorder> bst_1;
  bst_1.insert('d');
  bst_1.insert('a');
  bst_1.insert('b');
  bst_1.insert('f');
  bst_1.insert('e');
  bst_1.insert('z');
  bst_1.insert('k');
  std::BST<char, Preorder> bst_2;
  bst_2.insert('d');
  bst_2.insert('a');
  bst_2.insert('b');
  bst_2.insert('f');
  bst_2.insert('e');
  bst_2.insert('z');
  bst_2.insert('k');
  std::BST<char, Preorder> bst_3;
  bst_3.insert('d');
  bst_3.insert('b');
  bst_3.insert('a');
  bst_3.insert('f');
  bst_3.insert('e');
  bst_3.insert('z');
  bst_3.insert('k');
  ASSERT_EQ(bst_1 == bst_2, true);
  ASSERT_EQ(bst_1 != bst_2, false);
  ASSERT_EQ(bst_1 != bst_3, true);
  ASSERT_EQ(bst_1 == bst_3, false);
  ASSERT_EQ(bst_2 != bst_3, true);
  ASSERT_EQ(bst_2 == bst_3, false);
  ASSERT_EQ(bst_1 == bst_1, true);
  ASSERT_EQ(bst_2 == bst_2, true);
  ASSERT_EQ(bst_3 == bst_3, true);
  ASSERT_EQ(bst_1 != bst_1, false);
  ASSERT_EQ(bst_2 != bst_2, false);
  ASSERT_EQ(bst_3 != bst_3, false);
}

TEST(BSTPostorderTest, EqualTest) {
  std::BST<char, Postorder> bst_1;
  bst_1.insert('d');
  bst_1.insert('a');
  bst_1.insert('e');
  bst_1.insert('z');
  bst_1.insert('b');
  bst_1.insert('k');
  std::BST<char, Postorder> bst_2;
  bst_2.insert('d');
  bst_2.insert('a');
  bst_2.insert('b');
  bst_2.insert('e');
  bst_2.insert('z');
  bst_2.insert('k');
  std::BST<char, Postorder> bst_3;
  bst_3.insert('c');
  bst_3.insert('a');
  bst_3.insert('b');
  bst_3.insert('e');
  bst_3.insert('z');
  bst_3.insert('k');
  ASSERT_EQ(bst_1 == bst_2, true);
  ASSERT_EQ(bst_1 != bst_2, false);
  ASSERT_EQ(bst_1 != bst_3, true);
  ASSERT_EQ(bst_1 == bst_3, false);
  ASSERT_EQ(bst_2 != bst_3, true);
  ASSERT_EQ(bst_2 == bst_3, false);
  ASSERT_EQ(bst_2 == bst_1, true);
  ASSERT_EQ(bst_2 != bst_1, false);
  ASSERT_EQ(bst_1 == bst_1, true);
  ASSERT_EQ(bst_2 == bst_2, true);
  ASSERT_EQ(bst_3 == bst_3, true);
}

TEST(BSTInorderTest, OperatorEqTest) {
  std::BST<char, Inorder> bst_1;
  bst_1.insert('d');
  bst_1.insert('a');
  bst_1.insert('e');
  bst_1.insert('z');
  bst_1.insert('b');
  bst_1.insert('k');
  std::BST<char, Inorder> bst_2;
  bst_2 = bst_1;
  ASSERT_EQ(bst_1 == bst_2, true);
  ASSERT_EQ(bst_1 != bst_2, false);
}

TEST(BSTPreorderTest, OperatorEqTest) {
  std::BST<char, Preorder> bst_1;
  bst_1.insert('d');
  bst_1.insert('a');
  bst_1.insert('e');
  bst_1.insert('z');
  bst_1.insert('b');
  bst_1.insert('k');
  std::BST<char, Preorder> bst_2;
  bst_2 = bst_1;
  ASSERT_EQ(bst_1 == bst_2, true);
  ASSERT_EQ(bst_1 != bst_2, false);
}

TEST(BSTPostorderTest, OperatorEqTest) {
  std::BST<char, Postorder> bst_1;
  bst_1.insert('d');
  bst_1.insert('a');
  bst_1.insert('e');
  bst_1.insert('z');
  bst_1.insert('b');
  bst_1.insert('k');
  std::BST<char, Postorder> bst_2;
  bst_2 = bst_1;
  ASSERT_EQ(bst_1 == bst_2, true);
  ASSERT_EQ(bst_1 != bst_2, false);
}

TEST(BSTSuiteTest, OperatorEqLongTest) {
  std::BST<long long, Inorder> bst_1;
  for (int i = 0; i < 250; ++i) {
    bst_1.insert(i);
  }
  std::BST<long long, Inorder> bst_2;
  bst_2 = bst_1;
  ASSERT_EQ(bst_1 == bst_2, true);
  ASSERT_EQ(bst_1.get_allocator() == bst_2.get_allocator(), true);
  ASSERT_EQ(bst_1 != bst_2, false);
}

TEST(BSTSuiteTest, BSTSwapTest) {
  std::BST<long long, Inorder> bst1_copy;
  std::BST<long long, Inorder> bst2_copy;
  std::BST<long long, Inorder> bst_1;
  for (int i = 0; i < 250; ++i) {
    bst_1.insert(i);
    bst1_copy.insert(i);
  }
  std::BST<long long, Inorder> bst_2;
  for (int i = -1; i < 25; ++i) {
    bst_2.insert(i);
    bst2_copy.insert(i);
  }
  bst_1.swap(bst_2);
  ASSERT_EQ(bst_1 == bst2_copy, true);
  ASSERT_EQ(bst_1.get_allocator() == bst2_copy.get_allocator(), true);
  ASSERT_EQ(bst_2 == bst1_copy, true);
  ASSERT_EQ(bst_2.get_allocator() == bst1_copy.get_allocator(), true);
  ASSERT_EQ(bst_1 == bst1_copy, false);
  ASSERT_EQ(bst_2 == bst2_copy, false);
}

TEST(BSTSuiteTest, NonBSTSwapTest) {
  std::BST<long long, Inorder> bst1_copy;
  std::BST<long long, Inorder> bst2_copy;
  std::BST<long long, Inorder> bst_1;
  for (int i = 0; i < 250; ++i) {
    bst_1.insert(i);
    bst1_copy.insert(i);
  }
  std::BST<long long, Inorder> bst_2;
  for (int i = -1; i < 25; ++i) {
    bst_2.insert(i);
    bst2_copy.insert(i);
  }
  std::swap(bst_1, bst_2);
  ASSERT_EQ(bst_1 == bst2_copy, true);
  ASSERT_EQ(bst_1.get_allocator() == bst2_copy.get_allocator(), true);
  ASSERT_EQ(bst_2 == bst1_copy, true);
  ASSERT_EQ(bst_2.get_allocator() == bst1_copy.get_allocator(), true);
  ASSERT_EQ(bst_1 == bst1_copy, false);
  ASSERT_EQ(bst_2 == bst2_copy, false);
}

TEST(BSTSuiteTest, EmptyTest) {
  std::BST<int, Inorder> bst1;
  std::BST<char, Preorder> bst2;
  bst2.insert('a');
  bst2.insert('b');
  std::BST<long, Postorder> bst3;
  bst3.insert(3);
  ASSERT_EQ(bst1.empty(), true);
  ASSERT_EQ(bst2.empty(), false);
  ASSERT_EQ(bst3.empty(), false);
}

TEST(BSTSuiteTest, CopyConstructorTest) {
  std::BST<char, Preorder> bst_1;
  bst_1.insert('d');
  bst_1.insert('a');
  bst_1.insert('e');
  bst_1.insert('z');
  bst_1.insert('b');
  bst_1.insert('k');
  std::BST<char, Preorder> bst_2(bst_1);
  ASSERT_EQ(bst_1 == bst_2, true);
  ASSERT_EQ(bst_1 != bst_2, false);
}

TEST(BSTSuiteTest, InsertRangeTest) {
  std::BST<char, Inorder> bst1;
  bst1.insert('d');
  bst1.insert('a');
  bst1.insert('e');
  bst1.insert('z');
  bst1.insert('b');
  bst1.insert('k');
  char a[6] = {'a', 'b', 'd', 'e', 'k', 'z'};
  std::BST<char, Inorder> bst2;
  std::BST<char, Inorder> bst3;
  bst2.insert(bst1.begin(), bst1.end());
  bst3.insert(bst1.begin() + 2, bst1.end() - 1);
  ASSERT_EQ(bst1 == bst2, true);
  ASSERT_EQ(bst1 != bst2, false);
  Reverse_iterator<char, Inorder> it = bst3.rbegin();
  for (int i = 4; i >= 2; --i) {
    ASSERT_EQ(*it, a[i]);
    ++it;
  }
}

TEST(BSTSuiteTest, InitializerListTest1) {
  std::BST<char, Inorder> bst1;
  bst1.insert('d');
  bst1.insert('a');
  bst1.insert('e');
  bst1.insert('z');
  bst1.insert('b');
  bst1.insert('k');
  std::initializer_list<char> l = {'d', 'a', 'e', 'z', 'b', 'k'};
  std::BST<char, Inorder> bst2(l);
  ASSERT_EQ(bst1 == bst2, true);
  ASSERT_EQ(bst1 != bst2, false);
}

TEST(BSTSuiteTest, InitializerListTest2) {
  std::BST<int, Postorder> bst1;
  for (int i = 0; i < 10; ++i) {
    bst1.insert(i);
  }
  std::initializer_list<int> l = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::BST<int, Postorder> bst2(l);
  std::initializer_list<int> l2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  std::BST<int, Postorder> bst3(l2);
  ASSERT_EQ(bst1 == bst2, true);
  ASSERT_EQ(bst1 != bst2, false);
  ASSERT_EQ(bst1 == bst3, false);
  ASSERT_EQ(bst1 != bst3, true);
}

TEST(BSTSuiteTest, MergerTest) {
  std::BST<int, Inorder> bst1;
  std::BST<int, Inorder> bst2;
  for (int i = 0; i < 10; ++i) {
    bst1.insert(i);
  }
  for (int i = 10; i < 100; ++i) {
    bst2.insert(i);
  }
  std::size_t size = bst1.size() + bst2.size();
  bst1.merge(bst2);
  for (int i = 0; i < 100; ++i) {
    ASSERT_EQ(bst1.contains(i), true);
  }
  ASSERT_EQ(bst1.size(), size);
}

TEST(BSTSuiteTest, LowerBoundInorderTest) {
  std::BST<int, Inorder> bst;
  for (int i = 0; i < 10; ++i) {
    bst.insert(i);
  }
  auto iter = bst.lower_bound(2);
  for (int i = 2; i < 10; ++i) {
    ASSERT_EQ(*iter, i);
    ++iter;
  }
}

TEST(BSTInorderTest, UpperBoundTest) {
  std::BST<int, Inorder> bst;
  for (int i = 0; i < 10; ++i) {
    bst.insert(i);
  }
  auto iter = bst.upper_bound(2);
  for (int i = 3; i < 10; ++i) {
    ASSERT_EQ(*iter, i);
    ++iter;
  }
}

TEST(BSTInorderTest, LowerBoundTest2) {
  std::BST<int, Inorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  int a[7] = {2, 4, 6, 8, 10, 14, 16};
  int ans[20] { 0 };
  int k = -1;
  int ind = -1;
  for (int i = 0; i < 20; ++i) {
    if (k < i) {
      ++ind;
      k = ind < 7 ? a[ind] : 16;
    }
    ans[i] = k;
  }
  for (int i = 0; i < 20; ++i) {
    auto iter = bst.lower_bound(i);
    iter = iter == bst.end() ? iter - 1 : iter;
    ASSERT_EQ(*iter, ans[i]);
  }
}

TEST(BSTInorderTest, UpperBoundTest2) {
  std::BST<int, Inorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  int a[7] = {2, 4, 6, 8, 10, 14, 16};
  int ans[20] { 0 };
  int k = -1;
  int ind = -1;
  for (int i = 0; i < 20; ++i) {
    if (k <= i) {
      ++ind;
      k = ind < 7 ? a[ind] : 16;
    }
    ans[i] = k;
  }
  for (int i = 0; i < 20; ++i) {
    auto iter = bst.upper_bound(i);
    iter = iter == bst.end() ? iter - 1 : iter;
    ASSERT_EQ(*iter, ans[i]);
  }
}

TEST(BSTPreorderTest, LowerBoundTest) {
  std::BST<int, Preorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  int a[3] = {8, 14, 16};
  int ans[20] { 0 };
  int k = -1;
  int ind = -1;
  for (int i = 0; i < 20; ++i) {
    if (k < i) {
      ++ind;
      k = ind < 3 ? a[ind] : 16;
    }
    ans[i] = k;
  }
  for (int i = 0; i < 20; ++i) {
    auto iter = bst.lower_bound(i);
    iter = iter == bst.end() ? iter - 1 : iter;
    ASSERT_EQ(*iter, ans[i]);
  }
}

TEST(BSTPreorderTest, UpperBoundTest) {
  std::BST<int, Preorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  int a[3] = {8, 14, 16};
  int ans[20] { 0 };
  int k = -1;
  int ind = -1;
  for (int i = 0; i < 20; ++i) {
    if (k <= i) {
      ++ind;
      k = ind < 3 ? a[ind] : 16;
    }
    ans[i] = k;
  }
  for (int i = 0; i < 20; ++i) {
    auto iter = bst.upper_bound(i);
    iter = iter == bst.end() ? iter - 1 : iter;
    ASSERT_EQ(*iter, ans[i]);
  }
}

TEST(BSTPostorderTest, LowerBoundTest) {
  std::BST<int, Postorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  int ans[20] = {2, 2, 2, 6, 6, 6, 6, 10, 10, 10, 10, 16, 16, 16, 16, 16, 16, 8, 8, 8};
  for (int i = 0; i < 20; ++i) {
    auto iter = bst.lower_bound(i);
    iter = iter == bst.end() ? iter - 1 : iter;
    ASSERT_EQ(*iter, ans[i]);
  }
}

TEST(BSTPostorderTest, UpperBoundTest) {
  std::BST<int, Postorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  int ans[20] = {2, 2, 2, 6, 6, 6, 6, 10, 10, 10, 10, 16, 16, 16, 16, 16, 16, 8, 8, 8};
  for (int i = 0; i < 20; ++i) {
    auto iter = bst.upper_bound(i);
    iter = iter == bst.end() ? iter - 1 : iter;
    ASSERT_EQ(*iter, ans[i]);
  }
}

TEST(BSTInorderTest, RevTest2) {
  std::BST<int, Inorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  int ans[7] = {2, 4, 6, 8, 10, 14, 16};
  Reverse_iterator<int, Inorder> iter = bst.rbegin();
  for (int i = 6; i >= 0; --i) {
    ASSERT_EQ(*iter, ans[i]);
    ++iter;
  }
}

TEST(BSTPreorderTest, RevTest2) {
  std::BST<int, Preorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  int ans[7] = {8, 4, 2, 6, 14, 10, 16};
  Reverse_iterator<int, Preorder> iter = bst.rbegin();
  for (int i = 6; i >= 0; --i) {
    ASSERT_EQ(*iter, ans[i]);
    ++iter;
  }
}

TEST(BSTPostorderTest, RevTest2) {
  std::BST<int, Postorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  int ans[7] = {2, 6, 4, 10, 16, 14, 8};
  Reverse_iterator<int, Postorder> iter = bst.rbegin();
  for (int i = 6; i >= 0; --i) {
    ASSERT_EQ(*iter, ans[i]);
    ++iter;
  }
}

TEST(BSTInorderTest, IterTest2) {
  std::BST<int, Inorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  int ans[7] = {2, 4, 6, 8, 10, 14, 16};
  Iterator iter = bst.begin();
  for (int i = 0; i < 7; ++i) {
    ASSERT_EQ(*iter, ans[i]);
    ++iter;
  }
}

TEST(BSTPreorderTest, IterTest2) {
  std::BST<int, Preorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  int ans[7] = {8, 4, 2, 6, 14, 10, 16};
  Iterator iter = bst.begin();
  for (int i = 0; i < 7; ++i) {
    ASSERT_EQ(*iter, ans[i]);
    ++iter;
  }
}

TEST(BSTPostorderTest, IterTest2) {
  std::BST<int, Postorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  int ans[7] = {2, 6, 4, 10, 16, 14, 8};
  Iterator iter = bst.begin();
  for (int i = 0; i < 7; ++i) {
    ASSERT_EQ(*iter, ans[i]);
    ++iter;
  }
}

TEST(BSTSuiteTest, EraseTest) {
  std::BST<int, Inorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  bst.erase(4);
  int ans1[6] = {2, 6, 8, 10, 14, 16};
  Iterator iter = bst.begin();
  for (int i = 0; i < 6; ++i) {
    ASSERT_EQ(*iter, ans1[i]);
    ++iter;
  }
  bst.erase(8);
  int ans2[5] = {2, 6, 10, 14, 16};
  iter = bst.begin();
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(*iter, ans2[i]);
    ++iter;
  }
}

TEST(BSTInorderTest, EraseIterTest) {
  std::BST<int, Inorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  bst.erase(bst.begin());
  int ans[6] = {4, 6, 8, 10, 14, 16};
  Iterator iter = bst.begin();
  for (int i = 0; i < 6; ++i) {
    ASSERT_EQ(*iter, ans[i]);
    ++iter;
  }
}

TEST(BSTPreorderTest, EraseIterTest) {
  std::BST<int, Preorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  bst.erase(bst.begin());
  int ans[6] = {10, 4, 2, 6, 14, 16};
  Iterator iter = bst.begin();
  for (int i = 0; i < 6; ++i) {
    ASSERT_EQ(*iter, ans[i]);
    ++iter;
  }
}

TEST(BSTPostorderTest, EraseIterTest) {
  std::BST<int, Postorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  bst.erase(bst.begin());
  int ans[6] = {6, 4, 10, 16, 14, 8};
  Iterator iter = bst.begin();
  for (int i = 0; i < 6; ++i) {
    ASSERT_EQ(*iter, ans[i]);
    ++iter;
  }
}

TEST(BSTInorderTest, EraseStackTest) {
  std::BST<int, Inorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  bst.erase(bst.begin(), bst.begin() + 2);
  int ans[5] = {6, 8, 10, 14, 16};
  Iterator iter = bst.begin();
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(*iter, ans[i]);
    ++iter;
  }
}

TEST(BSTPreorderTest, EraseStackTest) {
  std::BST<int, Preorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  bst.erase(bst.begin(), bst.begin() + 2);
  int ans[5] = {10, 6, 2, 14, 16};
  Iterator iter = bst.begin();
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(*iter, ans[i]);
    ++iter;
  }
}

TEST(BSTPostorderTest, EraseStackTest) {
  std::BST<int, Postorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  bst.erase(bst.begin(), bst.begin() + 2);
  int ans[5] = {4, 10, 16, 14, 8};
  Iterator iter = bst.begin();
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(*iter, ans[i]);
    ++iter;
  }
}

TEST(BSTSuiteTest, ClearTest) {
  std::BST<int, Inorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  bst.clear();
  ASSERT_EQ(bst.size(), 0);
}