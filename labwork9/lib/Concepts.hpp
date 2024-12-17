#pragma once

#include <concepts>
#include <iostream>
#include <map>
#include <ranges>
#include <vector>

typedef std::ptrdiff_t difference_type;
typedef std::size_t size_type;

template <typename T>
concept Iterator = std::default_initializable<T> && requires(T t, T y) {
  { ++t } -> std::same_as<T&>;
  *t;
  t == y;
  t != y;
};

template<typename T>
concept SimpleContainer = requires(T a) {
  requires Iterator<typename T::iterator>;
  { a.begin() } -> std::same_as<decltype(a.end())>;
};

template<typename T>
concept AssociativeContainer = requires(T a) {
  requires SimpleContainer<T>;
  T::key_comp;
  T::value_comp;
};

template<typename Container>
concept Substractable = requires(typename Container::iterator t) {{ --t } -> std::same_as<typename Container::iterator&>;};

template<typename Container>
concept AddableDType = requires(typename Container::iterator t, difference_type d) {{ t += d } -> std::same_as<typename Container::iterator&>;};

template<typename Container>
concept SubstractableDType = requires(typename Container::iterator t, difference_type d) {{ t -= d } -> std::same_as<typename Container::iterator&>;};

template<typename Container>
concept AddableIter = requires(typename Container::iterator t1, typename Container::iterator t2) {{ t1 += t2 } -> std::same_as<typename Container::iterator&>;};

template<typename Container>
concept SubstractableIter = requires(typename Container::iterator t1, typename Container::iterator t2) {{ t1 -= t2 } -> std::same_as<typename Container::iterator&>;};

template<typename T>
concept ClassOperation = requires(T t) { T::operation_; };

template<typename T>
concept ClassPredicate = requires(T t) { T::predicate_; };

template<typename container>
concept PairKeys = requires {typename container::value_type::first_type; typename container::value_type::second_type;};