# Module 03 bench guide

## Analog wiring and capture

With USB disconnected, confirm GPIO34 and GPIO35 are free. Both are input-only ADC1 pins on the classic ESP32. Use only the ESP32 3V3 and GND rails. Measure with the DMM in DC voltage mode, black probe at common GND. Inspect connections before reconnecting USB. The older firmware may still be installed, so verify the serial port and upload the intended sketch after checking the wiring.

| Branch | Wiring | ADC |
|---|---|---|
| LDR | 3V3 → LDR → junction → 10 kΩ → GND | Junction → GPIO34 |
| Potentiometer | Outer terminals → 3V3 and GND | Wiper → GPIO35 |

Use [analog_capture.ino](code/analog_capture/analog_capture.ino), ESP32 Dev Module, and Serial Monitor at 115200 baud. Send one case-sensitive command at a time. Save a separate capture per run. Timestamp spacing targets 20 ms; use recorded times for latency.

| Command | Capture |
|---|---|
| `l` / `p` | 32 LDR / pot samples. Pot mode also records a separate `analogReadMilliVolts` baseline conversion. |
| `t` | 150 LDR samples for one settled Task 1 condition |
| `L` / `P` | 500 LDR / pot samples |
| `S` | 1000 pot samples for a step or a slow threshold sweep |

1. **LDR**. Fix lamp geometry and shield ambient light. Measure rail voltage and six settled light levels. Record DMM Vout, mean raw ADC, distance and lux if available. Independently repeat one level three times. Pick two levels to fit Task 3b and a separate level for validation. Reuse the wiring for card sweeps in both directions at two speeds and with pauses. Record matched light levels, timing and dwell. Take one fixed-light 500-sample log.
2. **Pot**. Pick two separated wiper voltages within the board's usable ADC range and a fresh third voltage. Save the raw mean, API voltage baseline and DMM reference at each. Return to a marked position ten times and record each return. Hold it fixed for 500 samples. In a separate long capture, hold one plateau, move quickly between two marked voltages, then hold the new plateau. Note the onset relative to the logger and retain both plateaus. Take a separate slow sweep around a chosen threshold to compare decisions.
3. **Quality checks**. A 3.3 V rail is a supply specification. It does not define the ADC full-scale voltage. For the classic ESP32 the documented 11 dB measurable range is approximately 0.15–3.10 V. Check the useful linear band with the DMM and flag clipped or nonlinear data. Treat the pot noise as an electrical-chain baseline and compare at similar ADC levels and bandwidth. Source impedance, light variation and operating point limit attribution of excess noise to the LDR alone.

## DS18B20 session

Use one confirmed, externally powered DS18B20 probe, a 4.7 kΩ resistor, reference thermometer, stirred cold and warm water, and dry mounting for electronics and connectors. Additional probes are optional. Verify the actual probe pinout. Wire each VDD to 3V3, each GND to common ground, and each DQ to GPIO18. Add one 4.7 kΩ pull-up from DQ to 3V3 unless the breakout already provides it. Check GPIO18 is free before upload. DS1302 uses a different interface and cannot provide these temperature measurements.

Open [ds18b20_capture.ino](code/ds18b20_capture/ds18b20_capture.ino). Use Serial Monitor at 115200 baud. Send `r` to list ROMs. Use `9`, `a`, `b`, `c` for ten conversions at 9, 10, 11 and 12 bits respectively. Check all ROMs are present and retain read errors. Use matching probe IDs for reference and resolution comparisons. Repeat the 9-bit and 12-bit runs at the same stable point for Task 4. Log outdoor locations with timestamps and a co-located reference.

For step tests use `s` at 12 bits. Establish the cold baseline, send `m` at the transfer into warm water and wait for a stable tail. Repeat in still air and document mounting, temperature difference and transfer timing. Only sealed probe tips go into water. Keep connectors and ESP32 dry. See the [Task 2 report and capture procedure](reports/task02-ds18b20.md).
