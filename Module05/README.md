# Module 5 — LEDs and Light as Output

## Module Overview

Light is the actuator used most often in this course. This module starts with a
single LED driven by PWM, then moves to an RGB LED with a serial `R G B` REPL,
and finally to an 8-pixel WS2812B strip driven by `NeoPixelBus` through an ESP32
hardware method. Every build is a PlatformIO project against
`board = mhetesp32minikit`.

| Task | Deliverable | Status | Evidence |
|---|---|---|---|
| [1 — One LED, PWM fade](#task-1--one-led-pwm-fade) | [`task1-led/`](task1-led/) | Built, flashed, serial verified | [Serial capture](#verified-serial-output--task-1); photo pending |
| [2 — RGB LED and `R G B` REPL](#task-2--rgb-led-and-serial-repl) | [`task2-rgb/`](task2-rgb/) | Built, flashed, R/G/B/white verified on hardware; photographic measurements added | [Nine original photos and current analysis](task2-rgb/README.md#photographic-measurements-and-branch-current-calculation); blue assignment and complete per-channel currents pending |
| 3 — 8-pixel WS2812B strip | `task3-strip/` | Not started | — |

## Equipment

| Item | Detail |
|---|---|
| Board | MH-ET LIVE ESP32 MiniKit (`mhetesp32minikit`); chip reported as ESP32-D0WD-V3 rev v3.1, 4 MB flash |
| Host link | CP210x USB-to-UART bridge, enumerated as `COM8` |
| Indicator | 5 mm LED (module specifies red or yellow for this task) |
| Resistor | 220 ohm in series with the LED |
| Toolchain | PlatformIO Core 6.2.0, `espressif32` 8.0.0, Arduino core 3.3.12 |

## Wiring — Task 1

| From | Via | To |
|---|---|---|
| GPIO 13 | 220 ohm | LED anode (long leg) |
| LED cathode (short leg, flat side of the body) | direct wire | GND |

The resistor is not optional: with the LED directly across 3V3 the forward
current is limited only by the pin and the LED, which destroys one or both.

**Pin deviation from the course module.** The module specifies GPIO 16 for the
fade. This board does not break out GPIO 16, so the fade is on **GPIO 13**
instead. GPIO 13 is a general-purpose output, is not a strapping pin, and is not
shared with the SPI flash, so it behaves like GPIO 16 for PWM. The same
substitution carries into the later tasks: the planned mapping is R = GPIO 13,
G = GPIO 14, B = GPIO 27, and WS2812B data = GPIO 13.

## Wiring — Task 2

| Channel | GPIO | Via | RGB LED pin |
|---|---|---|---|
| Red | 13 | 220 ohm | red leg |
| Green | 14 | 220 ohm | green leg |
| Blue | 27 | 220 ohm | blue leg |
| Common | — | direct wire | GND (common cathode) |

Each channel needs its own 220 ohm resistor: a shared resistor couples the
channels, so "red only" would light the other two as well. Task 2 reuses GPIO 13
from Task 1, so the single LED has to be unplugged first.

The part in use behaves as a **common cathode** build (`COMMON_ANODE = false`). A
common-anode part wires its common pin to 3V3 and needs the flag set to `true`,
which inverts the duty so that 255 still means full brightness.

## Build and flash

```sh
cd Module05/task1-led      # or Module05/task2-rgb
pio run -t upload          # build and flash
pio device monitor -b 115200
```

The sketch starts with `#include <Arduino.h>`: PlatformIO compiles `.cpp` files
as C++ and does not add it, while the Arduino IDE does.

## Verified serial output — Task 1

Captured from `COM8` at 115200 baud after a reset of the flashed board:

```text
rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
...
entry 0x4008059c
Task 1: PWM fade on GPIO 13
```

The final line is the `Serial.println(LED_PIN)` in `setup()`, so it also confirms
the flashed binary is the one built for GPIO 13.

## Study log

### Task 1 — one LED, PWM fade

The fade raises the PWM duty cycle from 0 to 255 in steps of 8 ms and then
brings it back down, so the LED brightens and dims smoothly; **duty cycle is the
fraction of each PWM period during which the output is high, and the LED's
average current — and therefore its apparent brightness — follows that
fraction.** A linear ramp in duty looks faster at the dim end than at the bright
end, which is why gamma correction appears further down this module.

Open points for this task:

- Measured forward current for the LED at full brightness (multimeter in series,
  or the voltage across the 220 ohm resistor via `I = U / R`) — still to measure.
- Photograph or short video of the fade on the real circuit — still to record.

### Task 2 — RGB LED and serial REPL

The firmware reads three integers over serial and drives three PWM channels. It
echoes the applied values and rejects out-of-range values, wrong arity and
non-numeric input, so the transcript doubles as evidence. Verified by typing
`255 0 0`, `0 255 0`, `0 0 255` and `255 255 255`: each lit only the expected
channel, and `off` cleared all three.

Two defects surfaced while bringing the task up, both in the input path rather
than in the LED wiring:

1. **A stray NUL byte made an empty Enter look like a parse error.** The byte that
   arrives when the port opens was stored in the input buffer; `String::length()`
   counted it, `trim()` did not remove it (NUL is not whitespace) and `c_str()`
   truncated at it, so `sscanf` saw an empty string and the error path printed
   `''`. Fixed by discarding every byte outside printable ASCII.
2. **`monitor_filters = send_on_enter` transmitted nothing on this host.** That
   filter buffers keystrokes and only flushes when the buffer ends with CRLF, but
   a Windows / VS Code terminal sends a bare CR on Enter, so the device never
   received a byte and the monitor looked frozen. Fixed by dropping the filter and
   accepting CR, LF and CRLF in the firmware.

Observation carried into the power work: at the same duty value the red channel is
visibly brighter than green and blue. Forward voltage affects branch current,
but optical efficiency and visual sensitivity also influence perceived brightness.
The [photographic measurement analysis](task2-rgb/README.md#photographic-measurements-and-branch-current-calculation)
now records nine original images: green gives approximately 4.5 mA using the
documented nominal 220 ohm resistor. Near-white gives an estimated 3.2 mA in one
unidentified branch, with an unresolved 0.2 V voltage-balance difference. The blue
photo assignments are inconsistent and need confirmation before a validated
current can be reported. These images do not document their exact PWM commands.

## Evidence to add

1. Photo or video of the fade with the 220 ohm resistor visible.
2. Measured current at full duty (Task 1); red-only current and confirmed blue/per-channel measurements (Task 2).
3. Red-only RGB photo or video; green, blue and near-white measurement photographs are already linked in the Task 2 report.
4. Confirm blue probe-node assignments, the measured near-white branch, and PWM settings for a complete Task 2 comparison.
