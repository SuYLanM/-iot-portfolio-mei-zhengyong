# Module 03 — Sensor Characterization, Calibration & Data Quality

[Portfolio](../README.md) · [Course brief](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/03-sensor-characterization-and-calibration.md) · [Bench guide](bench-guide.md) · [Lab report](reports/lab-report.md) · [Evidence](evidence/README.md)

| Round | Task | Work and expected evidence | Status |
|---|---|---|---|
| 1 | [1. LDR transfer function](reports/task01-ldr.md) | Six-level light ladder, DMM and ADC readings, resistance, log-log power law and linear fit residuals, three repeats versus one ADC step | Six-distance ADC ladder and repeatability captured. DMM and one-code comparison pending |
| 1 | [2. DS18B20 characterization](reports/task02-ds18b20.md) | One probe or optional multidrop, four resolution settings, reference and unit means, outdoor transect, air/water step curves and time constants | One ROM, four resolutions and water step verified. Reference, field and still-air step pending |
| 2 | [3. Calibration](reports/task03-calibration.md) | Pot/ADC two-point voltage correction, LDR two-point power and linear models, DS18B20 cold offset | 3a and 3b validated at held-out points. 3a photos archived. 3c temperature references pending |
| 2 | [4. Hysteresis and repeatability](reports/task04-repeatability.md) | LDR up/down curves, ten marked pot returns, ten readings per probe at 9 and 12 bits | One LDR sweep, pot and one DS18B20 verified. Controlled sweep timing and multi-probe comparison pending |
| 3 | 5. Noise and filtering | 500+ raw values per input, raw/filtered statistics, single spike comparison, pot step lag and threshold decisions | Prepared. Bench pending |
| 3 | 6. Uncertainty and filter comparison (stretcher) | Budget for one measured value, expanded uncertainty at k = 2, dominant term, moving average versus EMA and median | Prepared. Measurement budget pending |
| All | A. Shared study log | References, findings, verified LLM answers, examples, mini checks and calibration decisions | Template ready |
| 2–3 | B. Two pod exchanges | Independent-point review, agreed learning, shared artefact and Discord post | Classroom activity pending |

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
| Search notes | Search term, finding and source URL — Pending |
| Surprises and counterexamples | Add one observation per round with the run or figure — Pending |
| LLM Q&A | Record the actual prompt, a 2–3 line summary, how the claim was checked and where it failed — Pending |
| Two characterization examples | Source and lesson from each selected example — Pending |
| Mini checks | Instructor prompt and one-line answer from each round — Pending |
| Calibration decision | Correction applied or rejected, independent residual and reason — Pending |
| Frost alarm | Record the reading and uncertainty near 2 °C, what the LDR establishes, the trust decision, and what seasonal effects could only be bounded — Pending |
| Agreed learning (pod discussion) | Add each member's note and a link to the shared artefact — Pending |

[Task B exchange record](reports/class-exchange.md)
