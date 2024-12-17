#pragma once

#include "Iterator.hpp"
#include "Reverse_Iterator.hpp"
#include "Node.hpp"
#include "Stack.hpp"
#include "TagsOrder.hpp"

static const std::size_t kMaxSize = 1000;

namespace std {

template<typename T, typename Order, typename Compare = std::less<T>, typename Allocator = std::allocator<Node<T>>>
class BST {
public:
  typedef T value_type;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T* iterator;
  typedef const T* const_iterator;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  std::pair<Iterator<T, Order>, bool> insert(const T value) {
    if (contains(value)) {
      return std::make_pair(Iterator<T, Order>(end_), false);
    }
    ++size_;
    Node<T>* node = std::allocator_traits<decltype(allocator_)>::allocate(allocator_, 1);
    std::allocator_traits<decltype(allocator_)>::construct(allocator_, node, Node<T>(value));
    data_ = Insert(data_, node);
    SetBegin(node, my_traits<Order>::tag());
    SetEnd(node, my_traits<Order>::tag());

    return std::make_pair(find(value), true);
  }

  BST() = default;

  BST(const BST& bst)
  : data_(bst.data_)
  , size_(bst.size_)
  , begin_(bst.begin_)
  , end_(bst.end_)
  , first_root_(bst.first_root_)
  , last_root_(bst.last_root_)
  , allocator_(bst.allocator_)
  , compare_(bst.compare_) {
  }

  BST(const Compare& compare) {
    compare_ = compare;
  }

  size_type size() const noexcept {
    return size_;
  }

  size_type max_size() const noexcept {
    return kMaxSize;
  }

  void clear() {
    size_ = 0;
    ClearData(data_);
    allocator_.deallocate(end_, 1);
    allocator_.deallocate(begin_, 1);
  }

  Reverse_iterator<T, Order> rbegin() const noexcept {
    return Reverse_iterator<T, Order>(last_root_);
  }

  const Reverse_iterator<T, Order> crbegin() const noexcept {
    return const_cast<const Reverse_iterator<T, Order>&>(rbegin());
  }

  Reverse_iterator<T, Order> rend() const noexcept {
    return Reverse_iterator<T, Order>(begin_);
  }

  const Reverse_iterator<T, Order> crend() const noexcept {
    return const_cast<const Reverse_iterator<T, Order>&>(rbegin());
  }

  void insert(Iterator<T, Order> first, Iterator<T, Order> second) {
    while (first != second && first) {
      insert(*first);
      ++first;
    }
  }

  void insert(const std::initializer_list<T>& l) {
    for (T val: l) {
      insert(val);
    }
  }

  void insert(const T key, const Iterator<T, Order>& iter) {
    Insert(key);
  }

  Iterator<T, Order> find(const T value) const {
    Node<T>* el = Find(data_, value);
    if (el && el != begin_) {
      return Iterator<T, Order>(el); 
    }

    return Iterator<T, Order>(end_);
  }

  bool contains(const T key) const {
    return find(key).GetData() != end_;
  }

  BST(Iterator<T, Order> first, Iterator<T, Order> second) {
    insert(first, second);
  }

  BST(Iterator<T, Order> first, Iterator<T, Order> second, const Compare& compare): BST(first, second) {
    compare_ = compare;
  }

  BST(const std::initializer_list<T>& l) {
    insert(l);
  }

  BST(std::initializer_list<T> l, const Compare& compare): BST(l) {
    compare_ = compare;
  }

  Iterator<T, Order> begin() const noexcept {
    return Iterator<T, Order>(first_root_);
  }

  const Iterator<T, Order> cbegin() const noexcept {
    return const_cast<const Iterator<T, Order>&>(begin());
  }

  Iterator<T, Order> end() const noexcept {
    return Iterator<T, Order>(end_);
  }

  const Iterator<T, Order> cend() const noexcept {
    return const_cast<const Iterator<T, Order>&>(end());
  }

  Iterator<T, Order> lower_bound(const T key) const noexcept {
    return Iterator<T, Order>(Find_lb(data_, end_, key, my_traits<Order>::tag()));
  }

