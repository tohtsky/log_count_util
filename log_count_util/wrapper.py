from datetime import timedelta

import numpy as np
from numpy import typing as npt

from ._core import (
    find_last_record_index,
    find_last_record_index_f,
    find_n_record_before,
    find_records_within_interval,
)


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
    return find_records_within_interval(
        query_ids, query_datetime_ns, target_ids, target_datetime_ns, interval_in_ns
    )