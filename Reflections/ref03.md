# Reflection 03 — Calibration reduces error without removing every uncertainty

> I based this retrospective on my CSVs and reports; personal reactions and unrecorded interactions still require my confirmation.

## Take-away

I now separate precision from accuracy more carefully. My two-point ADC correction reduced the independent validation error from +27.560 to +12.994 mV, but a smaller error is not zero error. I should keep calibration data, held-out validation and instrument uncertainty distinct.

## What was good?

I collected an LDR distance ladder and repetitions, DS18B20 conversions at four resolutions, and a real water-temperature transition. I paired 150 ADC samples per calibration point with confirmed meter readings. My [calibration report](../Module03/reports/task03-calibration.md) keeps the 1.526 V validation point outside the endpoint fit.

## What was difficult?

I had anomalous light-condition records and an ADC stream stuck at zero. After reconnection, the LDR responded, but I cannot identify the original physical fault with certainty. I also had to distinguish the earlier 2.389 V photograph from the actual 2.398 V high calibration reference, and preserve the low-point photo/display discrepancy.

## Exploration and show stoppers

I compared hysteresis-like directional differences, ten potentiometer returns and filtering with real data. My EMA smoothed strongly but delayed the measured transition more. I have only one temperature probe and no independent reference thermometer, so I cannot claim validated temperature offset correction or multi-probe agreement.

## Interaction with peers and instructors

I still need to confirm actual peer review, jointly agreed learning and instructor mini-check feedback. I have not replaced missing exchange evidence with technical conclusions from my scripts.

## How did you verify AI suggestions?

I used Codex for acquisition tools and analysis, then checked outputs against raw CSVs, retained anomalies and covariance calculations. I kept supplementary simulation separate. My [uncertainty report](../Module03/reports/task06-uncertainty-filtering.md) explicitly excludes unknown systematic bounds from its conditional random result.

## Help and extra work

I used Codex to produce plots and reproducible analyses. I cannot assign undocumented assistance or extra hours to anyone, including myself.

## Self-assessment against learning goals

I have substantial characterization, calibration and filtering evidence. I still need reference-temperature validation, controlled dynamic comparisons and a fuller uncertainty budget; my [goal map](../Module03/README.md#learning-goals) avoids claiming complete metrological validation.