  Iterator<T, Order> upper_bound(const T key) const noexcept {
    return Iterator<T, Order>(Find_ub(data_, end_, key, my_traits<Order>::tag()));
  }

  size_type erase(const T key) {
    return Erase(key, data_);
  }

  Iterator<T, Order> erase(const Iterator<T, Order>& iter) {
    if (!iter) {
      std::cerr << "ERROR:\nIncorrect reference to iterator\n";
      return iter;
    }

    return Erase_node(*iter, data_);
  }

  Iterator<T, Order> erase(Iterator<T, Order>& iter) {
    if (!iter) {
      std::cerr << "ERROR:\nIncorrect reference to iterator\n";
      return iter;
    }

    return Erase_node(*iter, data_);
  }

  Iterator<T, Order> erase(const Iterator<T, Order>& first, const Iterator<T, Order>& second) {
    return Iterator<T, Order>(StackUnwinding(AddToStack(first, second)));
  }

  std::pair<Iterator<T, Order>, Iterator<T, Order>> equal_range(const T key) const {
    return std::make_pair(lower_bound(key), upper_bound(key));
  }

  size_type count(const T k) const {
    size_type count = 0;
    Count_element(data_, k, count);

    return count;
  }

  bool empty() const noexcept {
    return !size_;
  }

  bool operator!() const noexcept {
    return (data_ != nullptr);
  }

  bool operator==(const BST& bst) const {
    if (size_ != bst.size_) {
      return false;
    }
    Iterator<T, Order> iter1(this->first_root_);
    Iterator<T, Order> iter2(bst.first_root_);
    for (size_type i = 0; i < size_; ++i) {
      if (iter1 != iter2) {
        return false;
      }
      ++iter1;
      ++iter2;
    }

    return true;
  }

  bool operator!=(const BST& bst) const {
    return !(*this == bst);
  }

  void swap(BST& bst) {
    SwapBegins(bst);
    SwapSizes(bst);
    SwapEnds(bst);
    SwapFirstRoots(bst);
    SwapLastRoots(bst);
    SwapDates(bst);
    SwapAllocators(bst);
  }

  BST& operator=(const BST& bst) {
    if (this != &bst) {
      this->data_ = bst.data_;
      this->end_ = bst.end_;
      this->begin_ = bst.begin_;
      this->last_root_ = bst.last_root_;
      this->compare_ = bst.compare_;
      this->size_ = bst.size_;
      this->first_root_ = bst.first_root_;
      this->allocator_ = bst.allocator_;
    }

    return *this;
  }

  Allocator get_allocator() const noexcept {
    return allocator_;
  }

  Compare key_comp() const noexcept {
    return compare_;
  }

  Compare value_comp() const noexcept {
    return compare_;
  }

  Node<T>* GetData() const noexcept {
    return data_;
  }

  void merge(const BST& bst) {
    Merge(bst.data_, bst);
  }

  void print() const {
    std::cout << *this;
  }

  void Print(std::ostream& stream, Node<T>* data, const tag_inorder& tag) const {
    if (!data || data == end_ || data == begin_) {
      return;
    }
    Print(stream, data->left, tag);
    stream << data->key;
    stream << ' ';
    Print(stream, data->right, tag);
  }

  void Print(std::ostream& stream, Node<T>* data, const tag_preorder& t) const {
    if (!data || data == end_ || data == begin_) {
      return;
    }
    stream << data->key;
    stream << ' ';
    Print(stream, data->left, t);
    Print(stream, data->right, t);
  }

  void Print(std::ostream& stream, Node<T>* data, const tag_postorder& t) const {
    if (!data || data == end_ || data == begin_) {
      return;
    }
    Print(stream, data->left, t);
    Print(stream, data->right, t);
    stream << data->key;
    stream << ' ';
  }

private:
  Node<T>* data_ = nullptr;
  Node<T>* begin_ = nullptr;
  Node<T>* end_ = nullptr;
  Node<T>* last_root_ = nullptr;
  Node<T>* first_root_ = nullptr;
  size_type size_ = 0;
  Allocator allocator_;
  Compare compare_;

