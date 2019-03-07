#include <algorithm>
#include <sstream>

#include "console_display_buffer.h"
#include "tree.h"

struct info {
  int position;
  int end;
};

template <typename Node>
info print(console_display_buffer& buffer, const std::unique_ptr<Node>& root,
           int depth, int offset) {
  if (!root) return {offset, offset};

  const auto left_child = print(buffer, root->left_, depth + 1, offset);
  offset = left_child.end;
  const auto label = std::to_string(root->data_);
  const auto position = offset + size(label) / 2;
  buffer.emplace(2 * depth, offset, label);
  offset += std::size(label);
  const auto right_child = print(buffer, root->right_, depth + 1, offset);

  // printing edges
  if (root->left_) {
    buffer  //
        .emplace(2 * depth + 1, left_child.position, '_')
        .emplace(2 * depth + 1, left_child.position + 1,
                 std::string(position - left_child.position - 1, '_'));
  }
  if (root->left_ || root->right_) buffer.emplace(2 * depth + 1, position, '|');
  if (root->right_) {
    buffer  //
        .emplace(2 * depth + 1, position + 1,
                 std::string(right_child.position - position - 1, '_'))
        .emplace(2 * depth + 1, right_child.position, '_');
  }

  return {position, right_child.end};
}

template <typename T>
std::ostream& operator<<(std::ostream& os,
                         const std::unique_ptr<node<T>>& root) {
  console_display_buffer buffer{};
  const auto tmp = print(buffer, root, 0, 0);
  return os << buffer;
}

template <typename Node>
info print(wconsole_display_buffer& buffer, const std::unique_ptr<Node>& root,
           int depth, int offset) {
  if (!root) return {offset, offset};

  const auto left_child = print(buffer, root->left_, depth + 1, offset);
  offset = left_child.end;
  const auto label = std::to_wstring(root->data_);
  const auto position = offset + size(label) / 2;
  buffer.emplace(2 * depth, offset, label);
  offset += std::size(label);
  const auto right_child = print(buffer, root->right_, depth + 1, offset);

  // printing edges
  if (root->left_) {
    buffer  //
        .emplace(2 * depth + 1, left_child.position, L'\x250C')
        .emplace(2 * depth + 1, left_child.position + 1,
                 std::wstring(position - left_child.position - 1, L'\x2500'));
  }
  // if (root->left_ || root->right_) buffer.emplace(2 * depth + 1, position,
  // '|');
  if (root->left_ && root->right_)
    buffer.emplace(2 * depth + 1, position, L'\x2534');
  else if (root->left_)
    buffer.emplace(2 * depth + 1, position, L'\x2518');
  else if (root->right_)
    buffer.emplace(2 * depth + 1, position, L'\x2514');

  if (root->right_) {
    buffer  //
        .emplace(2 * depth + 1, position + 1,
                 std::wstring(right_child.position - position - 1, L'\x2500'))
        .emplace(2 * depth + 1, right_child.position, L'\x2510');
  }

  return {position, right_child.end};
}

template <typename T>
std::wostream& operator<<(std::wostream& os,
                          const std::unique_ptr<node<T>>& root) {
  wconsole_display_buffer buffer{};
  const auto tmp = print(buffer, root, 0, 0);
  return os << buffer;
}

template <typename Traversor>
info print(wconsole_display_buffer& buffer, Traversor root, int depth,
           int offset) {
  if (!root) return {offset, offset};

  const auto left_child = print(buffer, root.left(), depth + 1, offset);
  offset = left_child.end;
  const auto label = std::to_wstring(*root);
  const auto position = offset + size(label) / 2;
  buffer.emplace(2 * depth, offset, label);
  offset += std::size(label);
  const auto right_child = print(buffer, root.right(), depth + 1, offset);

  // printing edges
  if (root.left()) {
    buffer  //
        .emplace(2 * depth + 1, left_child.position, L'\x250C')
        .emplace(2 * depth + 1, left_child.position + 1,
                 std::wstring(position - left_child.position - 1, L'\x2500'));
  }

  if (root.left() && root.right())
    buffer.emplace(2 * depth + 1, position, L'\x2534');
  else if (root.left())
    buffer.emplace(2 * depth + 1, position, L'\x2518');
  else if (root.right())
    buffer.emplace(2 * depth + 1, position, L'\x2514');

  if (root.right()) {
    buffer  //
        .emplace(2 * depth + 1, position + 1,
                 std::wstring(right_child.position - position - 1, L'\x2500'))
        .emplace(2 * depth + 1, right_child.position, L'\x2510');
  }

  return {position, right_child.end};
}

template <typename Traversor>
std::wostream& operator<<(std::wostream& os, Traversor root) {
  wconsole_display_buffer buffer{};
  const auto tmp = print(buffer, root, 0, 0);
  return os << buffer;
}

int main(int argc, char** argv) {
  std::setlocale(LC_ALL, "");

  if (2 != argc) return -1;
  std::stringstream input{argv[1]};
  int n;
  input >> n;

  random_tree_maker<node<float>, std::mt19937,
                    std::uniform_real_distribution<float>>
      rtm{std::uniform_real_distribution<float>{-100, 100}};
  auto root = rtm(n);
  std::wcout << traversor<float>{root.get()};
}