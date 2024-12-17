#pragma once

#include "Node.hpp"

template<typename T>
struct Stack {
  T key;
  std::size_t size_ = 0;
  struct Stack* next;
  struct Stack(T key)
  : key(key)
  , next(nullptr) {
  }
};

template<typename T>
std::pair<Stack<T>*, Stack<T>*> Add(T key, Stack<T>* top_stack, Stack<T>* head) {
  head = new Stack(key);
  head->key = key;
  head->next = top_stack;
  top_stack = head;

  return std::make_pair(head, top_stack);
}

template<typename T>
std::pair<Stack<T>*, Stack<T>*> Pop(Stack<T>* top_stack, Stack<T>* head) {
  Stack<T>* pointer = head->next;
  head = head->next;
  Stack<T>* top_stack_copy = top_stack;
  delete top_stack_copy;
  top_stack = pointer;

  return std::make_pair(head, top_stack);
}
