# Task 2 — DS18B20 resolution, accuracy and response time

**Status: Hardware attempted. COM3 upload and UART verified. The trial scan found no valid DS18B20 ROM.**

## Objective

Compare measured temperature, resolution, conversion time and thermal response with the datasheet and a reference thermometer.

## Wiring and materials

Use an externally powered DS18B20 with verified pinout, an ESP32 Dev Module, a 4.7 kΩ resistor and a reference thermometer. Use a sealed probe for the two stirred water baths. A second or third probe is optional. Disconnect USB and check GPIO18 is free before wiring.

| DS18B20 connection | ESP32 connection |
|---|---|
| VDD | 3V3 |
| GND | GND |
| DQ | GPIO18 |
| Pull-up resistor | 4.7 kΩ between DQ and 3V3. Check for an existing pull-up on a breakout first. |

For multiple probes, parallel VDD, GND and DQ and use one bus pull-up. Verify the probe pinout with the supplier before trusting cable colors.

For the 2026-11-04 trial the operator authorized the assumed mapping red → 3V3, blue → GND and yellow → GPIO18. The wire functions, supply compatibility and physical installation of the 4.7 kΩ pull-up were unverified.

## Datasheet reference

| Resolution (bits) | Step (°C) | Maximum conversion time (ms) |
|---:|---:|---:|
| 9 | 0.5 | 93.75 |
| 10 | 0.25 | 187.5 |
| 11 | 0.125 | 375 |
| 12 | 0.0625 | 750 |

The manufacturer specifies ±0.5 °C from −10 to +85 °C. The specified maximum active current is 1.5 mA at VDD = 5 V, and the standby maximum is 1 µA under the stated datasheet conditions. The code step controls quantization and conversion time limits logging cadence. At 12 bits, the accuracy specification is larger than one code step. The probe assembly and medium determine the thermal lag. [Analog Devices datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/DS18B20.pdf)

## Capture and reference comparison

Open [the DS18B20 sketch](../code/ds18b20_capture/ds18b20_capture.ino) with ESP32 Dev Module selected. Use Serial Monitor at 115200 baud. Send `r` for the CRC-validated 64-bit ROM list. Send `9`, `a`, `b` or `c` for ten conversion cycles at 9, 10, 11 or 12 bits. Each cycle reports sample index, elapsed milliseconds, ROM, resolution, temperature and read status.

Save the original serial transcript under the [evidence index](../evidence/README.md). Copy the CSV header and its rows to one CSV per run. Preserve diagnostic rows and marker messages in the original transcript. Only rows with status `ok` contribute to temperature and timing statistics. A disconnected or invalid scratchpad read has a blank temperature, and an exact 85 °C reading is flagged because it can be the power-on value. Use `python Module03/analyze.py repeat RUN.csv OUT --kind ds` to summarize readings by ROM and resolution.

## Conversion time

The logger measures elapsed milliseconds from the conversion command to the first observed bus-ready poll. The timestamp includes polling overhead and represents all probes in a broadcast cycle. A missing busy signal or timeout leaves conversion time blank. Datasheet limits above are reference values. Actual means and standard deviations are pending.

Place the probe and reference thermometer together in one stable location. Record at least ten readings per unit plus the reference instrument and its uncertainty in the [temperature reference template](../data/temperature-reference.csv). Compare unit means, sample SDs and signed errors against the reference. With one probe, use the class table for the unit comparison when available. For the outdoor transect, record one or two assigned locations, times, ROM, temperatures, co-located reference, height or depth, and conditions. Assess whether the apparent differences exceed the accuracy limits.

## Thermal step and analysis

1. Equilibrate the sealed tip in cold stirred water. Send `s` for 240 conversions at 12 bits and record several stable baseline samples.
2. Move the tip into warm stirred water at comparable depth. Send `m` at transfer, ideally with a second person operating the computer. Keep recording until the temperature reaches a stable plateau. Keep the ESP32 and connectors dry, and use warm water below boiling.
3. Repeat the cold-to-warm step in still air. Keep mounting consistent and record the medium, reference temperatures, setup and any marker delay.
4. Convert the marker milliseconds to seconds, select a stable final window, and run the command below with separate output directories for water and air.

```text
python Module03/analyze.py step RUN.csv OUT --rom ROM --step-s TRANSFER_SECONDS --final-start-s STABLE_TAIL_SECONDS
```

The analysis uses pre-step and tail means and interpolates the first crossing of 63.2% of the observed change after transfer. It reports τ, the approximate 4–5τ settling range and an observed 5% settling time if the trace supports it. Report missing crossings or an unstable tail as unverified. If the run ends before stabilization, lengthen it and repeat. Discuss marker timing, conversion cadence, stirring, probe housing, reference lag and differences in heat transfer.

## Results

The ESP32-D0WD-V3 compiled with ESP32 Core 3.3.12 and uploaded on COM3. The uploader verified the written flash. At 115200 baud, one explicit `r` command returned `DS18B20_COUNT=0` and `NO_DS18B20`. The firmware counts only ROMs with family code 0x28 and a valid ROM CRC. This result does not distinguish absent bus presence from a search or ROM validation failure. The four resolution captures were skipped after discovery failed. No temperature, conversion-time, CSV sample rows or thermal plot were obtained.

**Follow-up after adding a pull-up.** The operator reported adding a resistor between GPIO18 and 3V3. Its resistance and the probe supply polarity remain unverified. Windows enumerated the COM3 USB bridge and the existing sketch compiled offline with ESP32 Core 3.3.12. The serial port was left unopened and no upload or scan was performed in this follow-up. The earlier zero-ROM result applies only to the earlier wiring.

| Evidence | Result |
|---|---|
| ROM scan and plausible first reading | Attempted. Zero valid ROMs. No temperature |
| Four resolutions and ten readings per probe | Pending hardware |
| Reference error and unit comparison | Pending hardware or class data |
| Outdoor transect | Pending field work |
| Water and air plots, τ and settling comparison | Pending hardware |
| Build and synthetic Python checks | [Software verification](../evidence/verification.md) |

### Bench evidence

- [Compile log](../evidence/task02-ds18b20/compile.txt)
- [Upload log](../evidence/task02-ds18b20/upload.txt), with absolute paths and board MAC redacted
- [Original COM3 scan response](../evidence/task02-ds18b20/scan.serial.txt)
