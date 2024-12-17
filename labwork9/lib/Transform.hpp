#pragma once

#include "Concepts.hpp"
#include "Functions.hpp"

template<SimpleContainer Container, typename Operation>
class Transform {
public:
  Transform() = default;

  Transform(const Container& my_container, const Operation& oper)
  : my_container_(my_container)
  , operation_(oper) {
  }

  Transform(const Operation& oper)
  : operation_(oper) {
  }

  class iterator {
  public:
    iterator() = default;

    iterator(const typename Container::iterator& iter, const Operation& operation)
    : iter_(iter)
    , oper_(operation) {
    }

    auto operator*() const {
      return oper_(*iter_);
    }

    Container::iterator base() const {
      return iter_;
    }

    iterator operator++(int) {
      iterator new_iter = *this;
      ++new_iter;

      return new_iter;
    }

    iterator& operator++() {
      ++iter_;

      return *this;
    }

    iterator operator--(int) requires Substractable<Container> {
      iterator new_iter = *this;
      --new_iter;

      return new_iter;
    }

    iterator& operator--() requires Substractable<Container> {
      --iter_;

      return *this;
    }

    iterator& operator+=(difference_type d) requires AddableDType<Container> {
      iter_ += d;

      return *this;
    }

    iterator& operator-=(difference_type d) requires SubstractableDType<Container> {
      iter_ -= d;

      return *this;
    }

    bool operator==(const iterator& iter) const {
      return iter_ == iter.iter_;
    }

    bool operator!=(const iterator& iter) const {
      return !(iter_ == iter.iter_);
    }

  private:
    typename Container::iterator iter_;
    Operation oper_;
  };

  iterator begin() {
    return iterator(my_container_.begin(), operation_);
  }

  iterator end() {
    return iterator(my_container_.end(), operation_);
  }

private:
  Container my_container_;
  Operation operation_;
};

template<SimpleContainer container, typename Operation>
Transform<container, Operation> transform(const container& my_container, const Operation& operation) {
  return Transform<container, Operation>(my_container, operation);
}

template<SimpleContainer container, ClassOperation MyTransform>
Transform<container, typename MyTransform::Operation_> operator|(container my_container, MyTransform mtransform) {
  return Transform<container, typename MyTransform::Operation_>(my_container, mtransform.operation_);
}

// template<std::integral T>
// void Check(T n) {
//   std::cout << "Number\n";
// }

// template<!(std::integral T)>
// void Check(T n) {
//   std::cout << "Not number\n";
// }

// static int Meta(const int n) {
//   return n * n;
// }