# Task 1 — Simple Pins, PWM, UART and Voltage Dividers

**Prepared / Awaiting hardware validation. All Module 02 voltage/current/UART measurements remain Pending.** Sources: [ESP32 electrical limits](references.md#r2-esp32-electrical), [Wire/UART APIs](references.md#r3-arduino-wire-and-uart).

## Baseline Comparison

Signal counts exclude power/ground; every connection still needs the appropriate reference/return path. Speeds are example configurations, not measurements or universal wire-length limits.

| Connection | Signal pins | Electrical behavior / voltage | Speed and length | Typical use | Failure mode |
|---|---|---|---|---|---|
| Digital GPIO | 1 per signal | 3.3 V logic here; output push-pull or specified input pull | Application-defined; short wiring first | Button, LED, driver enable | Floating input, reversed polarity, load overcurrent, 5 V input |
| PWM | 1 command line | Repeated 0/3.3 V pulses; duty or pulse timing carries command | Frequency/load-specific; 1 kHz is our proposed test, not measured | LED dimming; servo pulse commands | Wrong frequency, duty interpretation, no shared ground; powering load from GPIO |
| Analog / ADC | 1 voltage input | Divider/conditioning feeds ADC; range and accuracy depend on attenuation | Sample rate and settling matter; avoid long noisy leads | NTC/LDR/conditioned sensor voltage | Overrange, high source impedance, noise/aliasing, wrong scaling |
| UART logic | TX + RX (optional flow control) | 3.3 V single-ended push-pull, no clock wire | Proposed 115200 baud, 8N1; start with a short jumper | Console, GPS or another MCU | TX/TX connection, wrong baud/parity, no ground, RS-232 levels applied directly |

PWM is modulation, not a shared addressed/framed bus. Servo position pulses are not simply an average-power voltage. A digital sensor or UART at 5 V requires compatible translation; a divider does not translate both directions of I2C.

## Experiment Objectives and Scope

Classify GPIO, PWM, analog and UART connections; prepare an ADC voltage-divider check; and independently compare a real divider voltage with theory using a multimeter. The [official Task 1](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/02-i2c-and-sensor-addressing.md) requires a **5 V sensor signal divided for a 3.3 V ADC, with a DMM measurement**. Two experiments are kept distinct:

| Experiment | Purpose and nominal design | Status |
|---|---|---|
| A — preliminary baseline | 3.3 V supply, fixed R1=10 kΩ / R2=10 kΩ, calculated Vout=1.650 V | Prepared / Awaiting hardware validation |
| B — original course task | 5 V signal, proposed conservative R1=10 kΩ / R2=12 kΩ (1%), calculated Vout=2.727 V | Pending suitable resistors, bounded 5 V source and multimeter; not performed |

A does not satisfy or replace B. No new serial capture, wiring photo or measurement has been supplied for either divider experiment.

## A — 3.3 V Fixed-Resistor Baseline

[GPIO35 Arduino source](code/task01_voltage_divider/task01_voltage_divider.ino) · [Parts, wiring and run instructions](code/task01_voltage_divider/README.md).

```text
ESP32 3V3 ---- R1 = 10 kΩ ----+---- R2 = 10 kΩ ---- ESP32 GND
                             |
                         Vout node
                             |
                     ESP32 GPIO35 (ADC1_CH7)
```

This circuit uses two fixed resistors, not a potentiometer. GPIO35 connects only to their shared Vout junction. With exactly 3.300 V and ideal resistors:

`Vout = Vin × R2 / (R1 + R2) = 3.300 × 10 / (10 + 10) = 1.650 V`.

The ideal divider current is `3.300 / 20000 = 0.165 mA`; neither number is a measurement. With a fixed 3.300 V supply and independent ±1% resistors, the calculated worst-case output is 1.6335–1.6665 V. Confirm the actual resistor tolerance and supply; those assumptions are not yet verified. The divider's nominal Thevenin resistance is 5 kΩ, so input loading and ADC acquisition settling must also be considered.

### GPIO35 Configuration and 32-Sample Averaging

The program targets a **classic ESP32 Dev Module**: GPIO35 is input-only ADC1_CH7, without internal pull resistors. Confirm the actual board pinout. It includes `Arduino.h`, sets `ADC_PIN=35`, calls `analogReadResolution(12)` (raw 0–4095) and `analogSetPinAttenuation(ADC_PIN, ADC_11db)`. The [Espressif ADC API reference](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html) distinguishes uncalibrated raw counts from API-calibrated millivolts and lists a classic ESP32 11 dB measurable range of approximately 0.15–3.10 V. That measurement range is not an absolute maximum rating or permission to apply 5 V.

Each round takes **32 `analogRead()` conversions and 32 separate `analogReadMilliVolts()` conversions**. The means are `sum(raw)/32` and `sum(API_mV)/32/1000` in volts. These are two independent sample sets, not the same conversions. A 5 ms delay follows each pair; the burst takes at least about 160 ms. Wrap-safe `millis()` scheduling starts a burst approximately once per second, including acquisition time. This does not establish a precision sampling frequency or simultaneous measurement with a meter.

`Serial.begin(115200)` prints raw mean and API voltage estimate with three decimals. Use USB Serial Monitor at **115200 baud, 8N1**, with the actual detected port; no console input or loopback jumper is used by this ADC program. UART0 is only the reporting channel here, and the separate UART loopback task remains distinct. A near-zero raw mean (<5) or near-full-scale mean (>4090) produces a wiring/input warning. These checks do not catch every fault or prevent overvoltage.

### Expected Results Versus Real Results

**Expected (calculated only):** Vout approximately 1.650 V for the nominal circuit. A raw midpoint of exactly 2048 is not an accuracy requirement. ADC range, reference/calibration and nonlinearity mean that `raw × 3.3 / 4095` must not be described as an accurate physical voltage. The program instead averages API millivolts and explicitly labels the result an **API voltage estimate**.

**Actual:** Pending. No ADC means, measured voltages, successful upload, photographs or test pass are claimed. Three displayed decimals are a formatting choice, not evidence of millivolt accuracy. Averaging can reduce some random noise; it cannot remove systematic gain/offset errors, supply drift or resistor error.

### Measurement Method, Comparison and Limitations

1. Power off; verify the real board pinout and the two resistors. Measure resistor values only in an unpowered circuit with parallel paths excluded. Record the meter model, range, resolution and stated uncertainty.
2. Assemble the divider with GPIO35 initially disconnected. Power the ESP32/divider, measure 3V3-to-GND and Vout-to-GND in **voltage mode**, and check that the node is safe. Power off before connecting GPIO35.
3. Compile/upload the separate ADC sketch after the wiring checks, then record actual serial values and DMM readings with timestamps. Compare Vout before/after attaching the ADC to investigate loading. Save authentic captures and photos later.
4. Predict `Vpred = measured_Vin × measured_R2/(measured_R1 + measured_R2)`. Record the signed residual `DMM_Vout − Vpred` and relative difference `100 × (DMM_Vout − Vpred)/Vpred` when Vpred is nonzero. Compare API estimate with DMM independently: `API_V − DMM_Vout`. Do not substitute API-derived voltage for a DMM reading.
5. Interpret differences against resistor/supply variation, DMM uncertainty/loading, ADC calibration/nonlinearity, noise and settling. Record time separation and circuit loading; sequential samples and a separate meter are not simultaneous reference measurements. If current is calculated from voltage/resistance, label it calculated. Any direct current measurement requires a safe series connection, correct meter range and sockets; never place a current-mode meter across a supply.

Only use known low-voltage sources, common ground and short connections. Attenuation changes ADC scaling; it does not clamp/protect the GPIO. Do not exceed the actual board/chip input limits, drive an unpowered ESP32 input, or connect GPIO35 directly to 5 V. Power off for every wiring change. No mains-referenced or unknown transient source is appropriate.

## B — Original 5 V Divider Task: Pending

The following is a proposed design calculation, **not a completed 5 V experiment**.

```text
+5 V signal --- R1 (upper) --- node to ADC
                              |
                              R2 (lower)
                              |
                             GND
Vout = Vin × R2/(R1+R2)
```

Professor's nominal example: R1=10 kΩ, R2=20 kΩ gives **3.333 V at 5.000 V**. This exceeds nominal 3.3 V, although it does not by itself prove the 3.3 V I/O absolute limit has already been exceeded. Absolute/stress limits are not intended operating targets, depend on the I/O supply domain, and do not define an accurate ADC range. ESP32 is not a 5 V-tolerant input.

Example tolerance checks (calculated, not observed):

| Design / assumptions | Calculated output |
|---|---|
| 10k / 20k, Vin=5 V, nominal | 3.333 V |
| Same, Vin=5.25 V, upper −1%, lower +1% | 3.523 V |
| Same, Vin=5.25 V, upper −5%, lower +5% | 3.615 V |
| Conservative 10k / 12k, Vin=5 V, nominal | 2.727 V |
| Conservative, Vin=5.25 V, upper −1%, lower +1% | 2.890 V |
| Conservative, Vin=4.75 V, upper +1%, lower −1% | 2.567 V |

The proposed **10 kΩ upper / 12 kΩ lower, 1%** divider assumes the input stays within 4.75–5.25 V. It keeps the computed upper value below 3.0 V, with ADC headroom. At 5 V the divider current is 5/22000≈0.227 mA; Thevenin resistance is about 5.45 kΩ. These are nominal calculations only. Check input loading, ADC settling, resistor tolerances, actual supply, faults/transients and powered-off backfeeding. It is not a universal protection circuit or a bidirectional level shifter. A digital receiver additionally needs guaranteed HIGH/LOW thresholds at the actual source's worst-case VOH/VOL; these assumptions do not guarantee compatibility with every 5 V logic family.

**Before connecting the node to ESP32:** measure resistor values and node-to-ground voltage with the ESP32 input disconnected; compare with the actual board datasheet. Power off for rewiring. Do not reuse this untested divider for mains or unknown transients.

## UART Experiment Plan — Pending

[UART loopback source](code/task01_uart_loopback/task01_uart_loopback.ino) · [English instructions](code/task01_uart_loopback/README.md).

On a verified classic WROOM board, use UART2 RX16/TX17 and a short TX17→RX16 jumper; UART0 remains the USB Serial Monitor at 115200 baud, 8N1. GPIO16/17 can be unavailable/reserved on other modules (notably some PSRAM boards); confirm first. The code sends test lines every two seconds and prints a match only if the received line equals the sent token. User-entered lines can also be sent; enable Newline. It never echoes RX back to TX indefinitely.

Save a no-jumper and loopback run with actual settings. A genuine mismatched UART-peer baud test needs an independently configured second UART/device; a wrong Serial Monitor setting tests only console decoding and must be labelled accordingly. Garbled/truncated or absent text suggests a baud/framing/wiring mismatch but is not a unique diagnosis. A logic analyzer can distinguish bit timing from wiring faults. No Module 02 UART result is recorded yet.

For a 20 m multi-drop run, an **RS-485 transceiver adds a differential electrical link and controlled driver enable**, not automatic device addressing or Modbus framing; choose compatible transceivers, cable, reference/common-mode management and termination. Source: [RS-485 guide](references.md#r6-rs485).

Dividers also convert resistive sensors to voltage and scale references; their error/loading and lack of isolation matter.

## Actual Results — Pending

Populate these tables only after real measurements. Leave unknown fields blank; attach evidence links only after the genuine files exist. Record operator/date, board, IDE/core/port, resistor markings/tolerances, DMM model/range/uncertainty and measurement conditions with each run. All values below are intentionally blank.

| Experiment / run | Measured R1 (Ω) | Measured R2 (Ω) | DMM Vin (V) | DMM Vout, ADC disconnected (V) | DMM Vout, ADC connected (V) | Vpred from measured parts (V) |
|---|---|---|---|---|---|---|
| A — 3.3 V baseline | | | | | | |
| B — original 5 V task | | | | | | |

| Experiment / timestamp | Raw mean (32 samples) | API voltage estimate (32 samples, V) | Independent DMM Vout (V) | API − DMM (V) | DMM − Vpred (V / %) | Genuine evidence / notes |
|---|---|---|---|---|---|---|
| A | | | | | | |
| B | | | | | | |

The existing general voltage/current and UART record templates remain available:

| Date / board / operator | Actual R1/R2 | Measured Vin | Measured Vout | Calculated Vout | Measured current / method | Meter / uncertainty | Evidence |
|---|---|---|---|---|---|---|---|
| | | | | | | | |

| UART wiring / baud / parity | Sent line | Actual received line | Observation / fault | Capture link |
|---|---|---|---|---|
| | | | | |

No physical voltage, current, UART success or group roles are inferred from prepared code.

## Remaining Task 1 TODO

- [ ] Hardware-test A using the two real 10 kΩ resistors, GPIO35 and an independent DMM; record real observations and evidence.
- [ ] Obtain/check the proposed 1% 10 kΩ / 12 kΩ resistors, a known 5 V signal/source and its worst-case voltage (assumed 4.75–5.25 V here), and a multimeter for B.
- [ ] Confirm resistor tolerances, source transients, board input limits and loaded node voltage before connecting B to GPIO35; revise the design if the assumptions do not hold.
- [ ] Perform the professor's original 5 V divider measurement, compare DMM Vout with the calculation, and record uncertainty and authentic evidence.
- [ ] Perform the separate UART line experiment and record the real baud/wiring observation; prepared Serial Monitor output alone does not establish UART loopback success.
