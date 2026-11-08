# Module 03 — Sensor Characterization, Calibration & Data Quality

[Portfolio](../README.md) · [Course brief](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/03-sensor-characterization-and-calibration.md) · [Bench guide](bench-guide.md) · [Lab report](reports/lab-report.md) · [Evidence](evidence/README.md)

[Final submission audit](reports/final-submission-audit.md) lists each current requirement, the archived deliverables and the outstanding activities. This portfolio is submission-ready as a transparent record of achieved work, with the incomplete requirements disclosed.

| Round | Task | Work and expected evidence | Status |
|---|---|---|---|
| 1 | [1. LDR transfer function](reports/task01-ldr.md) | Six-level light ladder, DMM and ADC readings, resistance, log-log power law and linear fit residuals, three repeats versus one ADC step | Six-distance ADC ladder and repeatability captured. Supplementary one-code sensitivity modeled; independent DMM/local ADC validation pending |
| 1 | [2. DS18B20 characterization](reports/task02-ds18b20.md) | One probe or optional multidrop, four resolution settings, reference and unit means, outdoor transect, air/water step curves and time constants | One ROM, ten readings per resolution and water step verified. Current 20-reading warming/configuration evidence, reference, field and air step pending |
| 2 | [3. Calibration](reports/task03-calibration.md) | Pot/ADC two-point voltage correction, LDR two-point power and linear models, DS18B20 cold offset | 3a and 3b validated at held-out points. 3a photos archived. 3c temperature references pending |
| 2 | [4. Hysteresis and repeatability](reports/task04-repeatability.md) | LDR up/down curves, ten marked pot returns, ten readings per probe at 9 and 12 bits | One LDR sweep, pot and one DS18B20 verified. Controlled sweep timing and multi-probe comparison pending |
| 3 | [5. Noise and filtering](reports/task05-noise-filtering.md) | 500+ raw values per input, raw/filtered statistics, single spike comparison, pot step lag and threshold decisions | Real pot/LDR noise and decision replay verified. Unclipped descending transition latency measured; simulation retained |
| 3 | [6. Uncertainty and filter comparison (stretcher)](reports/task06-uncertainty-filtering.md) | Budget for one measured value, expanded uncertainty at k = 2, dominant term, moving average versus EMA and median | Available-data analysis complete. Conditional random budget; full uncertainty pending reference specifications and systematic bounds |
| All | A. Shared study log | References, findings, verified LLM answers, examples, mini checks and calibration decisions | Evidence-based analytical notes added. Personal learning, pod agreement and classroom checks pending |
| 2–3 | B. Two pod exchanges | Independent-point review, agreed learning, shared artefact and Discord post | Classroom activity pending |

## Final status with supplementary simulation

[Supplementary Simulation — Not Hardware Measurement](simulation/supplementary/README.md) contains reproducible parameters, synthetic CSVs, figures and limitations. These additions do not replace missing physical evidence. No further hardware experiments are planned for this submission stage.

| Task | Hardware / analysis status | Supplementary status | Pending External Evidence |
|---|---|---|---|
| 1 | **Hardware Partially Completed**: six light conditions, fits and repeats | **Simulation Completed**: code-step and resistance sensitivity | LDR-session DMM rail/node checks and validated local voltage-per-code comparison |
| 2 | **Hardware Partially Completed**: one probe, four resolution runs and water response | **Simulation Completed**: 80 warming points per resolution; ideal air/water models | Actual current warming/configuration exercise, reference temperatures, air and field observations |
| 3a / 3b | **Hardware Completed** for recorded two-point fits and held-out validation; nominal-light limitations retained | Task 1 sensitivity reuses ADC calibration and the real LDR fit | Peer review; no claim of complete metrological traceability |
| 3c | **Hardware Partially Completed**: stable indications only; **Reference Pending** | **Simulation Completed**: hypothetical cold offset, independent indoor/ice validation and reference sensitivity | Paired independent references and actual independent ice-water validation |
| 4 | **Hardware Partially Completed**: manual LDR sweep, ten pot returns, one probe at two resolutions | **Simulation Completed**: scan rates/dwell and three explicitly virtual probes | Controlled real timing comparison and real additional probes; actual probe count remains one |
| 5 | **Hardware Completed** for measured noise, digital-filter replay, real descending step and ADC threshold analysis | Existing synthetic spike/drift examples retained | No temperature-alarm hardware result is claimed; transition/latency limits remain in report |
| 6 (stretcher) | Available-data analysis completed; conditional uncertainty only, no new hardware claim | Existing illustrative reference sensitivity retained | DMM accuracy, drift and systematic/model bounds for a complete uncertainty statement |
| A / B | Evidence-based study notes prepared | Simulation is not personal or classroom evidence | Personal/member confirmation, shared agreement, exchanges and Discord post remain **Pending External Evidence** |

