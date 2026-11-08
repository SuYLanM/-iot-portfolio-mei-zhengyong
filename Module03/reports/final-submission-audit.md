# Module 03 — Final submission audit

Audit date 2026-11-09. [Official brief](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/03-sensor-characterization-and-calibration.md) checked at source commit `63625214d3ed56b4469c2ea355e1bfe02bd07e92`. The course marks this material as a provisional draft. Requirements below reflect the version available on the audit date.

**Submission assessment.** The portfolio is ready to submit as an evidenced record of work achieved under the available conditions, with the pending items disclosed. It does not yet demonstrate every activity in the current brief. Any assessment or acceptance of the remaining gaps belongs to the instructor.

Status terms are **Verified**, **Partial**, **Pending**, **Conditional** and **Optional**. A successful build establishes software availability; the linked raw records establish the captured hardware outcomes.

## Task 1 — LDR transfer function

| Requirement | Status and evidence |
|---|---|
| Divider orientation and resistance formula | Verified in [report](task01-ldr.md) and manifest |
| DMM rail and node measurements in the LDR session | Pending. ADC voltage estimates and nominal 3.3 V are used. The later Task 3a rail photo cannot establish the earlier rail |
| At least six light conditions and logged observations | Verified. Six distance groups, 150 samples each, with excluded attempts retained |
| Relative illuminance or lux-ratio comparison | Verified using inverse-square relative distance. No lux instrument was used |
| Log-log fit, slope, residuals and linear comparison | Verified. Gamma 0.263, explicit point residuals and model metrics |
| Three independent repeats at one level | Verified at 30 cm. One-local-code resistance comparison pending |
| Class table / neighbour comparison if available | Pending. No class dataset or exchange evidence |
| Light-step dynamics and fall-light extension | Optional, unperformed |

## Task 2 — DS18B20 characterization

| Requirement | Status and evidence |
|---|---|
| Datasheet resolution, accuracy, conversion, current and pull-up notes | Verified in [report](task02-ds18b20.md) |
| One externally powered probe, ROM discovery and fresh readings | Verified. One CRC-valid ROM on GPIO18 |
| Additional 2–3 probes on the same bus | Optional in Task 2. Only one probe is available |
| Current Task 2.4 resolution verification and at least 20 warming readings per resolution | Partial. Existing captures contain ten valid readings per setting. The code checks the per-address getter, but setter-return checking and printed getter evidence are missing. A changing-temperature 20-reading run per setting is pending |
| Stable probe versus independent reference, ten observations and unit/class comparison | Pending independent thermometer and class data. Sensor-only stable captures exist |
| Outdoor transect with co-located reference, spot and time | Pending |
| Cold-to-warm water trace, time constant and settling estimate | Partial. 240 valid samples and a trace-derived onset estimate. The delayed chat marker and changing bath limit timing accuracy |
| Still-air step and air/water comparison | Pending |
| Self-heating and DHT11 extensions | Optional, unperformed |

## Task 3 — Calibration and independent validation

| Requirement | Status and evidence |
|---|---|
| 3a two DMM/ADC endpoint pairs | Verified. 0.568 and 2.398 V, 150 samples each |
| 3a independent validation and signed before/after error | Verified. Held-out 1.526 V, +27.560 to +12.994 mV |
| 3a gain/offset interpretation | Verified in [report and calibration records](task03-calibration.md) |
| 3a neighbour gain/offset comparison and sign-off | Pending |
| 3b two-point log power law and same-point linear model | Verified using existing Task 1 endpoint captures |
| 3b excluded third condition and residual comparison | Verified with the existing 30 cm capture excluded from both fits. This is retrospective held-out validation within one session |
| 3b reference/uncertainty qualification | Partial. Relative light and ADC-derived resistance are explicit; absolute reference uncertainty is unquantified |
| 3c cold outdoor and indoor paired references | Pending. Stable sensor-only records do not establish a reference |
| 3c cold-point correction and independent stirred-ice validation | Pending. No correction or accuracy improvement is claimed |
| Standard calibration record and peer check | Records added from existing values. Reviewer initials, date and exchange remain Pending |

