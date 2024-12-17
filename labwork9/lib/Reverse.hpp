#pragma once

#include "Concepts.hpp"
#include "Functions.hpp"

template<SimpleContainer Container>
class Reverse {
public:
  Reverse() = default;

  Reverse(const Container& my_container)
  : my_container_(my_container) {
  }

  class iterator {
  public:
    iterator() = default;

    iterator(const typename Container::iterator& iter, const typename Container::iterator& begin,
             const typename Container::iterator& end)
    : iter_(iter)
    , begin_(begin)
    , end_(end) {
    }

    auto operator*() const {
      return *iter_;
    }

    Container::iterator base() const {
      return iter_;
    }

    iterator operator++(int) requires Substractable<Container> {
      iterator new_iter = *this;
      ++new_iter;

      return new_iter;
    }

    iterator& operator++() requires Substractable<Container> {
      if (iter_ == begin_) {
        iter_ = end_;

        return *this;
      }
      --iter_;

      return *this;
    }

    iterator operator--(int) {
      iterator new_iter = *this;
      --new_iter;

      return new_iter;
    }

    iterator& operator--() {
      if (iter_ == end_) {
        iter_ = begin_;

        return *this;
      }
      ++iter_;

      return *this;
    }

    iterator& operator+=(difference_type d) requires SubstractableDType<Container> {
      iter_ -= d;

      return *this;
    }

    iterator& operator-=(difference_type d) requires AddableDType<Container> {
      iter_ += d;

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
    typename Container::iterator begin_;
    typename Container::iterator end_;
  };

  iterator begin() {
    return iterator(--my_container_.end(), my_container_.begin(), my_container_.end());
  }

  iterator end() {
    return iterator(my_container_.end(), my_container_.begin(), my_container_.end());
  }

private:
  Container my_container_;
};

template<SimpleContainer container>
Reverse<container> reverse(const container& my_container) {
  return Reverse<container>(my_container);
}

template<SimpleContainer container>
Reverse<container> operator|(container my_container, MyReverse reverse) {
  return Reverse<container>(my_container);
}
