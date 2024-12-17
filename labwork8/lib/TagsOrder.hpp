#pragma once

struct tag_inorder {};
struct tag_preorder {};
struct tag_postorder {};

struct Inorder {};
struct Preorder {};
struct Postorder {};

template<typename MyType>
struct my_traits {
  typedef tag_inorder tag;
};

template<>
struct my_traits<Preorder> {
  typedef tag_preorder tag;
};

template<>
struct my_traits<Postorder> {
  typedef tag_postorder tag;
};
