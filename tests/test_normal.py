from datetime import timedelta
from io import StringIO

import numpy as np
import pandas as pd
import pytest

from log_count_util import (
    find_last_record_index,
    find_n_records_before,
    find_n_records_within_interval,
    sum_records_within_interval,
)

target_dataframe_small = pd.read_csv(
    StringIO(
        """user_id,timestamp,value_column
0, 2021-02-18T09:59:59, 4.0
0, 2021-02-18T10:00:10, 2.0
0, 2021-02-18T10:00:20, 1.0
0, 2021-02-18T10:00:20, 0.5
0, 2021-02-18T10:00:20, 0.25
0, 2021-02-18T11:00:20, 0.125
"""
    ),
    parse_dates=["timestamp"],
)

query_dataframe_small = pd.read_csv(
    StringIO(
        """user_id,timestamp,value_column
0, 2021-02-18T10:00:00, 100.0
0, 2021-02-18T10:00:10, 10.0
0, 2021-02-18T10:00:21, 1.0
0, 2021-02-18T11:00:21, 0.1
1, 2020-02-18T10:00:10, 100.0
1, 2020-02-18T10:00:20, 10.0
1, 2020-02-18T10:00:20, 1.0
1, 2020-02-18T10:00:29, 0.0
"""
    ),
    parse_dates=["timestamp"],
)


@pytest.mark.parametrize(
    "query_dataframe, target_dataframe",
    [
        (query_dataframe_small, query_dataframe_small),
        (query_dataframe_small, target_dataframe_small),
    ],
)
def test_query(query_dataframe: pd.DataFrame, target_dataframe: pd.DataFrame) -> None:
    td = timedelta(seconds=10)
    arr = find_n_records_within_interval(
        query_dataframe.user_id,
        query_dataframe.timestamp,
        target_dataframe.user_id,
        target_dataframe.timestamp,
        td,
    )
    for i, row in enumerate(query_dataframe.itertuples()):
        time = row.timestamp
        uid = row.user_id
        answer = (
            (target_dataframe.user_id == uid)
            & (target_dataframe.timestamp < time)
            & (target_dataframe.timestamp >= (time - td))
        ).sum()
        assert answer == arr[i]


@pytest.mark.parametrize(
    "query_dataframe, target_dataframe",
    [
        (query_dataframe_small, query_dataframe_small),
        (query_dataframe_small, target_dataframe_small),
    ],
)
def test_sum(query_dataframe: pd.DataFrame, target_dataframe: pd.DataFrame) -> None:
    td = timedelta(seconds=10)
    arr = sum_records_within_interval(
        query_dataframe.user_id,
        query_dataframe.timestamp,
        target_dataframe.user_id,
        target_dataframe.timestamp,
        target_dataframe.value_column.values,
        timedelta(seconds=10),
    )

    for i, row in enumerate(query_dataframe.itertuples()):
        time = row.timestamp
        uid = row.user_id
        answer = target_dataframe.value_column[
            (
                (target_dataframe.user_id == uid)
                & (target_dataframe.timestamp < time)
                & (target_dataframe.timestamp >= (time - td))
            )
        ].sum()
        assert answer == arr[i]


@pytest.mark.parametrize(
    "query_dataframe, target_dataframe",
    [
        (query_dataframe_small, query_dataframe_small),
        (query_dataframe_small, target_dataframe_small),
    ],
)
def test_find_records_before(
    query_dataframe: pd.DataFrame, target_dataframe: pd.DataFrame
) -> None:
    arr = find_n_records_before(
        query_dataframe.user_id,
        query_dataframe.timestamp,
        target_dataframe.user_id,
        target_dataframe.timestamp,
    )

    for i, row in enumerate(query_dataframe.itertuples()):
        time = row.timestamp
        uid = row.user_id
        answer = (
            (target_dataframe.user_id == uid) & (target_dataframe.timestamp < time)
        ).sum()
        assert answer == arr[i]


@pytest.mark.parametrize(
    "query_dataframe, target_dataframe",
    [
        (query_dataframe_small, query_dataframe_small),
        (query_dataframe_small, target_dataframe_small),
    ],
)
def test_find_last_records_index(
    query_dataframe: pd.DataFrame, target_dataframe: pd.DataFrame
) -> None:
    arr = find_last_record_index(
        query_dataframe.user_id,
        query_dataframe.timestamp,
        target_dataframe.user_id,
        target_dataframe.timestamp,
    )

    for i, row in enumerate(query_dataframe.itertuples()):
        time = row.timestamp
        uid = row.user_id
        candidate_index = np.where(
            (
                (target_dataframe.user_id == uid) & (target_dataframe.timestamp < time)
            ).values
        )[0]
        if candidate_index.shape[0] == 0:
            assert arr[i] == -1
        else:
            assert candidate_index[-1] == arr[i]
