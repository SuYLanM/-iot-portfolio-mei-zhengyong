# Task 5 — Shared-input Digital Filter Comparison

Status: **TODO: actual input, plots, noise and latency measurements**.

## Parts, GPIO and wiring

ESP32, USB cable, GPIO34 and the inspected LDR + confirmed 10k divider from Task 4. Alternatively use Task 3's potentiometer divider. Breadboard/jumpers and 3.3 V supply; no LED/button required. TODO confirm the chosen sensor, resistance and pins; disconnect power to rewire.

## Run

1. Open `task05_filters.ino`; ESP32 Dev Module / Core 3.3.12 / COM3. Verify, then upload after hardware review. Open Serial Plotter at 115200.
2. Hold a stable condition, apply a sustained step, then a short physical disturbance. All filters receive **the same sampled input**. Wait until `full_window=1` before steady-state comparisons (20 samples); gated startup is separate.
3. Compare moving average windows 5, 10 and 20, median window 5, validity-gated moving average 5 and optional EMA (`alpha=0.2`). Hide irrelevant traces for readability.
4. For timestamped records set `PLOT_OUTPUT=false`, reupload and save CSV. Do not claim separate live runs are identical inputs: each CSV row contains the common input and all outputs for that run.
5. Optional software-only spike: in Serial Monitor send lowercase `s`. One next sample is replaced by 4095 and `synthetic=1`; CSV retains `measured_raw` too. Label and separate this algorithm test from physical disturbances and hardware evidence. No synthetic spike is inserted by default.

## Output and filter behaviour

CSV: `t_ms,dt_ms,measured_raw,input_raw,synthetic,ma5,ma10,ma20,median5,gated_ma5,gate_accept,gated_valid,ema,full_window`. Plot mode gives named input/filter traces and status flags. Nominal sampling is 20 ms (50 Hz); CSV `dt_ms` measures actual spacing, first row 0 means no preceding sample.

- MA5/10/20 use available samples during startup, then full windows. Theoretical group delays are `(N-1)/2` sample intervals; full step settling takes about N samples. Measure actual latency separately; these formulas are not observed results.
- Median5 rejects an isolated impulse after startup; a real cover/uncover lasting several samples may be a real step, not a single spike. Even startup counts use the mean of the two middle values.
- Gate accepts 205–3890 inclusive (illustrative 5–95% band in 12-bit codes). **TODO: justify/change bounds from actual plausible input.** Rejected samples never enter its average. If all are rejected at startup, output is -1 and `gated_valid=0`. After initial valid samples, rejection holds the previous accepted-sample average: this may become stale. Check `gate_accept`, not just a smooth curve. Accepted-sample windows do not span a fixed time after rejection. Legitimate dark/bright readings can be wrongly rejected.
- Longer MA windows smooth more but follow changes more slowly: this is the low-pass cutoff trade-off, distinct from the outlier validity gate. EMA is optional extra smoothing.

Fill [filter record](../../records/task05-filters.md) with window sizes, bounds, actual sample interval, noise metric and latency criterion (e.g. reaching 90% of a sustained step). Record raw and filtered results on the same plot/log. No measured benefit or latency is claimed in advance. Optional hysteresis, dead-zone and analog RC stretch tasks are not part of this sketch.