The hardware labels describe the named experiment, not professor acceptance or completion of every associated classroom activity. The [final audit](reports/final-submission-audit.md) remains the detailed outstanding-requirements checklist.

## Tools and outputs

- [Analog logger](code/analog_capture/analog_capture.ino) covers Tasks 1, 3a/3b, 4 and 5. It logs raw readings and three causal filters.
- [DS18B20 logger](code/ds18b20_capture/ds18b20_capture.ino) covers Tasks 2, 3c and 4 using OneWire 2.3.8 and DallasTemperature 4.0.6.
- [CSV templates](data/README.md), [Python analysis](analyze.py), [figures index](figures/README.md) and [verification record](evidence/verification.md).

Install Python dependencies with `python -m pip install -r Module03/requirements.txt`. Run from the repository root. In the commands below, replace input filenames, ROM and numerical placeholders with values from the named run. Use a separate output directory for each invocation.

```text
python Module03/analyze.py ldr RUN.csv OUT --adc-step-v LOCAL_V_PER_CODE
python Module03/analyze.py calibrate RUN.csv OUT --kind pot
python Module03/analyze.py calibrate RUN.csv OUT --kind ldr
python Module03/analyze.py calibrate RUN.csv OUT --kind temperature
python Module03/analyze.py repeat RUN.csv OUT --kind ldr
python Module03/analyze.py repeat RUN.csv OUT --kind pot
python Module03/analyze.py repeat RUN.csv OUT --kind ds
python Module03/analyze.py step RUN.csv OUT --rom ROM --step-s TRANSFER_SECONDS --final-start-s STABLE_TAIL_SECONDS
python Module03/analyze.py noise RUN.csv OUT --spike-codes SPIKE_DELTA
python Module03/analyze.py noise RUN.csv OUT --step-s STEP_ONSET_SECONDS
python Module03/analyze.py noise RUN.csv OUT --threshold CENTER_CODE --band HALF_WIDTH_CODES
python Module03/analyze.py budget RUN.csv OUT --value MEASURED_VALUE --unit V
```

Every command writes a JSON summary. LDR adds point residuals and fit plots. Noise adds filter CSVs and plots and, for fixed input, a statistics CSV. Hysteresis and thermal runs add plots. Budget adds a standard-uncertainty CSV. Omit `--adc-step-v` when a local ADC slope is unavailable and leave that comparison pending. The noise commands support separate fixed-input, step and slow threshold-drift runs.

## Study log

The [Task 3 calibration records](reports/task03-calibration.md) contain the fit, held-out residual, uncertainty notes and pending peer sign-off.

Shared pod log keeper and link from the other member's portfolio — Pending.

