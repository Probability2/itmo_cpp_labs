#pragma once

#include "MyIterator.hpp"
#include "Node.hpp"
#include "TagsOrder.hpp"

template<typename T, typename Order>
struct Reverse_iterator: public MyIterator<T, Order> {
  typedef T value_type;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T* iterator;
  typedef const T* const_iterator;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  Reverse_iterator() = default;

  explicit Reverse_iterator(Node<T>* data)
  : MyIterator<T, Order>(data) {
  }

  Reverse_iterator(const MyIterator<T, Order>& iter)
  : MyIterator<T, Order>(iter.data_) {
  }

  Reverse_iterator& operator=(const Reverse_iterator& iter) {
    this->data_ = this != &iter ? iter.data_ : this->data_;
    
    return *this;
  }

  Reverse_iterator& operator--() override final {
    if (!this->data_) {
      std::cerr << "ERROR:\nIncorrect reference to iterator\n";
      return *this;
    }
    this->Increment(my_traits<Order>::tag());

    return *this;
  }

  Reverse_iterator& operator++() override final {
    if (!this->data_) {
      std::cerr << "ERROR:\nIncorrect reference to iterator\n";
      return *this;
    }
    this->Decrement(my_traits<Order>::tag());

    return *this;
  }

  Reverse_iterator operator++(int) {
    Reverse_iterator old = *this;
    ++old;

    return old;
  }

  Reverse_iterator operator--(int) {
    Reverse_iterator old = *this;
    --old;

    return old;
  }

  Reverse_iterator& operator+=(difference_type d) {
    while (d-- && this->data_) {
      ++(*this);
    }

    return *this;
  }

  Reverse_iterator& operator-=(difference_type d) {
    while (d-- && this->data_) {
      --(*this);
    }

    return *this;
  }

  Reverse_iterator operator+(const difference_type d) {
    Reverse_iterator result(*this);
    result += d;

    return result;
  }

  Reverse_iterator operator-(const difference_type d) {
    Reverse_iterator result(*this);
    result -= d;

    return result;
  }
};
