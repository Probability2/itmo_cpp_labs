#pragma once

#include "MyIterator.hpp"
#include "Node.hpp"
#include "TagsOrder.hpp"

template<typename T, typename Order>
struct Iterator: public MyIterator<T, Order> {
  typedef T value_type;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T* iterator;
  typedef const T* const_iterator;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  Iterator() = default;

  Iterator(Node<T>* iter)
  : MyIterator<T, Order>(iter) {
  }

  Iterator(const MyIterator<T, Order>& iter)
  : MyIterator<T, Order>(iter.data_) {
  }

  Iterator& operator=(const Iterator& iter) {
    this->data_ = this != &iter ? iter.data_ : this->data_;

    return *this;
  }

  Iterator& operator--() override final {
    if (!this->data_) {
      std::cerr << "ERROR:\nIncorrect reference to iterator\n";
      return *this;
    }
    this->Decrement(my_traits<Order>::tag());

    return *this;
  }

  Iterator& operator++() override final {
    if (!this->data_) {
      std::cerr << "ERROR:\nIncorrect reference to iterator\n";
      return *this;
    }
    this->Increment(my_traits<Order>::tag());

    return *this;
  }

  Iterator operator++(int) {
    Iterator old = *this;
    ++old;

    return old;
  }

  Iterator operator--(int) {
    Iterator old = *this;
    --old;

    return old;
  }

  Iterator& operator+=(difference_type d) {
    while (d-- && this->data_) {
      ++(*this);
    }

    return *this;
  }

  Iterator& operator-=(difference_type d) {
    while (d-- && this->data_) {
      --(*this);
    }

    return *this;
  }

  Iterator operator+(const difference_type d) {
    Iterator result(*this);
    result += d;

    return result;
  }

  Iterator operator-(const difference_type d) {
    Iterator result(*this);
    result -= d;

    return result;
  }
};
