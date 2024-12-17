#pragma once

#include <iostream>
#include <utility>

template<typename T>
struct Node {
  T key;
  struct Node<T>* left;
  struct Node<T>* right;
  struct Node<T>* prev;
  struct Node (const T key) noexcept
    : key(key)
    , left(nullptr)
    , right(nullptr)
    , prev(nullptr) {
  }

  bool Is_postorder_isolated() const {
    if (!this->right && !this->prev) {
      return true;
    }

    return false;
  }

  Node() = default;
};
