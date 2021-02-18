m: int
n: int
from numpy import float64, int64

import log_count_util._core
import typing
import numpy

_Shape = typing.Tuple[int, ...]

__all__ = [
    "find_last_record_index",
    "find_n_record_before",
    "find_records_within_interval",
    "sum_records_within_interval",
]

def find_last_record_index(
    query_ids: numpy.ndarray[numpy.int64],
    query_datetime: numpy.ndarray[numpy.int64],
    target_id: numpy.ndarray[numpy.int64],
    target_datetime: numpy.ndarray[numpy.int64],
) -> numpy.ndarray[numpy.int64]:
    pass

def find_n_record_before(
    query_ids: numpy.ndarray[numpy.int64],
    query_datetime: numpy.ndarray[numpy.int64],
    target_id: numpy.ndarray[numpy.int64],
    target_datetime: numpy.ndarray[numpy.int64],
) -> numpy.ndarray[numpy.int64]:
    pass

def find_records_within_interval(
    query_ids: numpy.ndarray[numpy.int64],
    query_datetime: numpy.ndarray[numpy.int64],
    target_id: numpy.ndarray[numpy.int64],
    target_datetime: numpy.ndarray[numpy.int64],
    days: int,
) -> numpy.ndarray[numpy.int64]:
    pass

def sum_records_within_interval(
    query_ids: numpy.ndarray[numpy.int64],
    query_datetime: numpy.ndarray[numpy.int64],
    target_id: numpy.ndarray[numpy.int64],
    target_datetime: numpy.ndarray[numpy.int64],
    target_values: numpy.ndarray[numpy.int64],
    days: int,
) -> numpy.ndarray[numpy.int64]:
    pass
