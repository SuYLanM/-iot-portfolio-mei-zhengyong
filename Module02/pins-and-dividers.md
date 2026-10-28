# Task 1 — Simple Pins, PWM, UART and Voltage Dividers

**Prepared theory and code; all Module 02 voltage/current/UART measurements Pending.** Sources: [ESP32 electrical limits](references.md#r2-esp32-electrical), [Wire/UART APIs](references.md#r3-arduino-wire-and-uart).

## Baseline Comparison

Signal counts exclude power/ground; every connection still needs the appropriate reference/return path. Speeds are example configurations, not measurements or universal wire-length limits.

| Connection | Signal pins | Electrical behavior / voltage | Speed and length | Typical use | Failure mode |
|---|---|---|---|---|---|
| Digital GPIO | 1 per signal | 3.3 V logic here; output push-pull or specified input pull | Application-defined; short wiring first | Button, LED, driver enable | Floating input, reversed polarity, load overcurrent, 5 V input |
| PWM | 1 command line | Repeated 0/3.3 V pulses; duty or pulse timing carries command | Frequency/load-specific; 1 kHz is our proposed test, not measured | LED dimming; servo pulse commands | Wrong frequency, duty interpretation, no shared ground; powering load from GPIO |
| Analog / ADC | 1 voltage input | Divider/conditioning feeds ADC; range and accuracy depend on attenuation | Sample rate and settling matter; avoid long noisy leads | NTC/LDR/conditioned sensor voltage | Overrange, high source impedance, noise/aliasing, wrong scaling |
| UART logic | TX + RX (optional flow control) | 3.3 V single-ended push-pull, no clock wire | Proposed 115200 baud, 8N1; start with a short jumper | Console, GPS or another MCU | TX/TX connection, wrong baud/parity, no ground, RS-232 levels applied directly |

PWM is modulation, not a shared addressed/framed bus. Servo position pulses are not simply an average-power voltage. A digital sensor or UART at 5 V requires compatible translation; a divider does not translate both directions of I2C.

## Divider Calculation — Not a Measurement

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

| Date / board / operator | Actual R1/R2 | Measured Vin | Measured Vout | Calculated Vout | Measured current / method | Meter / uncertainty | Evidence |
|---|---|---|---|---|---|---|---|
| | | | | | | | |

| UART wiring / baud / parity | Sent line | Actual received line | Observation / fault | Capture link |
|---|---|---|---|---|
| | | | | |

No physical voltage, current, UART success or group roles are inferred from prepared code.