  Node<T>* Insert(Node<T>* head, Node<T>* curr_value) {
    if (!head || head == end_ || head == begin_) {
      return curr_value;
    }
    if (compare_(curr_value->key, head->key)) {
      Node<T>* ans = head;
      head->left = Insert(head->left, curr_value);
      head->left->prev = ans;
    } else {
      Node<T>* ans = head;
      head->right = Insert(head->right, curr_value);
      head->right->prev = ans;
    }
    
    return head;
  }

  void Count_element(Node<T>* data, const T key, size_type& count) const {
    if (!data || data == end_ || data == begin_) {
      return;
    }
    count = data->key == key ? count + 1 : count;
    Count_element(data->left, key, count);
    Count_element(data->right, key, count);
  }

  void ClearData(Node<T>* data) {
    if (!data || data == begin_ || data == end_) {
      return;
    }
    ClearData(data->left);
    ClearData(data->right);
    allocator_.deallocate(data, 1);
  }

  Node<T>* Find(Node<T>* data, T value) const {
    if (!data || data == end_ || data == begin_) {
      return data;
    }
    if (data->key == value) {
      return data;
    }
    if (compare_(data->key, value)) {
      return Find(data->right, value);
    } else {
      return Find(data->left, value);
    }
  }

  void SetFirstNullNode(Node<T>* node) {
    begin_ = std::allocator_traits<decltype(allocator_)>::allocate(allocator_, 1);
    SetFirstRoot(node);
  }

  inline void CheckNullBegin() {
    begin_ = !begin_ ? std::allocator_traits<decltype(allocator_)>::allocate(allocator_, 1) : begin_;
  }

  inline void CheckNullEnd() {
    end_ = !end_ ? std::allocator_traits<decltype(allocator_)>::allocate(allocator_, 1) : end_;
  }

  inline void SetFirstRoot(Node<T>* node) {
    first_root_ = node;
    begin_->prev = first_root_;
    begin_->left = nullptr;
    begin_->right = nullptr;
    first_root_->left = begin_;
  }

  void SetFirstNode(Node<T>* node, const tag_inorder&) {
    if (!first_root_) {
      SetFirstNullNode(node);
      return;
    }
    CheckNullBegin();
    if (compare_(node->key, first_root_->key)) {
      SetFirstRoot(node);
    }
  }

  Node<T>* GoThroughLeft(Node<T>* root) const {
    while (root && root->left) {
      if (root->left == begin_ || root->left == end_) {
        break;
      }
      root = root->left;
    }

    return root;
  }

  Node<T>* GoThroughRight(Node<T>* root) const {
    while (root && root->right) {
      if (root->right == begin_ || root->right == end_) {
        break;
      }
      root = root->right;
    }

    return root;
  }

  void FindFirstRoot() {
    Node<T>* data = data_;
    while (data && (data->left || data->right)) {
      if (data->left && (data->left != begin_)) {
        data = data->left;
      } else if (data->right && (data->right != end_)) {
        data = data->right;
      } else {
        break;
      }
    }

    SetFirstRoot(data);
  }

  void ClearEnd() {
    if (end_->prev) {
      end_->prev->left = end_->prev->left == end_ ? nullptr : end_->prev->left;
      end_->prev->right = end_->prev->right == end_ ? nullptr : end_->prev->right;
    }
    last_root_ = nullptr;
  }

  void FindLastRoot() {
    Node<T>* data = data_;
    while (data && (data->left || data->right)) {
      if (data->right && (data->right != end_)) {
        data = data->right;
      } else if (data->left && (data->left != begin_)) {
        data = data->left;
      } else {
        break;
      }
    }
    CheckNullEnd();
    ClearEnd();
    SetLastRoot(data);
  }

  void SetFirstNode(Node<T>* node, const tag_postorder&) {
    if (!first_root_) {
      SetFirstNullNode(node);
      return;
    }
    CheckNullBegin();
    FindFirstRoot();
  }