| Entry | Preparation / evidence to add |
|---|---|
| References | Course brief for independent validation, [DS18B20 datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/DS18B20.pdf) for resolution and accuracy, [ESP32 ADC documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html) for raw codes and attenuation, [GUM](https://www.bipm.org/en/committees/jc/jcgm/publications) for combining uncertainty. Add what each member understood. |
| Search notes | Official course text, GUM and DS18B20 datasheet were retrieved for Task 6 preparation. Personal search terms and findings — Pending |
| Surprises and counterexamples | Task 3a's +12.994 mV validation error exceeds the 3.552 mV conditional random k=2 result. Task 5 EMA smooths most strongly but has the longest measured delay. [Task 6 evidence](reports/task06-uncertainty-filtering.md) |
| LLM Q&A | Actual request on 2026-11-09 asked for a budget from Task 3a and a real-data filter comparison. The response propagates endpoint and validation noise, retaining coefficient covariance, and labels missing systematic terms. Checked against original CSVs, finite-difference sensitivities and equivalent covariance propagation in [the script](analyze_uncertainty.py). A complete accuracy interval cannot be inferred from the partial budget. |
| Two characterization examples | Candidates from actual work are the [two-point ADC validation](reports/task03-calibration.md) and [DS18B20 resolution/accuracy comparison](reports/task02-ds18b20.md). They show residual error after correction and why a finer code step does not establish accuracy. Members' selection of their best two examples — Pending |
| Mini checks | No instructor mini-check answers are recorded. Draft evidence-based answer on filter choice: median helps isolated spikes, EMA reduces SD with more latency, and a threshold decision needs hysteresis selected for the application. Classroom check — Pending |
| Calibration decision | The Task 3a two-point correction reduced the independent voltage error from +27.560 to +12.994 mV. The Task 3b held-out point favored the power law. Task 3c offset remains unapplied because a paired independent temperature reference is missing. |
| Frost alarm | Record the reading and uncertainty near 2 °C, what the LDR establishes, the trust decision, and what seasonal effects could only be bounded — Pending |
| Agreed learning (pod discussion) | Add each member's note and a link to the shared artefact — Pending |

[Task B exchange record](reports/class-exchange.md)

These additions summarize the recorded analysis. Each member's personal interpretation, shared log-keeper assignment, peer review, agreed learning and Discord artefact remain pending.

### Evidence-based reflection notes for student review

The ADC calibration reduced the held-out error but left a 12.994 mV discrepancy. The Task 6 conditional random budget was smaller than that discrepancy. This supports checking the reference, timing and measurement model before relying on extra smoothing to improve accuracy. The original photo mismatch also shows why a photograph and a contemporaneous paired reference need distinct labels.

LDR values were non-monotonic at some intermediate distances, and the manual up/down card sequence gave a directional gap. Together they show that repeatable geometry, settling time and illumination history matter when interpreting the transfer model. The current evidence cannot isolate static hysteresis.

The temperature probe gave fresh conversions and visible resolution-dependent code steps. Its repeated stable codes do not establish the true temperature. A reference near the frost threshold is still needed. In the ADC filter replay, EMA reduced SD most strongly and delayed the real transition most. A decision should account for both effects.

These are retrospective conclusions from archived records. Each member's own experience and agreed learning are awaiting confirmation.

### Sources, lookup topics and verification

| Lookup topic | Documented finding | Source and verification |
|---|---|---|
| ADC code versus voltage | Raw codes and API voltage conversions have distinct roles; Task 3a pairs the ADC with DMM readings | [ESP32 ADC API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html), checked against the sketch and original CSV columns |
| DS18B20 resolution versus accuracy | Step size changes with resolution; the datasheet accuracy is a separate specification | [Manufacturer datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/DS18B20.pdf), checked against ROM-specific run tables. The current 20-reading sweep remains pending |
| Correlated calibration coefficients | Slope and intercept share endpoint observations; their covariance enters propagation | [GUM](https://doi.org/10.59161/JCGM100-2008E), checked through two equivalent calculations in the Task 6 script |

### Selected actual LLM exchanges

| Recorded request | Answer summary and verification |
|---|---|
| “没有万用表，模拟一下吧” | Generated explicitly synthetic noise, spike, step and drift examples. Later ADC troubleshooting recovered a real light response and supplied a separate fresh 500-point record. Simulation did not establish hardware noise. |
| “请专门排查 ESP32 GPIO34 的真实模拟输入” | A minimal diagnostic returned zero under illumination and cover. After the operator reconnected the divider, nonzero/light-sensitive readings appeared. UART logs support the change, but the exact physical fault was not identified. |
| “现在开始完成 HSBI Sensors & Actuators — Module 3 — Task 6：Uncertainty and Declarative Filtering” | Recomputed uncertainty and filters from archived CSVs. Source hashes, finite-difference sensitivities and covariance identities checked the calculations. The result remains conditional because reference and systematic bounds are missing. |

### Draft mini-check answers from the records

- **Why is the voltage validation independent?** Its 1.526 V capture was acquired after the endpoint captures and excluded from the two-point fit.
- **Which correction acts on the chain?** Task 3a maps raw ADC code to DMM-referenced voltage within the tested range. Task 3c has no applied temperature offset.
- **What can be said about the frost threshold?** The present probe has no independent reference near 2 °C, so its uncertainty at that threshold is unverified. The LDR provides a relative-light indication with geometry and voltage limitations.
- **Which seasonal effects were measured?** Short-run noise, response, repeatability and held-out calibration errors were observed. Seasonal drift, installation and environmental dependence were not bounded.

These are document-based draft answers. Instructor mini checks and personal or joint confirmation remain pending.
