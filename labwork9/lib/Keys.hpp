#pragma once

#include "Concepts.hpp"
#include "Functions.hpp"

template<AssociativeContainer Container>
class Keys {
public:
  Keys() = default;

  Keys(const Container& my_container)
  : my_container_(my_container) {
  }

  class iterator {
  public:
    iterator() = default;

    iterator(const typename Container::iterator& iter)
    : iter_(iter) {
    }

    auto operator*() const requires PairKeys<Container> {
      return (*iter_).first;
    }

    auto operator*() const requires(!PairKeys<Container>) {
      return *iter_;
    }

    Container::iterator base() const {
      return iter_;
    }

    iterator operator++(int) {
      iterator new_iter = *this;
      ++new_iter;

      return *this;
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
  };

  iterator begin() {
    return iterator(my_container_.begin());
  }

  iterator end() {
    return iterator(my_container_.end());
  }

private:
  Container my_container_;
};

template<AssociativeContainer container>
Keys<container> keys(const container& my_container) {
  return Keys<container>(my_container);
}

template<AssociativeContainer container>
Keys<container> operator|(container my_container, MyKeys keys) {
  return Keys<container>(my_container);
}