  void SetBegin(Node<T>* node, const tag_inorder&) {
    SetFirstNode(node, my_traits<Order>::tag());
  }

  void SetBegin(Node<T>* node, const tag_preorder&) {
    if (!first_root_) {
      first_root_ = data_;
      begin_ = std::allocator_traits<decltype(allocator_)>::allocate(allocator_, 1);
      begin_->left = first_root_;
      begin_->right = nullptr;
      begin_->prev = nullptr;
      first_root_->prev = begin_;
    }
  }

  void SetBegin(Node<T>* node, const tag_postorder&) {
    SetFirstNode(node, my_traits<Order>::tag());
  }

  inline void SetNullLastNode(Node<T>* node) {
    last_root_ = node;
    end_ = std::allocator_traits<decltype(allocator_)>::allocate(allocator_, 1);
    end_->prev = last_root_;
    end_->left = nullptr;
    end_->right = nullptr;
    last_root_->right = end_;
  }

  inline void SetLastRoot(Node<T>* node) {
    last_root_ = node;
    end_->prev = last_root_;
    end_->left = nullptr;
    end_->right = nullptr;
    last_root_->right = end_;
  }

  void SetLastNode(Node<T>* node, const tag_inorder&) {
    if (!last_root_) {
      SetNullLastNode(node);
      return;
    }
    CheckNullEnd();
    if (compare_(last_root_->key, node->key)) {
      SetLastRoot(node);
    }
  }

  void SetLastNode(Node<T>* node, const tag_preorder&) {
    if (!last_root_) {
      SetNullLastNode(node);
      return;
    }
    FindLastRoot();
  }

  void SetEnd(Node<T>* node, const tag_inorder&) {
    SetLastNode(node, my_traits<Order>::tag());
  }

  void SetEnd(Node<T>* node, const tag_preorder&) {
    SetLastNode(node, my_traits<Order>::tag());
  }

  void SetEnd(Node<T>* node, const tag_postorder&) {
    if (!last_root_) {
      last_root_ = data_;
      end_ = std::allocator_traits<decltype(allocator_)>::allocate(allocator_, 1);
      end_->left = last_root_;
      end_->right = nullptr;
      end_->prev = nullptr;
      last_root_->prev = end_;
    }
  }

  Node<T>* Find_lb(Node<T>* data, Node<T>* alter_root, const T key, const tag_inorder& tag) const {
    if (!data || data == end_ || data == begin_) {
      return alter_root;
    }
    if (compare_(key, data->key) || key == data->key) {
      alter_root = data;
      return Find_lb(data->left, alter_root, key, tag);
    } else {
      return Find_lb(data->right, alter_root, key, tag);
    }
  }

  Node<T>* Find_lb(Node<T>* data, Node<T>* alter_root, const T key, const tag_preorder& tag) const {
    if (!data || data == end_ || data == begin_) {
      return alter_root;
    }
    if (compare_(key, data->key) || key == data->key) {
      return data;
    } else {
      return Find_lb(data->right, alter_root, key, tag);
    }
  }

  Node<T>* Find_lb(Node<T>* data, Node<T>* alter_root, const T key, const tag_postorder& tag) const {
    if (!data || data == end_ || data == begin_) {
      return alter_root;
    }
    if (compare_(key, data->key) || key == data->key) {
      if (data->right) {
        alter_root = GoThroughLeft(data->right);
      } else {
        alter_root = data;
      }
      return Find_lb(data->left, alter_root, key, tag);
    } else {
      return Find_lb(data->right, alter_root, key, tag);
    }
  }

  Node<T>* Find_ub(Node<T>* data, Node<T>* alter_root, const T key, const tag_inorder& tag) const {
    if (!data || data == end_ || data == begin_) {
      return alter_root;
    }
    if (compare_(key, data->key)) {
      alter_root = data;
      return Find_ub(data->left, alter_root, key, tag);
    } else {
      return Find_ub(data->right, alter_root, key, tag);
    }
  }

