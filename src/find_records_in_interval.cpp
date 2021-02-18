#include <algorithm>
#include <bits/stdint-intn.h>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <stdexcept>
#include <tuple>

#include "find_records_in_interval.hpp"

namespace py = pybind11;

template <typename IDType>
inline void
find_co_indptr(py::array_t<IDType> query_ids, py::array_t<IDType> target_ids,
               std::function<void(size_t, size_t, size_t, size_t)> callback) {

  const auto query_id_info = query_ids.request(false);
  auto query_ptr = static_cast<const IDType *>(query_id_info.ptr);

  const auto target_id_info = target_ids.request(false);
  auto target_ptr = static_cast<const IDType *>(target_id_info.ptr);

  const auto query_size = static_cast<size_t>(query_id_info.size);
  const auto target_size = static_cast<size_t>(target_id_info.size);
  find_co_indptr_imple(query_ptr, query_size, target_ptr, target_size,
                       callback);
}

template <typename IDType, typename Scalar>
py::array_t<int64_t> find_records_within_interval_within_query(
    py::array_t<Scalar> query_ids, py::array_t<int64_t> query_datetime,
    py::array_t<Scalar> target_ids, py::array_t<int64_t> target_datetime,
    int64_t days) {
  const auto query_id_size = query_ids.request(false).size;
  const auto target_id_size = target_ids.request(false).size;

  const auto &query_datetime_info = query_datetime.request(false);
  const auto &target_datetime_info = target_datetime.request(false);

  if (query_id_size != query_datetime_info.size) {
    throw std::invalid_argument(
        "query id and query datetime have different shapes.");
  }
  if (target_id_size != target_datetime_info.size) {
    throw std::invalid_argument(
        "target id and target datetime have different shapes.");
  }

  auto query_ptr = static_cast<const Scalar *>(query_datetime_info.ptr);
  auto target_ptr = static_cast<const Scalar *>(target_datetime_info.ptr);

  const size_t result_size = query_datetime_info.size;
  py::array_t<std::int64_t> result(result_size);

  auto result_ptr = static_cast<int64_t *>(result.request(true).ptr);

  auto callback = [query_ptr, target_ptr, result_ptr,
                   days](size_t query_index_start, size_t query_index_end,
                         size_t target_index_start, size_t target_index_end) {
    find_records_within_interval_with_query(
        query_ptr + query_index_start, (query_index_end - query_index_start),
        target_ptr + target_index_start,
        (target_index_end - target_index_start), days,
        result_ptr + query_index_start);
  };
  find_co_indptr(query_ids, target_ids, callback);
  return result;
}

template <typename Scalar, typename IDType>
py::array_t<int64_t> sum_records_within_interval(
    py::array_t<IDType> query_ids, py::array_t<int64_t> query_datetime,
    py::array_t<IDType> target_ids, py::array_t<int64_t> target_datetime,
    py::array_t<Scalar> target_values, int64_t days) {
  const auto query_id_size = query_ids.request(false).size;
  const auto target_id_size = target_ids.request(false).size;

  const auto &query_datetime_info = query_datetime.request(false);
  const auto &target_datetime_info = target_datetime.request(false);
  const auto &target_value_info = target_values.request(false);

  if (query_id_size != query_datetime_info.size) {
    throw std::invalid_argument(
        "query id and query datetime have different shapes.");
  }
  if (target_id_size != target_datetime_info.size) {
    throw std::invalid_argument(
        "target id and target datetime have different shapes.");
  }
  if (target_id_size != target_value_info.size) {
    throw std::invalid_argument(
        "target id and target values have different shapes.");
  }

  auto query_ptr = static_cast<const IDType *>(query_datetime_info.ptr);
  auto target_ptr = static_cast<const IDType *>(target_datetime_info.ptr);
  auto target_value_ptr = static_cast<const Scalar *>(target_value_info.ptr);

  const size_t result_size = query_datetime_info.size;
  py::array_t<Scalar> result(result_size);

  auto result_ptr = static_cast<Scalar *>(result.request(true).ptr);

  auto callback = [query_ptr, target_ptr, result_ptr, days, target_value_ptr](
                      size_t query_index_start, size_t query_index_end,
                      size_t target_index_start, size_t target_index_end) {
    sum_records_within_interval_with_query<IDType, Scalar>(
        query_ptr + query_index_start, (query_index_end - query_index_start),
        target_ptr + target_index_start, target_value_ptr + target_index_start,
        (target_index_end - target_index_start), days,
        result_ptr + query_index_start);
  };
  find_co_indptr(query_ids, target_ids, callback);
  return result;
}

