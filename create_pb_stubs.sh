#!/bin/sh
pybind11-stubgen -o stubs --no-setup-py log_count_util._core
rm log_count_util/_core.pyi
echo 'm: int
n: int
from numpy import float64, int64
' >> log_count_util/_core.pyi
cat stubs/log_count_util/_core-stubs/__init__.pyi >> log_count_util/_core.pyi
black log_count_util/_core.pyi

