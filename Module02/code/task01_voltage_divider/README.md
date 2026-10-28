# Task 1 — Fixed-Resistor Voltage Divider ADC

**Prepared / Awaiting hardware validation.** No upload, ADC result, multimeter reading or photograph is recorded. This 3.3 V baseline does **not** replace the professor's original 5 V divider measurement.

[Arduino sketch](task01_voltage_divider.ino) · [English report and blank measurements](../../pins-and-dividers.md) · [Module navigation](../../README.md)

## Parts and Connections

- Classic ESP32 Dev Module with verified GPIO35 / ADC1_CH7 pinout, USB cable and Arduino IDE.
- Two fixed 10 kΩ resistors (R1 and R2); confirm their actual values and tolerances. No potentiometer is needed.
- Breadboard, jumper wires and a multimeter for independent voltage checks.

```text
ESP32 3V3 ---- R1 = 10 kΩ ----+---- R2 = 10 kΩ ---- ESP32 GND
                             |
                         Vout node
                             |
                     ESP32 GPIO35 (ADC1_CH7)
```

| Terminal | Connect to |
|---|---|
| R1 upper end | ESP32 3V3 supply, not a GPIO output |
| R1 lower end and R2 upper end | Same Vout breadboard node |
| R2 lower end | ESP32 GND |
| GPIO35 | Vout node after independent voltage check |
| Multimeter COM / V probe | GND / Vout, voltage mode |

Power off before wiring. Initially leave GPIO35 disconnected, power the divider, and check Vout with a multimeter. Power off again before attaching GPIO35. GPIO35 is input-only with no internal pull resistors; it must not float or be driven directly by 5 V. Verify the actual board pinout and rails before proceeding.

## Arduino IDE Procedure

1. Open `task01_voltage_divider.ino` in this same-name folder. Keep the UART loopback sketch separate.
2. Select **ESP32 Dev Module**, installed Espressif **Core 3.3.12**, and the actual USB port. Do not assume the current port is COM3.
3. Verify/compile, then upload only after checking the powered-off circuit and DMM voltage. Open Serial Monitor at **115200 baud, 8N1**. No line-ending option is needed because this sketch takes no console input.
4. Record board, core, port, resistor values, measured supply and Vout, meter model/range and uncertainty, raw means, API voltage estimates and actual observation times in the report. Save genuine serial captures and wiring photographs later; no evidence link is created in advance.
5. Investigate near-zero/full-scale warnings. They are diagnostic hints, not a complete wiring or overvoltage detector. Disconnect power if the node is unsafe; the ADC cannot protect its own input.

## Acquisition and Output

The sketch explicitly selects GPIO35, 12-bit resolution and `ADC_11db`. Each burst separately calls `analogRead()` 32 times and `analogReadMilliVolts()` 32 times, with 5 ms after each pair: **64 conversions**, not two representations of the same conversion. Each set has its own arithmetic mean. Burst starts are scheduled approximately 1000 ms apart using wrap-safe `millis()` subtraction; each burst takes at least about 160 ms. This is slow monitoring, not precise waveform sampling.

The Serial Monitor prints a raw mean and an API voltage estimate in volts, each with three decimal places. Startup text labels **1.650 V as theory only**. There is no fabricated example measurement. API millivolts use the core's calibration path; they remain an estimate to compare against an independent DMM. Averaging may reduce random noise but does not remove systematic errors, guarantee accuracy, or make the displayed decimals significant.

## Expected Versus Actual

At exactly 3.300 V with equal ideal 10 kΩ resistors, the calculated node voltage is **1.650 V**. Actual supply, resistor tolerance, loading, settling and ADC calibration can change observations; a raw value of exactly 2048 is not required. All actual results remain **Pending** in the [report](../../pins-and-dividers.md).

The original 5 V experiment remains a separate TODO. The report proposes 10 kΩ upper / 12 kΩ lower, 1% resistors with an explicitly bounded 4.75–5.25 V source; confirm DMM voltage and board limits before any connection. Neither attenuation nor this unprotected divider permits a direct 5 V GPIO connection.

## Software Validation

**Compile PASS — 2026-10-28**, using the installed Arduino CLI; compilation alone does not establish hardware operation.

- Target: `esp32:esp32:esp32` (classic ESP32 Dev Module), Espressif Core 3.3.12.
- Command: `arduino-cli compile --fqbn esp32:esp32:esp32 --jobs 4 Module02/code/task01_voltage_divider` (run from the repository root; build output was directed to a temporary directory).
- Compiler result: 278744 bytes flash; 22196 bytes global RAM. No board was flashed or measured.
- Hardware validation, independent DMM comparison and original 5 V measurement: **Pending**.
