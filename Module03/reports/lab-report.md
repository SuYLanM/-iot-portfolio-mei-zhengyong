# Sensor characterization, calibration and data quality

**Status:** Software preparation. Hardware and classroom results pending. See the [bench guide](../bench-guide.md), [CSV templates](../data/README.md) and [evidence index](../evidence/README.md).

## Task 1 — LDR transfer function

See the [Task 1 bench report](task01-ldr.md) for the live capture, results and outstanding measurements.

**Objective.** Estimate how resistance changes with illumination using the DMM as the voltage reference and the ESP32 as the ADC logger.

The LDR is the upper divider element and the fixed resistor is the lower element. Therefore `R = Rfixed × (Vs − Vout) / Vout`, and more light raises the node voltage. Measure the supply and node voltages and identify whether the fixed resistor was measured or taken at nominal value. Record at least six levels and three repeats of one level. Use measured lux for the entire fit or relative illuminance from inverse-square distance. If both are available, compare their ratios and explain deviations from point-source geometry. A phone app supplies an approximate relative reference.

| Result | Evidence to add |
|---|---|
| Light ladder and optional class rows | Distance, lux if available, rail and node volts, ADC code, calculated resistance, orientation |
| Power law `R = A E^(-γ)` | Log-log slope, A, residuals and plot |
| Linear fit over the same range | Residuals in ohms and comparison with power law |
| Repeatability | Three independent readings, range and SD versus local one-code resistance change |

Record the largest residuals and possible causes such as ambient light, lamp geometry, saturation and light history. If available, compare slope and repeats with the class table. Optional extensions are a light-step time constant and a sheltered fall-light comparison.

## Task 2 — DS18B20 characterization

See the [Task 2 bench report](task02-ds18b20.md) for GPIO18 wiring, CSV capture, and the thermal step procedure. The current course brief permits one probe, with multiple probes optional.

| Resolution | Step (°C) | Datasheet maximum conversion time (ms) |
|---|---:|---:|
| 9 bit | 0.5 | 93.75 |
| 10 bit | 0.25 | 187.5 |
| 11 bit | 0.125 | 375 |
| 12 bit | 0.0625 | 750 |

The manufacturer specifies ±0.5 °C over −10 to +85 °C, active current up to 1.5 mA and standby current up to 1 µA under the stated datasheet conditions. The bus uses 64-bit ROM addressing and a 4.7 kΩ pull-up in this experiment. [Analog Devices DS18B20 datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/DS18B20.pdf)

- **Stable reference.** Record ROM IDs and ten readings per probe next to the reference. Compare means, SD and reference errors. If only one probe is available, use class data for the unit comparison when available.
- **Field transect.** Measure one or two assigned locations and record time, height or depth, reference and conditions. Compare cold spots against reference error and datasheet limits.
- **Dynamics.** At 12 bits, estimate the elapsed time from transfer to 63.2% of the observed step. Report both the approximate 4–5τ settling range and the observed sustained 5% settling time when available. Compare water and air using separate plots and state transfer-time uncertainty. Optional extensions are self-heating and the DHT11 comparison.

## Task 3 — Calibration and independent validation

| Case | Fit | Independent check and interpretation |
|---|---|---|
| 3a Pot/ADC | `V = gain × code + offset` from two DMM-referenced points in a usable range | Third voltage, signed error before (ADC API baseline) and after (new model), noise comparison and neighbour gain/offset |
| 3b LDR | Two light levels determine the power law in log space and a linear resistance model | Predict resistance at the same third light level with both models, compare residuals and state the two largest uncertainty sources |
| 3c DS18B20 | Measure cold outdoor and indoor offsets; apply only `reference_cold − sensor_cold` | Stirred ice water. Compare before/after error and whether the indoor correction predicts the cold observation. Decide if correction helps near 2 °C. |

Use separately acquired validation observations and keep them outside the fit. Record which correction helps and what it corrects. A single-point offset does not establish performance across temperatures.

## Task 4 — Hysteresis and repeatability

- **LDR.** Overlay increasing and decreasing runs at matched light levels for two sweep speeds and a paused sweep. Report the largest gap for each. Compare rate dependence and dwell dependence before attributing a gap to static hysteresis.
- **Pot.** Perform ten independent returns to a marked position. Report DMM and ADC ranges and sample SD. Compare with the fixed-pot noise baseline and discuss marking error and mechanical play.
- **DS18B20.** At one stable reference condition record ten readings at 9 and 12 bits for each probe. Compare observed spread, code step and sensor offsets. Record one sentence per sensor on what effect the test isolated and what remains confounded.

## Task 5 — Noise, filtering and decisions

| Run | Record |
|---|---|
| Fixed pot and fixed LDR | Sample count, cadence, mean, sample SD and conditions |
| Fixed LDR replay | Raw, trailing mean-5, trailing median-5 and EMA α = 0.2 on the same samples. Compare SD over the common valid interval. |
| Spike | Inject a single labeled synthetic perturbation into a copy and compare peak deviations with unmodified replay. |
| Pot step | Measured half-step crossing and extra delay versus raw for all filters. Compare the mean's nominal two-sample group delay with the observed response. |
| Slow drift | Compare single and two-threshold decisions and their transition counts; discuss stability versus delayed switching. |

State which filter and decision rule you would ship for the frost alarm and its latency cost. Preserve raw data and record any software-injected perturbation separately.

## Task 6 — Uncertainty and filter comparison (stretcher)

Select one measurand (for example calibrated wiper voltage), report its value and state whether it is a single reading or a mean. Use the budget CSV. Convert each contribution into the measurand unit using its sensitivity before combining independent terms. A rectangular half-width `a` contributes `a / sqrt(3)`, so one ADC step contributes `step / sqrt(12)` under a uniform rounding assumption. Use a local volts-per-code slope. Avoid double-counting noise and repeatability from the same mechanism. If reporting a mean, justify the effective sample count in the presence of serial correlation.

| Contribution | Evidence / basis to enter |
|---|---|
| Reference | DMM or thermometer range, specification and reading |
| Component tolerance | Measured or nominal resistor with source and propagation where applicable |
| Quantization | Local code step or temperature resolution |
| Repeatability | Independently reset or repeated runs |
| Noise | Fixed-input traces and filter bandwidth |
| Nonlinearity | Independent residuals over the stated range |
| Drift | Repeat later, or document specification / bound and assumptions |
| Self-heating | Controlled comparison or documented bound if relevant |

Report `y ± U`, with `U = 2 × sqrt(sum(u_i²))`, units, assumptions and dominant contributor. If a term is unknown, state the limitation instead of assigning it zero without justification. Compare the firmware's moving average with EMA and median on the same capture using SD, spike response and step delay. The optional IoTempower comparison is outside this bench workflow.

## Conclusion and peer review

- Frost decision at 2 °C, with uncertainty and the relevant seasonal limitations — Pending.
- Peer validation and shared artefact — [Task B record](class-exchange.md).
- References and decisions — [shared study log](../README.md#study-log).