  Node<T>* Find_ub(Node<T>* data, Node<T>* alter_root, const T key, const tag_preorder& tag) const {
    if (!data || data == end_ || data == begin_) {
      return alter_root;
    }
    if (compare_(key, data->key)) {
      return data;
    } else {
      return Find_ub(data->right, alter_root, key, tag);
    }
  }

  Node<T>* Find_ub(Node<T>* data, Node<T>* alter_root, const T key, const tag_postorder& tag) const {
    if (!data || data == end_ || data == begin_) {
      return alter_root;
    }
    if (compare_(key, data->key)) {
      if (data->right) {
        alter_root = GoThroughLeft(data->right);
      } else {
        alter_root = data;
      }
      return Find_lb(data->left, alter_root, key, tag);
    } else {
      return Find_lb(data->right, alter_root, key, tag);
    }
  }

  void Merge(Node<T>* data, const BST& bst) {
    if (!data || data == bst.end_ || data == bst.begin_) {
      return;
    }
    insert(data->key);
    Merge(data->left, bst);
    Merge(data->right, bst);
  }

  size_type Erase(const T key, Node<T>* data) {
    if (!data || data == end_ || data == begin_) {
      return 0;
    }
    if (data->key == key) {
      --size_;
      Delete_root(data);
      return 1;
    }
    if (compare_(data->key, key)) {
      return Erase(key, data->right);
    } else {
      return Erase(key, data->left);
    }
  }

  Node<T>* Erase_node(const T key, Node<T>* data) {
    if (!data || data == end_ || data == begin_) {
      return end_;
    }
    if (data->key == key) {
      --size_;
      Iterator<T, Order> iter(data);
      ++iter;
      Delete_root(data);
      return iter.GetData();
    }
    if (compare_(data->key, key)) {
      return Erase_node(key, data->right);
    } else {
      return Erase_node(key, data->left);
    }
  }

  void Delete_root_one_child(Node<T>* data) {
    if (data->left) {
      PullOutRoot(data, data->left);
      SetRootParameters(data->left, data->key, my_traits<Order>::tag());
      return;
    }
    PullOutRoot(data, data->right);
    SetRootParameters(data->right, data->key, my_traits<Order>::tag());
  }

  void Delete_root_no_childs(Node<T>* data) {
    if (data->prev) {
      if (data->prev->right == data) {
        data->prev->right = nullptr;
      } else {
        data->prev->left = nullptr;
      }
    }
    SetRootParameters(data->prev, data->key, my_traits<Order>::tag());
  }

  void Delete_root_two_childs(Node<T>* data) {
    Node<T>* lchild = data->right;
    lchild = GoThroughLeft(lchild);
    ChangeParents(data, lchild);
    DiscardParents(lchild);
    if (data->prev) {
      ChangeChilds(data, lchild);
      lchild->prev = data->prev;
    }
    if (data->left && data->left->key != lchild->key) {
      lchild->left = data->left;
    }
    if (data->right && data->right->key != lchild->key) {
      lchild->right = data->right;
    }
    SetRootParameters(lchild, data->key, my_traits<Order>::tag());
  }

  void Delete_root(Node<T>* data) {
    T left = GoThroughLeft(data_)->key;
    T right = GoThroughRight(data_)->key;
    if ((data->key == left && !data->right) || (data->key == right && !data->left) || (!data->left && !data->right)) {
      Delete_root_no_childs(data);
    } else if ((data->key == left && data->right) || (data->key == right && data->left)
            || (!data->left && data->right) || (data->left && !data->right)) {
      Delete_root_one_child(data);
    } else {
      Delete_root_two_childs(data);
    }
    allocator_.deallocate(data, 1);
  }

  void PullOutRoot(Node<T>* data, Node<T>* child) {
    if (data->prev) {
      if (data->prev->left == data) {
        data->prev->left = child;
      } else {
        data->prev->right = child;
      }
      child->prev = data->prev;
    } else {
      child->prev = nullptr;
    }
  }