## Task 4 — Hysteresis and repeatability

| Requirement | Status and evidence |
|---|---|
| LDR increasing/decreasing light at matched settings, curves and gap | Verified for one manual card sequence in [report](task04-repeatability.md). Positions are relative; the high point is a shared turn |
| Two sweep speeds and a pause at each level | Pending controlled timing. Static hysteresis cannot be isolated by the existing sequence |
| Ten independent marked-position potentiometer returns | Verified. Each return and baseline has 150 raw samples and timestamps |
| Discuss mechanical, positioning and ADC contributions | Verified as possible contributors; none was isolated |
| Ten 9-bit and ten 12-bit observations at a stable point | Partial. Valid single-probe short runs exist, but independent environment/reference stability is unverified |
| Task 4's requested 2–3-probe comparison | Pending. Task 2 lists multidrop as optional, while Task 4 explicitly asks to repeat across probes |
| State measured and unmeasured effects | Verified in the report conclusion |

## Task 5 — Noise and filtering

| Requirement | Status and evidence |
|---|---|
| Fixed-pot 500-sample noise floor | Verified. New capture, non-clipped, 500 rows |
| Fixed-LDR 500-sample capture and comparison | Verified. Different operating levels and source impedances limit attribution |
| Mean-5, median-5, EMA and common comparison interval | Verified on the same real inputs, with original rows preserved |
| Single-sample outlier comparison | Verified as explicitly synthetic injection into a separate real-data copy |
| Real potentiometer transition and measured filter delay | Verified for a non-clipped descending transition. The requested rising direction was not achieved; the course step-latency objective is demonstrated on the recorded direction |
| Two-threshold/dead-zone decision comparison | Verified as replay on real ADC records, with a clearly labeled synthetic slow-drift supplement |
| Noise/lag trade-off and frost-alarm discussion | Verified in [report](task05-noise-filtering.md). No physical temperature alarm or heater test is claimed |
| Wider filter / analog RC extensions | Optional, unperformed |

## Task 6 — Uncertainty and filtering stretcher

| Requirement | Status and evidence |
|---|---|
| Budget for one measurand with distributions, sources and common units | Verified as a partial budget for the Task 3a ADC mean |
| Covariance-aware combination and k=2 statement | Conditional. uc 1.776 mV and U 3.552 mV include the specified random terms only |
| Unknown terms and validation residual treatment | Verified. DMM accuracy, drift and other systematic terms remain Unknown; the residual is not assigned a random variance |
| Firmware / alternative filtering comparison | Verified through archived firmware columns and raw-data replay of mean, median and EMA. IoTempower is optional and unused |
| Dominant contribution and engineering priority | Verified for the quantified partial terms. Total dominance remains undetermined |

See the [Task 6 report](task06-uncertainty-filtering.md). A full metrological interval remains pending, with the limits disclosed.

## Task A and Task B

| Task A requirement | Status |
|---|---|
| Shared `## Study log` in module README | Present. Log keeper assignment and each member's personal takeaway pending |
| Sources and concepts | Present as evidence-based analytical notes; personal interpretation requires review |
| Search terms, finding and URL | Technical lookup topics added. Personal search history is not inferred |
| Surprises and counterexamples | Added from recorded voltage, noise, timing and photo discrepancies |
| LLM prompts, summary and verification | Selected actual exchanges documented. Full member-curated prompt log remains incomplete |
| Two best characterization examples | Candidates linked. Personal selection and confirmation pending |
| Mini checks and calibration decision | Draft analytical answers and decisions available. Actual instructor mini-check records pending |

Task B has no documented neighbour exchange, peer initials, shared 3–5 takeaways, jointly agreed artefact, Discord post or member confirmation. Each remains **Pending** in the [exchange record](class-exchange.md). None can be completed from the numerical dataset alone.

## Deliverable inventory

