# log-count-utils

Suppose we have an action log data `df` like

| user_id | timestamp           | expense |
| ------: | :------------------ | ------: |
|       0 | 2021-02-18 10:00:00 |     100 |
|       0 | 2021-02-18 10:00:10 |      10 |
|       0 | 2021-02-18 10:00:21 |       1 |
|       0 | 2021-02-18 11:00:21 |     0.1 |
|       1 | 2020-02-18 10:00:10 |     100 |
|       1 | 2020-02-18 10:00:20 |      10 |
|       1 | 2020-02-18 10:00:20 |       1 |
|       1 | 2020-02-18 10:00:29 |       0 |

Suppose that you have to compute the following quantity **for each row in this dataframe**:

- the number of actions each user has taken within 10 seconds
- total amount of expenses of a user within 10 seconds

The following naive way is fine for this tiny example but becomes costly (O(N^2)) for large data frame.

```python
from datetime import timedelta
import numpy as np

td = timedelta(seconds=10)

answers = []
for uid, time_point in zip(df.user_id, df.timestamp):
    cnt = np.sum(
        (df.user_id == uid) & (df.timestamp < time_point) & (df.timestamp >= (time_point - td))
    )
    answers.append(cnt)
```

If `df` is sorted (by `user_id` as the primary and `timestamp` as the secondary key),
we can do this blazing fast (O(N)) using `log_count_util`.

```python
from log_count_util import find_n_records_within_interval

answers = find_n_records_within_interval(
    df.user_id, df.timestamp, df_user_id, df.timestamp, td
)
```
