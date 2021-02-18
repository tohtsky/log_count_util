#pragma once

#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>

template <typename Scalar>
inline void assert_sorted(const Scalar *data, size_t size) {
  if (size == 0)
    return;
  Scalar previous = data[0];
  Scalar next;
  for (size_t i = 1; i < size; i++) {
    next = data[i];
    if (previous > next) {
      throw std::runtime_error("not sorted");
    }
    previous = next;
  }
}

template <typename Scalar>
inline void find_records_within_interval_with_query(
    const Scalar *query_ptr, const std::int64_t query_size,
    const Scalar *target_ptr, const std::int64_t target_size,
    const Scalar &days, int64_t *target_cnt) {
  assert_sorted(query_ptr, query_size);
  assert_sorted(target_ptr, target_size);

  auto t_upper_examine_ptr = target_ptr + (target_size - 1);
  auto t_lower_examine_ptr = target_ptr + (target_size - 1);

  for (int64_t i = query_size - 1; i >= 0; i--) {
    const Scalar t_upper = query_ptr[i];
    const Scalar t_lower = query_ptr[i] - days;
    if (t_lower_examine_ptr >= target_ptr) {
      while (true) {
        if ((*t_lower_examine_ptr) < t_lower) {
          break;
        } else {
          t_lower_examine_ptr--;
          if (t_lower_examine_ptr < target_ptr) {
            break;
          }
        }
      }
    }
    if (t_upper_examine_ptr >= target_ptr) {
      while (true) {
        if ((*t_upper_examine_ptr) < t_upper) {
          break;
        } else {
          t_upper_examine_ptr--;
          if (t_upper_examine_ptr < target_ptr) {
            break;
          }
        }
      }
    }
    target_cnt[i] =
        static_cast<int64_t>(t_upper_examine_ptr - t_lower_examine_ptr);
  }
}

template <typename Scalar, typename ValueType>
inline void sum_records_within_interval_with_query(
    const Scalar *query_ptr, const std::int64_t query_size,
    const Scalar *target_ptr, const ValueType *target_values,
    const std::int64_t target_size, const Scalar &days, ValueType *output_ptr) {
  assert_sorted(query_ptr, query_size);
  assert_sorted(target_ptr, target_size);

  int64_t L = target_size - 1;
  int64_t U = target_size - 1;
  ValueType val_plus = 0;
  ValueType val_minus = 0;
  const ValueType *target_values_plus = target_values + L;
  const ValueType *target_values_minus = target_values + U;

  for (int64_t i = query_size - 1; i >= 0; i--) {
    const Scalar t_upper = query_ptr[i];
    const Scalar t_lower = query_ptr[i] - days;
    if (L >= 0) {
      while (true) {
        if (target_ptr[L] < t_lower) {
          break;
        } else {
          val_plus += *(target_values_plus--);
          --L;
          if (L < 0) {
            break;
          }
        }
      }
    }
    if (U >= 0) {
      while (true) {
        if (target_ptr[U] < t_upper) {
          break;
        } else {
          val_minus += *(target_values_minus--);
          --U;
          if (U < 0) {
            break;
          }
        }
      }
    }
    output_ptr[i] = val_plus - val_minus;
  }
}

template <typename Scalar>
inline void find_last_record_index_with_query(const Scalar *query_ptr,
                                              const std::int64_t query_size,
                                              const Scalar *target_ptr,
                                              const std::int64_t target_size,
                                              size_t offset,
                                              int64_t *target_cnt) {
  assert_sorted(query_ptr, query_size);
  assert_sorted(target_ptr, target_size);

  int64_t U = target_size - 1;
  for (int64_t i = query_size - 1; i >= 0; i--) {
    const Scalar t_upper = query_ptr[i];
    if (U >= 0) {
      while (true) {
        if (target_ptr[U] < t_upper) {
          break;
        } else {
          --U;
          if (U < 0) {
            break;
          }
        }
      }
    }
    if (U >= 0) {
      target_cnt[i] = U + offset;
    } else {
      target_cnt[i] = -1;
    }
  }
}

template <typename Scalar>
inline void find_records_before_query(const Scalar *query_ptr,
                                      const std::int64_t query_size,
                                      const Scalar *target_ptr,
                                      const std::int64_t target_size,
                                      int64_t *target_cnt) {
  assert_sorted(query_ptr, query_size);
  assert_sorted(target_ptr, target_size);

  int64_t U = target_size - 1;
  for (int64_t i = query_size - 1; i >= 0; i--) {
    const Scalar t_upper = query_ptr[i];
    if (U >= 0) {
      while (true) {
        if (target_ptr[U] < t_upper) {
          break;
        } else {
          --U;
          if (U < 0) {
            break;
          }
        }
      }
    }
    target_cnt[i] = U + 1;
  }
}

template <typename IDType>
inline void find_constant_index_range(int64_t &upper, int64_t &lower,
                                      const IDType *data, const IDType &query) {
  while (true) {
    if (upper == 0)
      break;

    if (data[upper - 1] <= query) {
      break;
    }
    // here, data[upper - 1] > query, so upper -1 still a valid candidate for U.
    upper--;
  }
  lower = upper; // as data[upper] > q
  while (true) {
    lower--;
    if (lower < 0) // data[-1] = -infinity < query
      break;
    if (data[lower] < query)
      break;
  }
}

template <typename IDType>
inline void find_co_indptr_imple(
    const IDType *query_ptr, const size_t query_size, const IDType *target_ptr,
    const size_t target_size,
    std::function<void(size_t, size_t, size_t, size_t)> callback) {

  assert_sorted(query_ptr, query_size);
  assert_sorted(target_ptr, target_size);

  if (query_size == 0) {
    return;
  }

  int64_t upper_query = query_size;
  int64_t lower_query = query_size;

  int64_t upper_target = target_size;
  int64_t lower_target = target_size;
  int64_t cursor = query_size - 1;
  while (true) {
    const IDType q = query_ptr[cursor];
    find_constant_index_range(upper_query, lower_query, query_ptr, q);
    find_constant_index_range(upper_target, lower_target, target_ptr, q);
    cursor = lower_query;

    callback(lower_query + 1, upper_query, lower_target + 1, upper_target);
    if (cursor < 0)
      break;
  }
}
