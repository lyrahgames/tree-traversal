#ifndef TREE_H_
#define TREE_H_

#include <memory>
#include <random>

template <typename T>
class node {
 public:
  node() = default;
  node(const T& x) : data_{x} {}

  T data_{};
  std::unique_ptr<node<T>> left_{};
  std::unique_ptr<node<T>> right_{};
};

template <typename T>
class iterator {
  iterator() = delete;
  iterator(node<T>* pointer) : pointer_{pointer} {};

 private:
  node<T>* pointer_;
};

template <typename Node, typename RNG, typename Distribution>
struct random_tree_maker {
  Distribution distribution{};
  RNG rng{std::random_device{}()};

  std::unique_ptr<Node> operator()(int max_depth) {
    if (max_depth <= 0) return {};

    auto root = std::make_unique<Node>(distribution(rng));

    std::uniform_real_distribution<float> dist{};
    if (dist(rng) <= 0.8) root->left_ = (*this)(max_depth - 1);
    if (dist(rng) <= 0.8) root->right_ = (*this)(max_depth - 1);

    // root->left_ = (*this)(max_depth - 1);
    // root->right_ = (*this)(max_depth - 1);

    return root;
  }
};

#endif  // TREE_H_