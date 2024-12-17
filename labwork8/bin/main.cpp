#include "lib/BST.hpp"

int main() {
  std::BST<int, Inorder> bst;
  bst.insert(8);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(14);
  bst.insert(16);
  bst.insert(10);
  bst.clear();
  
  std::cout << bst << '\n';

  return EXIT_SUCCESS;
}
