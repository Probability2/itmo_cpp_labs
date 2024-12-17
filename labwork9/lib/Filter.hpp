#pragma once

#include "Concepts.hpp"
#include "Functions.hpp"

template<SimpleContainer Container, typename Predicate>
class Filter {
public:
  Filter() = default;

  Filter(const Container& my_container, const Predicate& pred)
  : my_container_(my_container)
  , predicate_(pred) {
  }

  Filter(const Predicate& pred)
  : predicate_(pred) {
  }

  class iterator {
  public:
    iterator() = default;

    iterator(const typename Container::iterator& iter, const Predicate& predicate,
             const typename Container::iterator& begin, const typename Container::iterator& end)
    : iter_(iter)
    , predicate_(predicate)
    , begin_(begin)
    , end_(end) {
    }

    auto operator*() const {
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
      while (iter_ != end_ && !predicate_(*iter_)) {
        ++iter_;
      }

      return *this;
    }

    iterator operator--(int) requires Substractable<Container> {
      iterator new_iter = *this;
      --new_iter;

      return new_iter;
    }

    iterator& operator--() requires Substractable<Container> {
      --iter_;
      while (iter_ != begin_ && !predicate_(*iter_)) {
        --iter_;
      }

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
    typename Container::iterator begin_;
    typename Container::iterator end_;
    Predicate predicate_;
  };

  iterator begin() {
    return iterator(my_container_.begin(), predicate_, my_container_.begin(), my_container_.end());
  }

  iterator end() {
    return iterator(my_container_.end(), predicate_, my_container_.begin(), my_container_.end());
  }

private:
  Container my_container_;
  Predicate predicate_;
};

template<SimpleContainer container, typename Predicate>
Filter<container, Predicate> filter(const container& my_container, const Predicate& predicate) {
  return Filter<container, Predicate>(my_container, predicate);
}

template<SimpleContainer container, ClassPredicate MyFilter>
Filter<container, typename MyFilter::Predicate_> operator|(container my_container, MyFilter mfilter) {
  return Filter<container, typename MyFilter::Predicate_>(my_container, mfilter.predicate_);
}
