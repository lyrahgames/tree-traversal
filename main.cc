#include <iostream>
#include <locale>
#include <memory>
#include <random>
#include <string>

using namespace std;

template <typename T>
class node {
 public:
  node() = default;
  node(const T& x) : data_{x} {}

  T data_{};
  unique_ptr<node<T>> left_{};
  unique_ptr<node<T>> right_{};
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
  RNG rng{random_device{}()};

  unique_ptr<Node> operator()(int max_depth) {
    if (max_depth <= 0) return {};

    auto root = make_unique<Node>(distribution(rng));

    uniform_real_distribution<float> dist{};
    if (dist(rng) <= 0.8) root->left_ = (*this)(max_depth - 1);
    if (dist(rng) <= 0.8) root->right_ = (*this)(max_depth - 1);

    // root->left_ = (*this)(max_depth - 1);
    // root->right_ = (*this)(max_depth - 1);

    return root;
  }
};

template <typename Node>
struct tree_cache {
  int depth{};
  int size{};
  int current_depth{};

  tree_cache(const unique_ptr<Node>& root) { compute(root); }

  void compute(const unique_ptr<Node>& root) {
    if (!root) return;
    ++size;
    ++current_depth;
    depth = max(depth, current_depth);

    compute(root->left_);
    compute(root->right_);

    --current_depth;
  }
};

class display_buffer {
 public:
  display_buffer(int width, int height)
      : width_{width}, height_{height}, buffer_((width + 1) * height, ' ') {
    for (int i = 0; i < height; ++i) buffer_[(width + 1) * i + width] = '\n';
  }

  auto width() const noexcept { return width_; }
  auto height() const noexcept { return height_; }
  auto size() const noexcept { return width() * height(); }

  auto& operator()(int i, int j) noexcept {
    return buffer_[(width() + 1) * j + i];
  }
  const auto& operator()(int i, int j) const noexcept {
    return buffer_[(width() + 1) * j + i];
  }

  const auto& string() const noexcept { return buffer_; }

 private:
  int width_;
  int height_;
  std::string buffer_;
};

ostream& operator<<(ostream& os, const display_buffer& buffer) {
  return os << buffer.string();
}

template <typename Node>
struct tree_printer {
  display_buffer buffer;
  int size{};
  int depth{};

  tree_printer(const unique_ptr<Node>& root, int max_size, int max_depth)
      : buffer{max_size, 2 * max_depth - 1} {
    print(root);
  }

  int print(const unique_ptr<Node>& root) {
    if (!root) return -1;
    ++depth;
    const int left_child_pos = print(root->left_);

    buffer(size, 2 * (depth - 1)) = to_string(root->data_)[0];
    // const auto label = to_string(root->data_);
    // for (int i = 0; i < std::size(label); ++i) {
    //   buffer(size - std::size(label) / 2 + i, 2 * (depth - 1)) = label[i];
    // }
    const int root_pos = size;
    ++size;
    // size += std::size(label);

    const int right_child_pos = print(root->right_);
    --depth;

    // edges
    if (left_child_pos > -1) {
      buffer(left_child_pos, 2 * depth + 1) = '_';
      for (int i = left_child_pos + 1; i < root_pos; ++i) {
        buffer(i, 2 * depth + 1) = '_';
      }
    }

    if (left_child_pos > -1 || right_child_pos > -1)
      buffer(root_pos, 2 * depth + 1) = '|';

    if (right_child_pos > -1) {
      buffer(right_child_pos, 2 * depth + 1) = '_';
      for (int i = root_pos + 1; i < right_child_pos; ++i) {
        buffer(i, 2 * depth + 1) = '_';
      }
    }

    return root_pos;
  }
};

// template <typename Node>
// struct tree_printer2 {
//   vector<vector<pair<int, char>>> lines;
//   int size{};
//   int depth{};

//   tree_printer2(const unique_ptr<Node>& root, int max_size, int max_depth)
//       : buffer{max_size, 2 * max_depth - 1} {
//     print(root);
//   }

//   struct info {
//     int root_position;
//     int new_offset;
//   };

//   info print(const unique_ptr<Node>& root, int depth, int offset) {
//     if (depth >= lines.size()) lines.resize(depth + 1);

//     if (root->left_) {
//       const auto tmp = print(root->left_, depth + 1, offset);
//       lines[index + 1].push_back({tmp.root_position, '_'});
//       for (int i = tmp.root_position + 1; i < tmp.offset; ++i)
//         lines[index + 1][i] = '_';
//     }

//     const auto label = to_string(root->data_);
//     for (int i = 0; i < std::size(label); ++i) {
//       buffer(size - std::size(label) / 2 + i, 2 * (depth - 1)) = label[i];
//     }
//     const int root_pos = size;
//     size += std::size(label);
//     if (left_child_pos > -1 || right_child_pos > -1)
//       buffer(root_pos, 2 * depth + 1) = '|';

//     if (root->right_) {
//       print(root->right_, depth + 1);
//       buffer(right_child_pos, 2 * depth + 1) = '_';
//       for (int i = root_pos + 1; i < right_child_pos; ++i) {
//         buffer(i, 2 * depth + 1) = '_';
//       }
//     }

//     return root_pos;
//   }
// };

template <typename T>
ostream& operator<<(ostream& os, const unique_ptr<node<T>>& root) {
  tree_cache cache{root};
  tree_printer printer{root, cache.size, cache.depth};
  return os << printer.buffer;
}

int main() {
  // setlocale(LC_ALL, "");
  random_tree_maker<node<int>, mt19937, uniform_int_distribution<int>> rtm{
      uniform_int_distribution<int>{0, 1000}};
  auto root = rtm(8);
  cout << root << '\n';
}