  void SetRootParameters(Node<T>* node, const T root_key, const tag_inorder&) {
    if (data_->key == root_key) {
      data_ = node;
    }
    SetFirstRoot(GoThroughLeft(data_));
    SetLastRoot(GoThroughRight(data_));
  }

  void SetRootParameters(Node<T>* node, const T root_key, const tag_preorder& tag) {
    if (data_->key == root_key) {
      data_ = node;
    }
    first_root_ = nullptr;
    SetBegin(node, tag);
    FindLastRoot();
  }

  void SetRootParameters(Node<T>* node, const T root_key, const tag_postorder& tag) {
    if (data_->key == root_key) {
      data_ = node;
      SetEnd(node, tag);
    }
    if (first_root_->key == root_key) {
      FindFirstRoot();
    }
  }

  void SwapBegins(BST& bst) {
    Node<T>* node = this->begin_;
    this->begin_ = bst.begin_;
    bst.begin_ = node;
  }

  void SwapEnds(BST& bst) {
    Node<T>* node = this->end_;
    this->end_ = bst.end_;
    bst.end_ = node;
  }

  void SwapFirstRoots(BST& bst) {
    Node<T>* node = this->first_root_;
    this->first_root_ = bst.first_root_;
    bst.first_root_ = node;
  }

  void SwapLastRoots(BST& bst) {
    Node<T>* node = this->last_root_;
    this->last_root_ = bst.last_root_;
    bst.last_root_ = node;
  }

  void SwapDates(BST& bst) {
    Node<T>* node = this->data_;
    this->data_ = bst.data_;
    bst.data_ = node;
  }

  void SwapSizes(BST& bst) {
    size_type sz_copy = this->size_;
    this->size_ = bst.size_;
    bst.size_ = sz_copy;
  }

  void SwapAllocators(BST& bst) {
    std::allocator<Node<T>> alloc_copy = this->allocator_;
    this->allocator_ = bst.allocator_;
    bst.allocator_ = alloc_copy;
  }

  void DiscardParents(Node<T>* root) {
    if (root->prev->left == root) {
      root->prev->left = nullptr;
    }
    if (root->prev->right == root) {
      root->prev->right = nullptr;
    }
  }

  void ChangeParents(Node<T>* root, Node<T>* new_parent) {
    if (root->left->prev == root && root->left != new_parent) {
      root->left->prev = new_parent;
    }
    if (root->right->prev == root && root->right != new_parent) {
      root->right->prev = new_parent;
    }
  }

  void ChangeChilds(Node<T>* data, Node<T>* child) {
    if (data->prev->left == data) {
      data->prev->left = child;
    } else {
      data->prev->right = child;
    }
  }

  std::pair<Stack<T>*, Stack<T>*> AddToStack(Iterator<T, Order> first, const Iterator<T, Order>& second) {
    Stack<T>* stack = nullptr;
    Stack<T>* top_stack = nullptr;
    while (*first != *second) {
      auto pair = Add(*first, top_stack, stack);
      stack = pair.first;
      top_stack = pair.second;
      ++first;
    }

    return std::make_pair(stack, top_stack);
  }

  Node<T>* StackUnwinding(std::pair<Stack<T>*, Stack<T>*> stack_pair) {
    Stack<T>* stack = stack_pair.first;
    Stack<T>* top_stack = stack_pair.second;
    Node<T>* ans = end_;
    while (stack) {
      ans = Erase_node(stack->key, data_);
      auto pair = Pop(top_stack, stack);
      stack = pair.first;
      top_stack = pair.second;
    }

    return ans;
  }
};

template<typename T, typename Order>
void swap(BST<T, Order>& first_bst, BST<T, Order>& second_bst) {
  first_bst.swap(second_bst);
}

template<typename T, typename Order>
std::ostream& operator<<(std::ostream& stream, const BST<T, Order>& bst) {
  if (bst.empty()) {
    stream << "ERROR:\nThe BST is empty\n";
    return stream;
  }
  bst.Print(stream, bst.GetData(), my_traits<Order>::tag());

  return stream;
}

};
