# Module 4 – Experiment 2: Capacitive Liquid Level Measurement

## 1. Objective

Evaluate how the analog output of a capacitive probe can be used to distinguish liquid levels. Examine the direction and sensitivity of the depth response, short-term variation, and the requirements for a usable calibration.

## 2. Equipment

- ESP32 development board
- Capacitive Soil Moisture Sensor V1.2
- Breadboard and jumper wires
- Transparent plastic cup and water

## 3. Circuit Connection

| Sensor terminal | ESP32 connection |
|---|---|
| VCC | 3V3 |
| GND | GND |
| AOUT | GPIO34 |

The [Arduino sketch](Module4_Task2_Capacitive.ino) configures GPIO34 as an input and reads the 12-bit ADC every 500 ms. The serial output contains the elapsed device time in milliseconds and the raw ADC count at 115200 baud. Counts are used directly because the input voltage was not independently calibrated. [Arduino-ESP32 ADC documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html)

## 4. Experimental Procedure

1. Connect the sensor and upload the sketch. Confirm the serial stream and sampling interval in air.
2. For a depth sweep, hold the cup and probe in a fixed orientation. Use air and tip contact as baseline conditions, followed by 1 cm, 2 cm, 3 cm, and a higher depth within the probe's rated immersion range. Keep the electronics and connector above the water. A return-to-air condition completes the sequence.
3. Once each condition is stable, log a 10 s window. Compute the arithmetic mean, minimum, maximum, and sample standard deviation. Compare the depth windows and the two air conditions.

**Data source.** The table and figures in Section 5 use the existing [synthetic CSV](simulation/synthetic-capacitive-water-level.csv). It contains 147 modeled ADC values across seven windows, each with 21 samples spaced 500 ms apart and 10 s from first to last. The model assigns a decreasing response with depth, adds random variation, and includes a hypothetical 4 cm level. These values were generated separately from the bench records and provide a dataset for demonstrating the analysis. The hypothetical depth does not establish an immersion rating for this board.

**Bench check.** The sketch was compiled and uploaded, and COM3 returned serial samples at 500 ms intervals. The [air retest](data/test-A-air-retest-20261111-171317-334.csv) and the [approximately 1 cm diagnostic](data/diagnostic-water-1cm-20261111-171908-796-deduplicated.csv) each contained 21 distinct samples. Every ADC value in both windows was 0. Duplicate lines in the original capture were counted once per device timestamp in the diagnostic summary. The bench test stopped at this point. A nonzero output and a valid hardware liquid-level calibration remain unverified.

## 5. Results and Data Analysis

| Condition | n | Mean ADC | Minimum | Maximum | Sample SD |
|---|---:|---:|---:|---:|---:|
| Air | 21 | 3150.48 | 3135 | 3163 | 6.50 |
| Initial water contact | 21 | 3047.38 | 3024 | 3059 | 9.76 |
| 1 cm immersion | 21 | 2552.43 | 2536 | 2573 | 9.08 |
| 2 cm immersion | 21 | 2179.24 | 2162 | 2203 | 9.49 |
| 3 cm immersion | 21 | 1913.24 | 1899 | 1930 | 8.26 |
| Hypothetical 4 cm immersion | 21 | 1720.14 | 1696 | 1737 | 10.15 |
| Removed from water | 21 | 3117.48 | 3098 | 3138 | 11.51 |

![Mean ADC and sample standard deviation by immersion depth](images/adc_vs_depth.png)

The mean decreases monotonically from tip contact through 4 cm. The decrease per 1 cm step becomes smaller, falling from about 495 counts between contact and 1 cm to 193 counts between 3 and 4 cm. This indicates a declining sensitivity in the modeled response. The adjacent depth windows have nonoverlapping ranges in this dataset.

![Raw ADC counts across the seven sample windows](images/adc_over_time.png)

The time plot places the seven independent windows on a common display axis. Within-window times come from the CSV, and spacing between windows is for display. The return-to-air mean is 33 counts below the initial air mean. Since the model assigns different baseline values to those conditions, the difference cannot establish a physical residual film, drift, or hysteresis.

## 6. Discussion

A capacitive probe creates an electric field around its electrodes. Water changes the effective permittivity within that field as more of the probe is wetted, changing the capacitance sensed by the circuit. The module converts its electrical response to an analog output, which the ESP32 digitizes. The output polarity and sensitivity depend on the electronics and geometry. The probe's response in a cup may also depend on water composition, probe position, nearby surfaces, and the presence of residual moisture. [DFRobot capacitive sensing reference](https://wiki.dfrobot.com/sen0193/docs/18035) · [Texas Instruments capacitive liquid-level sensing reference](https://www.ti.com/lit/ds/symlink/fdc1004.pdf)

Sample standard deviations range from 6.50 to 11.51 ADC counts in the supplied windows. They quantify the short-term variation in the dataset. A 12-bit output code does not establish voltage accuracy, depth accuracy, or minimum resolvable depth. A usable depth calibration would require nonzero hardware readings, independent depth measurements, repeated rising and falling sweeps, and validation at depths excluded from the fit. Cup geometry, probe placement, and water conditions should be controlled during calibration. The smaller changes per centimetre at higher modeled levels suggest that a single linear conversion could introduce depth-dependent error.

## 7. Conclusion

The analysis demonstrates a monotonic but nonlinear ADC-to-depth relationship for the supplied conditions. A probe with a repeatable response of this kind could support continuous level estimates over a validated range. The bench readings did not resolve the two tested conditions, so continuous liquid-level measurement and hardware calibration remain unverified for this setup.