template <typename IDType, typename Scalar>
py::array_t<int64_t> find_n_record_before(
    py::array_t<Scalar> query_ids, py::array_t<int64_t> query_datetime,
    py::array_t<Scalar> target_ids, py::array_t<int64_t> target_datetime) {
  const auto query_id_size = query_ids.request(false).size;
  const auto target_id_size = target_ids.request(false).size;

  const auto &query_datetime_info = query_datetime.request(false);
  const auto &target_datetime_info = target_datetime.request(false);

  if (query_id_size != query_datetime_info.size) {
    throw std::invalid_argument(
        "query id and query datetime have different shapes.");
  }
  if (target_id_size != target_datetime_info.size) {
    throw std::invalid_argument(
        "target id and target datetime have different shapes.");
  }

  auto query_ptr = static_cast<const Scalar *>(query_datetime_info.ptr);
  auto target_ptr = static_cast<const Scalar *>(target_datetime_info.ptr);

  const size_t result_size = query_datetime_info.size;
  py::array_t<std::int64_t> result(result_size);

  auto result_ptr = static_cast<int64_t *>(result.request(true).ptr);

  auto callback = [query_ptr, target_ptr, result_ptr](
                      size_t query_index_start, size_t query_index_end,
                      size_t target_index_start, size_t target_index_end) {
    find_records_before_query(query_ptr + query_index_start,
                              (query_index_end - query_index_start),
                              target_ptr + target_index_start,
                              (target_index_end - target_index_start),
                              result_ptr + query_index_start);
  };
  find_co_indptr(query_ids, target_ids, callback);
  return result;
}

template <typename IDType, typename Scalar>
py::array_t<int64_t> find_last_record_index(
    py::array_t<Scalar> query_ids, py::array_t<int64_t> query_datetime,
    py::array_t<Scalar> target_ids, py::array_t<int64_t> target_datetime) {
  const auto query_id_size = query_ids.request(false).size;
  const auto target_id_size = target_ids.request(false).size;

  const auto &query_datetime_info = query_datetime.request(false);
  const auto &target_datetime_info = target_datetime.request(false);

  if (query_id_size != query_datetime_info.size) {
    throw std::invalid_argument(
        "query id and query datetime have different shapes.");
  }
  if (target_id_size != target_datetime_info.size) {
    throw std::invalid_argument(
        "target id and target datetime have different shapes.");
  }

  auto query_ptr = static_cast<const Scalar *>(query_datetime_info.ptr);
  auto target_ptr = static_cast<const Scalar *>(target_datetime_info.ptr);

  const size_t result_size = query_datetime_info.size;
  py::array_t<std::int64_t> result(result_size);

  auto result_ptr = static_cast<int64_t *>(result.request(true).ptr);

  auto callback = [query_ptr, target_ptr, result_ptr](
                      size_t query_index_start, size_t query_index_end,
                      size_t target_index_start, size_t target_index_end) {
    find_last_record_index_with_query(
        query_ptr + query_index_start, (query_index_end - query_index_start),
        target_ptr + target_index_start,
        (target_index_end - target_index_start), target_index_start,
        result_ptr + query_index_start);
  };
  find_co_indptr(query_ids, target_ids, callback);
  return result;
}

PYBIND11_MODULE(_core, m) {
  m.def("find_records_within_interval",
        &find_records_within_interval_within_query<int64_t, int64_t>,
        py::arg("query_ids"), py::arg("query_datetime"), py::arg("target_id"),
        py::arg("target_datetime"), py::arg("days"));
  m.def("sum_records_within_interval",
        &sum_records_within_interval<double, int64_t>, py::arg("query_ids"),
        py::arg("query_datetime"), py::arg("target_id"),
        py::arg("target_datetime"), py::arg("target_values"), py::arg("days"));
  m.def("find_last_record_index", &find_last_record_index<int64_t, int64_t>,
        py::arg("query_ids"), py::arg("query_datetime"), py::arg("target_id"),
        py::arg("target_datetime"));
  m.def("find_n_record_before", &find_n_record_before<int64_t, int64_t>,
        py::arg("query_ids"), py::arg("query_datetime"), py::arg("target_id"),
        py::arg("target_datetime"));
};