| Task | Source / analysis | Raw evidence / derived data | Figures and report | Photographs |
|---|---|---|---|---|
| 1 | [Analog sketch](../code/analog_capture/analog_capture.ino), [analysis](../analyze_ldr_run.py) | [Archive](../evidence/task01-ldr/) | [Figures](../figures/task01-ldr/), [report](task01-ldr.md) | No dedicated Task 1 photos archived |
| 2 | [Probe sketch](../code/ds18b20_capture/ds18b20_capture.ino), [analysis](../analyze.py) | [Archive](../evidence/task02-ds18b20/) | [Figures](../figures/task02-ds18b20/), [report](task02-ds18b20.md) | No dedicated Task 2 photos archived |
| 3 | [ADC sketch](../code/adc_calibration/adc_calibration.ino), [analysis](../analyze_calibration.py) | [ADC](../evidence/task03-calibration/adc/), [probe check](../evidence/task03c-ds18b20/) | [Figures](../figures/task03-calibration/), [report](task03-calibration.md) | [Four original DMM photos](../evidence/task03-calibration/adc/photos/) |
| 4 | Reuses analog/probe sketches and [analysis](../analyze.py) | [LDR](../evidence/task04a-ldr/), [pot](../evidence/task04b-pot/), [probe](../evidence/task04c-ds18b20/) | [LDR figures](../figures/task04a-ldr/), [pot figures](../figures/task04b-pot/), [probe figures](../figures/task04c-ds18b20/), [report](task04-repeatability.md) | No dedicated Task 4 photos archived |
| 5 | Reuses analog sketch, [analysis](../analyze.py) | [Archive](../evidence/task05-noise/) | [Figures](../figures/task05-noise/), [report](task05-noise-filtering.md) | No dedicated Task 5 photos archived |
| 6 | [Analysis](../analyze_uncertainty.py); no separate firmware required | [Budget and source hashes](../data/task06-analysis/) | [Figures](../figures/task06-analysis/), [report](task06-uncertainty-filtering.md) | No new hardware experiment in Task 6 |

The current brief emphasizes measurement records and learning evidence. It does not prescribe a photograph for every task. Additional setup photographs would help traceability but do not replace the missing DMM, thermometer or classroom observations.

## File and provenance checks

- All six reports have direct links in the module README. The audit verified 232 local paths/anchors and decoded 35 image files. SHA-256 checks confirmed that all 307 existing non-Markdown files were unchanged.
- Four DMM photographs are original images. The low-photo filename says 0.568 V but the display is 0.589 V. The high photo shows the earlier 2.389 V. The fit still uses the separately confirmed 0.568 and 2.398 V pairs. Both differences are explained.
- Synthetic inputs remain under `simulation/task05/`. Injected-spike CSVs and plots are explicitly named. A real-data-plus-drift copy preserves its original hardware and added-drift columns.
- Corrupted UART captures and rail-code failures are retained and described. They are excluded from the successful noise statistics.
- Byte-identical files include repeated build outputs, identical ROM-only scans, zero-code diagnostic captures, shared attributes and repeated derived noise tables. These have different run or analysis roles. No duplicate raw evidence was deleted.
- No doubled image extensions were found. Generated figures are distinct from original photographs.
- Earlier stale Pending statements in the evidence index, figures index, planning guide and verification page were corrected or labeled as historical.

## Outstanding material for full course coverage

1. LDR-session DMM rail/node values and a justified local one-code comparison.
2. Task 2.4's current configuration checks and 20-reading warming sweeps, stable independent temperature references, assigned outdoor observations and an air response trace.
3. Task 3c's paired cold/indoor reference measurements, correction and independent ice-water validation.
4. Task 4's controlled sweep-speed/dwell comparison and the additional probe comparison.
5. Member-reviewed Task A entries and the actual Task B exchanges and Discord artefact.

Class tables apply if supplied by the instructor. Optional light dynamics, fall-light, DHT11, self-heating, analog RC and IoTempower extensions remain optional. Meter specifications and controlled drift measurements would strengthen the conditional Task 6 budget.
