#ifndef COUNTING_SORT_H_
#define COUNTING_SORT_H_

#include <algorithm>

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

#endif  // COUNTING_SORT_H_