# Task 2 — RGB LED with a serial `R G B` REPL

A common-cathode RGB LED driven by three PWM channels, controlled by typing
three integers into the serial monitor.

## Wiring

| Channel | GPIO | Resistor | RGB LED pin |
|---|---|---|---|
| Red | **13** | 220 ohm | red leg |
| Green | **14** | 220 ohm | green leg |
| Blue | **27** | 220 ohm | blue leg |
| Common | — | — | **GND** (common cathode) |

**Pin deviation.** The course module specifies GPIO 16 / 17 / 18. This board
does not break out GPIO 16, so the channels are on GPIO 13 / 14 / 27. GPIO 13 is
also used by Task 1's single LED — unplug that LED before measuring this task.

For a common-anode part, wire the common pin to 3V3 instead of GND and set
`COMMON_ANODE = true` at the top of `src/main.cpp`; the duty cycle is then
inverted so that 255 still means full brightness.

## Commands

| Input | Effect |
|---|---|
| `255 0 0` | red only |
| `0 255 0` | green only |
| `0 0 255` | blue only |
| `255 255 255` | all channels fully on |
| `128 128 128` | mid-level grey |
| `off` | every channel off |
| `300 0 0` | rejected: value outside 0..255 |
| `255 0` | rejected: needs exactly three integers |
| `hello` | rejected: not numeric |

The firmware echoes the applied values (`applied R=… G=… B=…`) so the serial log
is its own evidence, and prints an error line for anything it rejects.

## Build, flash, monitor

```sh
cd Module05/task2-rgb
pio run -t upload
pio device monitor -b 115200
```

`platformio.ini` sets `monitor_speed = 115200` and `monitor_echo = yes` so typed
characters are echoed locally.

**Deliberate deviation:** the course prompt asks for
`monitor_filters = send_on_enter`, but that filter only transmits once its buffer
ends with CRLF, while a Windows / VS Code terminal sends a bare CR on Enter. The
result was a monitor that accepted typing and transmitted nothing. The filter is
therefore not enabled, and the firmware accepts CR, LF and CRLF as line endings
instead.

## What to measure

Run each channel alone at full duty and measure the current, either with the
meter in series in that channel or by measuring the voltage across its 220 ohm
resistor and applying `I = U / R`.

| Channel | `Vf` (typical) | Current at 220 ohm, 3.3 V | Measured |
|---|---|---|---|
| Red | ≈1.9 V | ≈6 mA | pending |
| Green | ≈2.1 V | ≈5 mA | pending |
| Blue | ≈3.0–3.4 V | ≈1 mA or less | pending |

The three currents differ a lot for the same resistor, because the forward
voltages differ. That imbalance is the lesson of this task, not a fault.
