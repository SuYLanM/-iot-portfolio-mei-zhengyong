# Data templates

These CSVs contain headers only. Copy a template to a run-specific file and enter actual observations. Use SI units where specified. Keep raw captures separate from copied or aggregated data and identify their source in the bench log.

| CSV | Use |
|---|---|
| [LDR](ldr.csv) | One row per level with role `ladder`, then three independent rows at the same level with role `repeat`. For Task 3b copy only the two fit rows and independent validation row, labeling them `fit1`, `fit2`, `validation`. Record measured rail and node voltages. Use lux for every row or distance for every row. |
| [Pot calibration](pot-calibration.csv) | `fit1`, `fit2`, `validation`. Average raw and baseline columns of the corresponding short capture. Convert baseline mV to V. DMM reading is `reference_v`. |
| [Temperature calibration](temperature-calibration.csv) | `cold`, `indoor`, `ice` for one ROM, with sensor and reference readings. |
| [LDR sweep](ldr-sweep.csv) | Label each paired sweep, e.g. `fast`, `slow`, `paused`. Record `up` and `down` at matching measured lux or documented relative light levels, with timing and dwell notes. |
| [Pot returns](pot-repeat.csv) | Ten independent returns, one row per return. |
| [Temperature reference](temperature-reference.csv) | Stable reference comparisons and outdoor transect. Record date/time, site, probe and reference, and conditions. |
| [Uncertainty](uncertainty.csv) | One contribution per row in the final measurand unit. Distribution is `rectangular_half_width` or `standard`. Basis identifies measured, datasheet, or assumed. Source names the run, specification or assumption. |
| [Analog capture](analog.csv) and [temperature capture](temperature.csv) | Match sketch headers. Keep boot messages and markers in the original evidence log. |

Record meter model/range/accuracy, ADC setup, fixed resistor value and whether measured or nominal, lamp geometry, ambient shielding, lux reference, supply stability, and timing in the bench log. Retain excluded points and explain their exclusion.
