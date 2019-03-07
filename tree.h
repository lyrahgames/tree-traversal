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

template <typename T, bool Constant = true>
class traversor {
 public:
  using node_type = node<T>;
  using value_type = T;
  using difference_type = long;
  using reference =
      std::conditional_t<Constant, const value_type&, value_type&>;
  using pointer = std::conditional_t<Constant, const value_type*, value_type*>;
  using node_pointer =
      std::conditional_t<Constant, const node_type*, node_type*>;

  traversor() = delete;  // There should be no default constructor.
  traversor(node_pointer n) : node_{n} {}
  traversor(const traversor& it) = default;
  traversor& operator=(const traversor& it) = default;
  traversor(traversor&& it) = default;
  traversor& operator=(traversor&& it) = default;
  ~traversor() = default;

  traversor& traverse_left() {
    node_ = node_->left_.get();
    return *this;
  }
  traversor left() { return node_->left_.get(); }
  traversor& traverse_right() {
    node_ = node_->right_.get();
    return *this;
  }
  traversor right() { return node_->right_.get(); }
  reference operator*() const { return node_->data_; }
  pointer operator->() const { return &node_->data; }
  bool operator==(traversor it) const { return node_ == it.node_; }
  bool operator!=(traversor it) const { return !(*this == it); }
  bool is_leaf() const { return node_->left_ && node_->right_; }
  bool has_child() const { return node_->left || node_->right_; }
  operator bool() const { return node_ != nullptr; }

 private:
  node_pointer node_;
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