# Supplementary Simulation — Not Hardware Measurement

These supplementary calculations extend the archived Module 03 experiments without new wiring, firmware uploads or measurements. They do not fill missing hardware, reference-instrument or classroom evidence. Original files in `evidence/` and `data/` remain unchanged; all new synthetic datasets and figures stay here.

## Results index

| Topic | Data-based input | Assumed/model input | Output and scope |
|---|---|---|---|
| [Task 1: quantization](task01-quantization/README.md) | Six original LDR voltage means; Task 3a two-point voltage/code secant | Hypothetical ideal ADC span; nominal supply and resistor | One-code voltage and resistance sensitivity. Not a local ADC calibration or DMM comparison |
| [Task 2: temperature resolution and media](task02-ds18b20/README.md) | Real 12-bit mean; archived water endpoints and tau ≈ 9.67 s | Linear warming; first-order ideal bath step; air tau = 45 s | 80 synthetic points at each of four resolutions; modeled air/water curves. No measured air response |
| [Task 3c: offset calibration](task03c-calibration/README.md) | Datasheet quantization step only; **no real reference temperature** | 5/20/0 °C reference scenarios, bias law, reference-error bounds | Cold-point offset and independent synthetic validation; real Task 3c remains Reference Pending |
| [Task 4: dynamics and virtual probes](task04-hysteresis/README.md) | Refitted six-point LDR static law; real single-probe mean as a numerical anchor | LDR tau = 2 s, protocol timing, three virtual probe biases/noise | Rate-dependent loops and synthetic unit comparison. Not evidence of real static hysteresis or additional probes |

Task 5's real measurements, existing synthetic demonstrations and report are preserved. Task 6's [existing illustrative reference-uncertainty sensitivity table](../../data/task06-analysis/illustrative-reference-sensitivity.csv) and [conditional-budget explanation](../../reports/task06-uncertainty-filtering.md) remain applicable. The Task 3c sensitivity plot adds a clearly hypothetical temperature-reference example; it does not supply a meter specification or complete metrological traceability.

## Reproduce and verify

From the repository root, using the existing Module03 Python dependencies:

```text
python Module03/simulation/supplementary/generate_supplementary.py
python Module03/simulation/supplementary/validate_supplementary.py
```

- [Input parameters and assumptions](simulation_parameters.json) contain every chosen numerical model parameter and the fixed random seed. Units appear in field names.
- [Generator](generate_supplementary.py) reads original files and writes only inside this supplementary directory. It never opens a serial port.
- [Derived results](derived_results.json) distinguish computed data-derived coefficients from assumed parameters.
- [Source SHA-256 hashes](source_hashes.json) identify all input files read by the generator. Original failed/non-monotonic LDR records are preserved; the accepted-ladder roles follow the original manifest rather than a new selection.
- [Validation results](validation_results.json) record CSV counts, finite-number checks, quantization, timing, model checks, source integrity and image decoding. Re-running the generator should reproduce byte-identical outputs in the same Python environment.

Every CSV row is tagged `SUPPLEMENTARY_SIMULATION_NOT_HARDWARE_MEASUREMENT`. Mixed calculations identify real source means separately from modeled outputs. Plot headings carry the same distinction. No simulated calendar dates, real-looking ROM IDs, UART logs, certificates or peer comments are generated.

## Interpretation and remaining evidence

All four supplementary groups are **Simulation Completed**. The [final status table](../../README.md#final-status-with-supplementary-simulation) retains hardware and external-evidence limitations. In particular, real LDR-session DMM checks, independently referenced DS18B20 calibration, actual air/field observations, the current warming/configuration exercise and controlled/multiple-probe experiments remain missing. There will be no further hardware work in this submission stage.

Task A's personal/member confirmation and Task B's peer exchanges and Discord artefact remain **Pending External Evidence**. Simulations do not establish that these activities happened.

## Sources

- [Official Module 3 brief](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/03-sensor-characterization-and-calibration.md): task scope; the supplied course page contains a draft-boundary notice.
- [DS18B20 manufacturer datasheet, Rev 6](https://www.analog.com/media/en/technical-documentation/data-sheets/DS18B20.pdf): configuration register, resolution, accuracy and maximum conversion times. The ±0.5 °C specification at −10 to +85 °C is not a measured error of this probe or a distribution for virtual units.
- [Arduino-ESP32 ADC API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html): raw ADC readings versus API-estimated voltage. Nominal 3.3 V does not define the attenuation-specific ADC transfer function.
