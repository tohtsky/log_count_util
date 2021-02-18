from datetime import timedelta
from io import StringIO

import numpy as np
import pandas as pd

from log_count_util import find_n_records_within_interval, sum_records_within_interval

target_dataframe = pd.read_csv(
    StringIO(
        """user_id,timestamp,value_column
0,2021-02-18T09:59:59,4.0
0,2021-02-18T10:00:10,2.0
0,2021-02-18T10:00:20,1.0
0,2021-02-18T10:00:20,0.5
0,2021-02-18T10:00:20,0.25
0,2021-02-18T11:00:20,0.125
"""
    ),
    parse_dates=["timestamp"],
)

query_dataframe = pd.read_csv(
    StringIO(
        """user_id,timestamp,value_column
0,2021-02-18T10:00:00,100.0
0,2021-02-18T10:00:10,10.0
0,2021-02-18T10:00:21,1.0
0,2021-02-18T11:00:21,0.1
1,2020-02-18T10:00:10,100.0
1,2020-02-18T10:00:20,10.0
1,2020-02-18T10:00:20,1.0
1,2020-02-18T10:00:29,0.0
"""
    ),
    parse_dates=["timestamp"],
)


def test_query() -> None:
    arr = find_n_records_within_interval(
        query_dataframe.user_id,
        query_dataframe.timestamp,
        query_dataframe.user_id,
        query_dataframe.timestamp,
        timedelta(seconds=10),
    )
    np.testing.assert_array_equal(arr, [0, 1, 0, 0, 0, 1, 1, 2])
    arr = find_n_records_within_interval(
        query_dataframe.user_id,
        query_dataframe.timestamp,
        target_dataframe.user_id,
        target_dataframe.timestamp,
        timedelta(seconds=10),
    )
    np.testing.assert_array_equal(arr, [1, 0, 3, 1, 0, 0, 0, 0])


def test_sum() -> None:
    arr = sum_records_within_interval(
        query_dataframe.user_id,
        query_dataframe.timestamp,
        query_dataframe.user_id,
        query_dataframe.timestamp,
        query_dataframe.value_column.values,
        timedelta(seconds=10),
    )
    np.testing.assert_array_equal(arr, [0, 100, 0, 0, 0, 100, 100, 11])

    arr = sum_records_within_interval(
        query_dataframe.user_id,
        query_dataframe.timestamp,
        target_dataframe.user_id,
        target_dataframe.timestamp,
        target_dataframe.value_column,
        timedelta(seconds=10),
    )
    np.testing.assert_array_equal(arr, [4, 0, 1.75, 0.125, 0, 0, 0, 0])
