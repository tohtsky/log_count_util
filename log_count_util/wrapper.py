from datetime import timedelta

import numpy as np

from ._core import find_last_record_index as find_last_record_index_core
from ._core import find_n_record_before as find_n_record_before_core
from ._core import find_records_within_interval as find_records_within_interval_core
from ._core import sum_records_within_interval as sum_records_within_interval_core


def datetime_array_to_int(arr: np.ndarray) -> np.ndarray:
    return arr.astype("datetime64[ns]").astype(np.int64)


def find_n_records_within_interval(
    query_ids: np.ndarray,
    query_datetime: np.ndarray,
    target_ids: np.ndarray,
    target_datetime: np.ndarray,
    interval: timedelta,
) -> np.ndarray:
    query_datetime_ns = datetime_array_to_int(query_datetime)
    target_datetime_ns = datetime_array_to_int(target_datetime)
    interval_in_ns = int(interval.total_seconds() * 10 ** 9)
    return find_records_within_interval_core(
        query_ids, query_datetime_ns, target_ids, target_datetime_ns, interval_in_ns
    )


def sum_records_within_interval(
    query_ids: np.ndarray,
    query_datetime: np.ndarray,
    target_ids: np.ndarray,
    target_datetime: np.ndarray,
    target_values: np.ndarray,
    interval: timedelta,
) -> np.ndarray:
    query_datetime_ns = datetime_array_to_int(query_datetime)
    target_datetime_ns = datetime_array_to_int(target_datetime)
    interval_in_ns = int(interval.total_seconds() * 10 ** 9)
    return sum_records_within_interval_core(
        query_ids,
        query_datetime_ns,
        target_ids,
        target_datetime_ns,
        target_values.astype(np.float64),
        interval_in_ns,
    )


def find_n_records_before(
    query_ids: np.ndarray,
    query_datetime: np.ndarray,
    target_ids: np.ndarray,
    target_datetime: np.ndarray,
) -> np.ndarray:
    query_datetime_ns = datetime_array_to_int(query_datetime)
    target_datetime_ns = datetime_array_to_int(target_datetime)
    return find_n_record_before_core(
        query_ids,
        query_datetime_ns,
        target_ids,
        target_datetime_ns,
    )


def find_last_record_index(
    query_ids: np.ndarray,
    query_datetime: np.ndarray,
    target_ids: np.ndarray,
    target_datetime: np.ndarray,
) -> np.ndarray:
    query_datetime_ns = datetime_array_to_int(query_datetime)
    target_datetime_ns = datetime_array_to_int(target_datetime)
    return find_last_record_index_core(
        query_ids,
        query_datetime_ns,
        target_ids,
        target_datetime_ns,
    )
