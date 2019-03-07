#ifndef CONSOLE_DISPLAY_BUFFER_H_
#define CONSOLE_DISPLAY_BUFFER_H_

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "counting_sort.h"

struct console_display_buffer {
  struct element {
    element() = default;
    template <typename T>
    element(int r, int c, T&& t)
        : row{r}, column{c}, text{std::forward<T>(t)} {}
    int row;
    int column;
    std::string text;
  };

  std::vector<element> data;

  template <typename... Args>
  console_display_buffer& emplace(Args&&... args) {
    data.emplace_back(std::forward<Args>(args)...);
    return *this;
  }
};

inline std::ostream& operator<<(std::ostream& os,
                                console_display_buffer& buffer) {
  // auto tmp = buffer.data;
  decltype(buffer.data) tmp(buffer.data.size());
  counting_sort(begin(buffer.data), end(buffer.data), begin(tmp),
                [](const auto& e) { return e.column; });
  counting_sort(begin(tmp), end(tmp), begin(buffer.data),
                [](const auto& e) { return e.row; });

  auto row = 0;
  auto column = 0;
  for (const auto& e : buffer.data) {
    if (row != e.row) {
      os << std::string(e.row - row, '\n');
      row = e.row;
      column = 0;
    }
    const auto tmp = e.column - column;
    os << std::string((tmp < 0) ? (0) : (tmp), ' ') << e.text;
    column = e.column + size(e.text);
  }
  return os << '\n';
}

struct wconsole_display_buffer {
  struct element {
    element() = default;
    template <typename T>
    element(int r, int c, T&& t)
        : row{r}, column{c}, text{std::forward<T>(t)} {}
    int row;
    int column;
    std::wstring text;
  };

  std::vector<element> data;

  template <typename... Args>
  wconsole_display_buffer& emplace(Args&&... args) {
    data.emplace_back(std::forward<Args>(args)...);
    return *this;
  }
};

inline std::wostream& operator<<(std::wostream& os,
                                 wconsole_display_buffer& buffer) {
  // auto tmp = buffer.data;
  decltype(buffer.data) tmp(buffer.data.size());
  counting_sort(begin(buffer.data), end(buffer.data), begin(tmp),
                [](const auto& e) { return e.column; });
  counting_sort(begin(tmp), end(tmp), begin(buffer.data),
                [](const auto& e) { return e.row; });

  auto row = 0;
  auto column = 0;
  for (const auto& e : buffer.data) {
    if (row != e.row) {
      os << std::wstring(e.row - row, '\n');
      row = e.row;
      column = 0;
    }
    const auto tmp = e.column - column;
    os << std::wstring((tmp < 0) ? (0) : (tmp), ' ') << e.text;
    column = e.column + size(e.text);
  }
  return os << '\n';
}

#endif  // CONSOLE_DISPLAY_BUFFER_H_