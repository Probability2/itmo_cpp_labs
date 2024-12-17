#pragma once

#include "Concepts.hpp"
#include "Functions.hpp"

template<SimpleContainer Container>
class Drop {
public:
  typedef bool range_tag;
  Drop() = default;

  Drop(Container my_container, const difference_type d)
  : my_container_(my_container)
  , shift_(d) {
  }

  Drop& operator=(const Drop& take) {
    my_container_ = take.my_container_;
    shift_ = take.shift_;

    return *this;
  }

  class iterator {
  public:

    iterator() = default;

    iterator(const typename Container::iterator& iter,
             const typename Container::iterator& begin, const typename Container::iterator& end)
    : iter_(iter)
    , begin_(begin)
    , end_(end) {
    }

    iterator(const iterator& iter)
    : iter_(iter.iter_)
    , begin_(iter.begin_)
    , end_(iter.end_) {
    }

    iterator& operator=(const iterator& iter) {
      iter_ = iter.iter_;
      begin_ = iter.begin_;
      end_ = iter.end_;

      return *this;
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

      return new_iter;
    }

    iterator& operator++() {
      if (iter_ != end_) {
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
      if (iter_ != begin_) {
        --iter_;
      }

      return *this;
    }

    iterator& operator+=(difference_type d) requires AddableDType<Container> {
      typename Container::iterator next_iter = iter_;
      next_iter += d;
      if (next_iter != end_ && next_iter != begin_) {
        ++iter_;
      }

      return *this;
    }

    iterator& operator-=(difference_type d) requires SubstractableDType<Container> {
      typename Container::iterator next_iter = iter_;
      next_iter -= d;
      if (next_iter != end_ && next_iter != begin_) {
        ++iter_;
      }

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
    auto curr_begin = my_container_.begin();
    size_type i = shift_;
    while (curr_begin != my_container_.end() && i--) {
      ++curr_begin;
    }

    return iterator(curr_begin, curr_begin, my_container_.end());
  }

  iterator end() {
    auto curr_begin = my_container_.begin();
    size_type i = shift_;
    while (curr_begin != my_container_.end() && i--) {
      ++curr_begin;
    }
    return iterator(--my_container_.end(), curr_begin, my_container_.end());
  }

private:
  Container my_container_;
  difference_type shift_;
};

template<SimpleContainer container>
Drop<container> drop(const container& my_container, const difference_type shift) {
  return Drop<container>(my_container, shift);
}

template<SimpleContainer container>
Drop<container> operator|(container my_container, MyDrop drop_) {
  return Drop<container>(my_container, drop_.shift_);
}
