#pragma once

#include "Concepts.hpp"

template<typename Operation>
struct MyTransform {
  typedef Operation Operation_;
  Operation operation_;
  MyTransform(const Operation& operation)
  : operation_(operation) {
  }
};

template<typename Predicate>
struct MyFilter {
  typedef Predicate Predicate_;
  Predicate predicate_;
  MyFilter(const Predicate& predicate)
  : predicate_(predicate) {
  }
};

struct ShiftFunction {
  difference_type shift_;
  ShiftFunction(const difference_type d)
  : shift_(d) {
  }
};

struct MyTake: public ShiftFunction {
  MyTake(const difference_type d)
  : ShiftFunction(d) {
  }
};

struct MyDrop: public ShiftFunction {
  MyDrop(const difference_type d)
  : ShiftFunction(d) {
  }
};

struct MyReverse {};

struct MyKeys {};

struct MyValues {};