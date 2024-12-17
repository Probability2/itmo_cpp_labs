#pragma once

#include "Node.hpp"
#include "TagsOrder.hpp"

template<typename T, typename Order>
class MyIterator {
public:
  Node<T>* data_;

  typedef T value_type;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T* iterator;
  typedef const T* const_iterator;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  MyIterator() = default;

  explicit MyIterator(Node<T>* data)
  : data_(data) {
  }

  T operator*() const {
    if (!data_) {
      std::cerr << "ERROR:\nReference to null iterator\n";
    }

    return data_->key;
  }

  MyIterator& operator=(const MyIterator& iter) {
    this->data_ = this != &iter ? iter.data_ : this->data_;

    return *this;
  }

  virtual ~MyIterator() {};

  void Increment(const tag_inorder&) {
    if (data_->right) {
      data_ = data_->right;
      while (data_ && data_->left) {
        data_ = data_->left;
      }
    } else {
      while (data_) {
        if (!data_->prev) {
          std::cerr << "ERROR:\nIncorrect reference to iterator\n";
          return;
        }
        if (data_->prev->left == data_) {
          break;
        }
        data_ = data_->prev;
      }
      data_ = data_->prev;
    }
  }

  void Increment(const tag_preorder&) {
    if (data_->left) {
      data_ =  data_->left;
    } else if (data_->right) {
      data_ = data_->right;
    } else {
      if (data_->prev->left == data_ && data_->prev->right) {
        data_ = data_->prev->right;
      } else {
        data_ = data_->prev;
        FindPrevRoot();
      }
    }
  }

  void Increment(const tag_postorder&) {
    if (data_->prev->left == data_) {
      if (data_->prev->Is_postorder_isolated()) {
        data_ = data_->prev;
        return;
      }
      if (data_->prev->left == data_ && data_->prev->right) {
        data_ = data_->prev->right;
        FindLeftRoot();
      } else {
        data_ = data_->prev;
      }
    } else {
      data_ = data_->prev;
    }
  }

  void Decrement(const tag_inorder&) {
    if (data_->left) {
      data_ = data_->left;
      if (!data_) {
        return;
      }
      GoThroughtRight();
    } else if (data_->prev->left == data_) {
      data_ = data_->prev->prev;
    } else {
      data_ = data_->prev;
    }
  }

  void Decrement(const tag_preorder&) {
    if (!data_->prev) {
      std::cerr << "ERROR:\nIncorrect reference to iterator\n";
      return;
    }
    if (data_->prev->right == data_) {
      if (!data_->prev->left) {
        data_ = data_->prev;
        return;
      }
      data_ = data_->prev->left;
      FindRightRoot();
    } else if (data_->prev->left == data_) {
      data_ = data_->prev;
    }
  }

  void Decrement(const tag_postorder&) {
    if (data_->right) {
      data_ = data_->right;
    } else {
      while (data_ && data_->prev) {
        if (data_->prev->left != data_) {
          data_ = data_->prev;
          break;
        }
        data_ = data_->prev;
      }
      data_ = data_->left;
    }
  }

  virtual MyIterator& operator--() = 0;

  virtual MyIterator& operator++() = 0;

  bool operator!() const noexcept {
    return !data_;
  }

  explicit operator bool() const {
    return data_ != nullptr;
  }

  Node<T>* GetData() const noexcept {
    return data_;
  }

  bool operator==(const MyIterator& iter) const {
    if (!(*this) && !iter) {
      return true;
    } else if (!(*this) || !iter) {
      return false;
    }

    return data_->key == iter.data_->key;
  }

  bool operator!=(const MyIterator& iter) const {
    return !(*this == iter);
  }

private:
  void GoThroughtRight() {
    while (data_ && data_->right) {
      data_ = data_->right;
    }
  }

  void FindLeftRoot() {
    while (data_ && (data_->left || data_->right)) {
      if (data_->left) {
        data_ = data_->left;
        continue;
      }
      data_ = data_->right;
    }
  }

  void FindRightRoot() {
    while (data_ && (data_->left || data_->right)) {
      if (data_->right) {
        data_ = data_->right;
        continue;
      }
      data_ = data_->left;
    }
  }

  void FindPrevRoot() {
    while (data_ && data_->prev) {
      data_ = data_->prev;
      if (data_->right) {
        data_ = data_->right;
        break;
      }
    }
  }
};
