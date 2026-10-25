# Task 3 — Remaining Real ADC Record

Status: **real 10/12-bit endpoint comparison documented** in the [ADC hardware report](../evidence/Task03_ADC.md). That report is the source for endpoint raw codes, API millivolt readings, theoretical step calculations and the original five images. Results are not copied into a second table here. The task remains incomplete: blank cells below are TODOs, never zeros.

| Missing metadata / independent measurement | Value — TODO |
|---|---|
| Actual test date / physical board marking | |
| Person measuring / person recording | |
| Potentiometer measured resistance / independent pin-to-terminal continuity | |
| Independently measured supply (distinct from assumed full scale in the report) | |
| Actual sample intervals / full sweep duration | |
| DMM model, range, resolution, accuracy and uncertainty | |
| Complete raw CSV and plot links (endpoint screenshots are already in the report) | |

## Additional sweep evidence — TODO

| Resolution setting | Complete sweep log / plot | Observed unique codes over that documented run | Observed adjacent-code voltage separation V | Endpoint behaviour / limits |
|---|---|---|---|---|
| 12 bit | | | | |
| 10 bit | | | | |

The screenshots show running `distinct_observed` counters, not a controlled complete-sweep result. Record counts with their run boundaries; do not combine counters from different captures/resets. Observed unique codes are not `2^bits` unless actually captured. Estimate adjacent-code voltage separation only with sufficient independent measurement resolution; otherwise state not resolved. Noise can increase observed unique codes without increasing accuracy.

## Stationary DMM cross-check — add several positions for each resolution

| Date/run | Bits | Position / condition | Repetition | Raw | Ideal estimate V | API mV (separate conversion) | DMM V | Ideal−DMM V | API/1000−DMM V | Uncertainty / notes |
|---|---|---|---|---|---|---|---|---|---|---|
| | | | | | | | | | | |
| | | | | | | | | | | |
| | | | | | | | | | | |
| | | | | | | | | | | |
| | | | | | | | | | | |
| | | | | | | | | | | |

TODO: collect several stationary intermediate positions for both settings (approximately 25%, 50%, 75% travel is a useful plan, not measured voltage). The official Task 3 calls for multimeter cross-checks at several positions: lack of a meter leaves this requirement TODO, rather than proving it unnecessary. Compare low/mid/high range, possible offset/gain/nonlinearity and 10/12-bit results. Distinguish code resolution, repeatability and physical accuracy. Do not assign a hardware fault before observing it. Optional Task 7: actual smallest distinguishable input changes near each endpoint and proposed conditioning improvement, if attempted.
