# log-count-utils

Suppose we have an action log data `df` like

| user_id | timestamp           | price |
| ------: | :------------------ | ----: |
|       0 | 2021-02-18 10:00:00 |   100 |
|       0 | 2021-02-18 10:00:10 |    10 |
|       0 | 2021-02-18 10:00:21 |     1 |
|       0 | 2021-02-18 11:00:21 |   0.1 |
|       1 | 2020-02-18 10:00:10 |   100 |
|       1 | 2020-02-18 10:00:20 |    10 |
|       1 | 2020-02-18 10:00:20 |     1 |
|       1 | 2020-02-18 10:00:29 |     0 |

I often encounter a situation where I have to compute a number of actions each user has taken within 10 seconds **for each row in this data frame**.

A naive way,

```python
from datetime import timedelta
import numpy as np
td = timedelta(seconds=10)

answers = []
for uid, timepoint in zip(df.user_id, df.timestamp):
    cnt = np.sum(
        df[ (df.user_id == uid) & (df.timestamp < timepoint) & (df.timestamp >= (timepoint - td))]
    )
```

is fine for this tiny example but becomes costly (O(N^2)) for large data frame.

If `df` is sorted (by `user_id` as primary and `timestamp` as secondary key),
we can do this blazing fast (O(N)) using `log_count_util`.

```python
from log_count_util import find_n_records_within_interval

answers = find_n_records_within_interval(
    df.user_id, df.timestamp, df_user_id, df.timestamp, td
)
```
