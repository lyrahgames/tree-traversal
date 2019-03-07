#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

template <typename ForwardIt, typename RandomIt, typename KeyFunction>
void counting_sort(
    ForwardIt first, ForwardIt last, RandomIt out, KeyFunction key,
    decltype(std::declval<KeyFunction>()(*std::declval<ForwardIt>())) min_hint,
    decltype(min_hint) max_hint) {
  const auto offset = min_hint;
  const auto size = max_hint - min_hint + 1;

  int counts[size]{};  // this int is an independent implementation detail
  for (auto it = first; it != last; ++it) ++counts[key(*it) - offset];

  auto sum = counts[0];
  counts[0] = 0;
  for (int i = 1; i < static_cast<int>(size); ++i) {
    auto tmp = sum;
    sum += counts[i];
    counts[i] = tmp;
  }

  for (auto it = first; it != last; ++it)
    out[counts[key(*it) - offset]++] = *it;
}

template <typename ForwardIt, typename RandomIt, typename KeyFunction>
void counting_sort(ForwardIt first, ForwardIt last, RandomIt out,
                   KeyFunction key = {}) {
  auto it = first;
  auto min = key(*it);
  auto max = key(*it);
  for (; it != last; ++it) {
    max = std::max(max, key(*it));
    min = std::min(min, key(*it));
  }

  counting_sort(first, last, out, key, min, max);
}

struct console_display_buffer {
  struct element {
    element() = default;
    element(int r, int c, const string& t) : row{r}, column{c}, text{t} {}
    element(int r, int c, string&& t) : row{r}, column{c}, text{move(t)} {}
    int row;
    int column;
    string text;
  };

  vector<element> data;

  template <typename... Args>
  console_display_buffer& emplace(Args&&... args) {
    data.emplace_back(std::forward<Args>(args)...);
    return *this;
  }
};

ostream& operator<<(ostream& os, const console_display_buffer& buffer) {
  auto tmp = buffer.data;
  counting_sort(begin(buffer.data), end(buffer.data), begin(tmp),
                [](const auto& e) { return e.row; });

  auto row = 0;
  auto column = 0;
  for (const auto& e : tmp) {
    if (row != e.row) {
      os << string(e.row - row, '\n');
      row = e.row;
      column = 0;
    }
    os << string(e.column, ' ') << e.text;
    column = e.column + size(e.text);
  }
  return os << '\n';
}

int main() {
  console_display_buffer buffer;

  buffer.emplace(0, 0, "----")
      .emplace(4, 0, "----")
      .emplace(2, 2, "xxx")
      .emplace(1, 1, "hello");

  cout << buffer;
}