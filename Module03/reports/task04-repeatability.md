# Task 4 — Hysteresis and repeatability

## Objective

Distinguish the spread after a mechanical return from within-position ADC scatter, compare DS18B20 quantization at two resolutions, and check for direction-dependent LDR response.

## 4b — Potentiometer repeatability

**Hardware verified.** The potentiometer outer terminals were connected to 3V3 and GND and its wiper to GPIO34. The [ADC sketch](../code/adc_calibration/adc_calibration.ino) was compiled and flashed on COM3. After establishing a marked baseline, the operator moved away and returned ten times. A [serial collector](../code/capture_pot_repeat.ps1) triggered 150 samples only after each operator confirmation. Raw ADC and separate API voltage conversions were captured at 115200 baud.

| Statistic | Result |
|---|---:|
| Baseline mean / within-capture SD | 3179.59 / 21.44 codes |
| Mean of ten return means | 3268.96 codes |
| Sample SD of ten return means | 98.17 codes |
| Range of ten return means | 350.57 codes |
| Mean signed deviation from baseline | +89.37 codes |
| Largest absolute deviation | 272.43 codes |
| API voltage estimate across return means | Mean 2.7539 V, SD 0.0579 V, range 0.2043 V |

Within-return sample SDs ranged from 2.05 to 30.82 codes. The larger between-return variation describes the repeatability of the full marked-position procedure. Mechanical play, return direction, alignment of the mark and electrical/ADC variation were not isolated. The voltages are ADC API estimates. The Task 3a fit was not extrapolated to this higher voltage range.

[Original runs and timestamps](../evidence/task04b-pot/) · [Per-return statistics and signed deviations](../figures/task04b-pot/return-deviations.csv) · [Summary](../figures/task04b-pot/summary.json)

![Deviation of each return from the baseline](../figures/task04b-pot/pot-returns.png)

Reproduce the statistics with `python Module03/analyze.py repeat Module03/evidence/task04b-pot/trials.csv Module03/figures/task04b-pot --kind pot-adc`.

## 4c — DS18B20 resolution and repeatability

**Hardware verified for one probe.** The [DS18B20 sketch](../code/ds18b20_capture/ds18b20_capture.ino) was compiled and flashed on COM3. On GPIO18 the logger identified one family-0x28 ROM, `28E55FA300000007`, and validated its CRC. It requested and polled a new conversion for each row. Runs were sequential with no commanded change of environment. No independent thermometer or environmental stability trace was available.

| Resolution | Valid samples | Quantization step | Mean | Sample SD | Range | Mean conversion |
|---|---:|---:|---:|---:|---:|---:|
| 9 bit | 10 | 0.5 °C | 22.5500 °C | 0.1581 °C | 0.5000 °C | 78.0 ms |
| 12 bit | 10 | 0.0625 °C | 22.2938 °C | 0.0302 °C | 0.0625 °C | 616.1 ms |

Each run spanned one code step. The smaller 12-bit step describes quantization. The observed spread does not establish absolute accuracy or exclude changes between runs. Directional temperature hysteresis and a multi-probe comparison remain pending.

[Original runs and scan](../evidence/task04c-ds18b20/) · [Statistics](../figures/task04c-ds18b20/summary.json)

![Sequential ten-reading DS18B20 runs](../figures/task04c-ds18b20/repeatability.png)

## 4a — LDR hysteresis and light history

**Software prepared. Hardware pending.** With USB disconnected, remove the potentiometer wiper wire from GPIO34 before connecting the LDR divider node. Wire 3V3 → LDR → node → 10 kΩ → GND. The [analog capture sketch](../code/analog_capture/analog_capture.ino) records GPIO34 raw ADC and separate voltage estimates. Command `t` records 150 samples. Keep the lamp fixed and use a card with repeatable marked openings. Record the same levels in increasing and decreasing light. Use the same sample window and document actual dwell times and direction. Repeat at a faster and slower cadence, with a paused sequence to help distinguish lag from light history. Use matched measured lux if available or clearly identified relative card settings.

Save raw captures and populate the [sweep template](../data/ldr-sweep.csv), recording whether the divider voltages and resistor value are measured, ADC-estimated or nominal. Run `python Module03/analyze.py repeat RUN.csv OUT --kind ldr` to overlay directions and calculate the maximum gap for each sweep. Flag rail codes before deriving resistance. Any observed gap should be interpreted in light of timing, prior illumination and the reproducibility of the card positions.

## Conclusion

The potentiometer test measured marked-position return spread, with individual sources unresolved. The temperature test measured short-run repeatability and quantization on one probe. The LDR directional comparison remains pending.
