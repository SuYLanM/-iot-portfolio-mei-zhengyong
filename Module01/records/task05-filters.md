# Task 5 — Blank Real Filter Comparison

Status: **TODO, no observed noise reduction or latency entered**.

| Metadata | Value |
|---|---|
| Date/operator/board/Core/sketch settings | |
| Input: real LDR or potentiometer and wiring | |
| Common input CSV and same-run plot links | |
| Actual sample interval mean/range ms | |
| Gate bounds and justification | |
| Physical disturbance / sustained step conditions | |
| Baseline noise metric and interval; latency definition | |

Window/alpha labels below describe prepared configuration; they are not measured performance. Change them if code settings change.

| Output | Configured window / alpha | Measured steady noise (state metric/units) | Observed latency ms (state criterion) | What improved / what was lost | Evidence |
|---|---|---|---|---|---|
| Raw | none | | | | |
| Moving average | N=5 | | | | |
| Moving average | N=10 | | | | |
| Moving average | N=20 | | | | |
| Median | N=5 | | | | |
| Gated average | last 5 accepted values | | | | |
| EMA (optional) | alpha=0.2 | | | | |

## Step / impulse observations

| Run | Physical or explicitly synthetic? | Event timestamp ms | Raw/input code | MA5 | MA10 | MA20 | Median5 | Gated MA5 | Accepted? | EMA | Full window? |
|---|---|---|---|---|---|---|---|---|---|---|---|
| | | | | | | | | | | | |
| | | | | | | | | | | | |
| | | | | | | | | | | | |

TODO: compare outlier rejection with low-pass cutoff, explain increased delay with longer windows, and identify valid changes rejected by the gate. Record held/stale gated output explicitly. Keep any `synthetic=1` algorithm-test run separate and labelled; it cannot establish sensor noise or a real physical disturbance. No optional RC/high-pass/hysteresis measurements are claimed.
