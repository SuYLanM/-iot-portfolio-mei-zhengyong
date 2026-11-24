# Module 5 — LEDs and Light as Output

## Learning Goals

The rows follow the numbering and meaning of the [official module goals](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/05-leds-and-light-output.md).
Status describes the linked evidence, not an instructor assessment.

| Official goal (paraphrased) | How I learned or checked it | Existing evidence | Evidence status |
|---|---|---|---|
| 1. Explain LED physics and choose a current-limiting resistor. | I checked green resistor-drop arithmetic against photographs and kept blue-node ambiguity explicit; all branch resistors are nominal 220 ohm. | [RGB report/photos](task2-rgb/README.md), [current table](#measured-currents-and-calculated-estimates) | Partial; Needs further evidence for red/blue currents and confirmed measurement conditions. |
| 2. Use PWM and explain duty, frequency and perceptual gamma. | I inspected the 0..255 fade and distinguished on-time fraction from voltage; no measured PWM frequency or visual gamma comparison is archived. | [Fade source](task1-led/src/main.cpp), [Task 1 record](task1-led/README.md) | Fade implementation recorded; Needs further evidence for dedicated fade video and frequency/gamma tests. |
| 3. Mix RGB colours and explain additive mixing. | I retained common-cathode control and green/blue/near-white photos, without treating camera colour as calibrated chromaticity. | [RGB source](task2-rgb/src/main.cpp), [original images](task2-rgb/images/), [report](task2-rgb/README.md) | Photographic/control support; Needs further evidence for complete balanced-current/color comparison. |
| 4. Prepare an AI-assisted PlatformIO project and verify its implementation. | I checked actual GPIO constants, project configuration and serial handling; package versions remain unpinned and historical reports are not fresh builds. | [Shared development rules](AGENTS.md), [Task 1 project](task1-led/README.md), [Task 4 audit](#task-4--share-the-repo-audit) | Projects published and checks recorded; exact fresh-environment reproducibility needs further evidence. |
| 5. Address strip pixels and justify a hardware driver suitable for radio-active ESP32 use. | I documented selected-pixel patterns, GRB and NeoPixelBus RMT channel 0; the photos show luminance 96 while current source caps it at 80. | [Strip source](task3-strip/src/main.cpp), [six-photo report](task3-strip/experiment-report.md) | Addressing demonstrated; Needs further evidence for Wi-Fi/BLE stress verification. |
| 6. Compute and measure power demand and choose an adequate USB/supply arrangement. | I compared computed 8/30/60/144-pixel budgets with the lack of validated strip-current measurements; mV indications are not mA. | [Computed/measured budget](#strip-power-budget--computed-versus-measured), [meter photographs](task3-strip/experiment-report.md) | Computed planning complete; Needs further evidence for current, supply capability and margin. |

## Module Overview

Light is the actuator used most often in this course. This module starts with a
single LED driven by PWM, then moves to an RGB LED with a serial `R G B` REPL,
and finally to an 8-pixel WS2812B strip driven by `NeoPixelBus` through an ESP32
hardware method. Every build is a PlatformIO project against
`board = mhetesp32minikit`.

| Task | Deliverable | Status | Evidence |
|---|---|---|---|
| [1 — One LED, PWM fade](task1-led/README.md) | [`task1-led/`](task1-led/) | Built, flashed, serial verified | [Serial capture](#verified-serial-output--task-1); photo pending |
| [2 — RGB LED and `R G B` REPL](#task-2--rgb-led-and-serial-repl) | [`task2-rgb/`](task2-rgb/) | Built, flashed, R/G/B/white verified on hardware; photographic measurements added | [Nine original photos and current analysis](task2-rgb/README.md#photographic-measurements-and-branch-current-calculation); blue assignment and complete per-channel currents pending |
| [3 — 8-pixel WS2812B strip](task3-strip/README.md) | [`task3-strip/`](task3-strip/) | Source archived; selected-pixel and eight-pixel outputs documented | [Six photos and English experiment report](task3-strip/experiment-report.md); current/power not verified |
| [4 — Share the repo](#task-4--share-the-repo-audit) | Unified projects, documentation and evidence | Repository preparation complete; measurements and Discord evidence remain partial | [Audit checklist](#task-4--share-the-repo-audit) |

## Equipment

| Item | Detail |
|---|---|
| Board | MH-ET LIVE ESP32 MiniKit (`mhetesp32minikit`); chip reported as ESP32-D0WD-V3 rev v3.1, 4 MB flash |
| Host link | CP210x USB-to-UART bridge, enumerated as `COM8` |
| Indicator | 5 mm LED (module specifies red or yellow for this task) |
| Strip | 8-pixel WS2812B, pads `VDC` / `GND` / `DIN`, 5 V supply |
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
channel currents and changes colour balance when multiple channels are on. Task 2 reuses GPIO 13
from Task 1, so the single LED has to be unplugged first.

The part in use behaves as a **common cathode** build (`COMMON_ANODE = false`). A
common-anode part wires its common pin to 3V3 and needs the flag set to `true`,
which inverts the duty so that 255 still means full brightness.

## Wiring — Task 3

| Strip pad | Goes to | Note |
|---|---|---|
| `VDC` (5 V supply) | board `5V` | the strip needs 5 V, not 3V3 |
| `GND` | board `GND` | the ground must be shared with the board |
| `DIN` (data in) | **GPIO 13** | the `DOUT` end stays unconnected |

Connect in the order GND, then DIN, then VDC, and only while the board is
unpowered; never plug or unplug the strip while it is powered. Data is
unidirectional, so it must enter at the `DIN` end of the strip. A 330 ohm series
resistor in the data line is optional insurance and is not fitted here.

Task 3 reuses GPIO 13, which already carried Task 1's LED and Task 2's red
channel, so those have to be unplugged first.

## Build and flash

```sh
cd Module05/task1-led      # or task2-rgb, task3-strip
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

### Task 3 — 8-pixel WS2812B strip

The firmware drives eight pixels over a single data line with
`NeoPixelBusLg<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod, NeoGammaNullMethod>` at
`SetLuminance(80)`. It accepts `index R G B` (index 0..7), `all R G B` and
`clear`, echoes every applied change, and rejects out-of-bounds indices,
out-of-range values and wrong arity. Verified over the serial console:
`all 255 255 255` lit the whole strip, `0 255 0 0` lit only the first pixel, and
`8 255 0 0` / `all 300 0 0` were rejected.

Two points the module asks this build to justify:

1. **Driver method.** The RMT peripheral generates the 800 kHz NRZ waveform in
   hardware, reducing dependence on CPU instruction timing compared with
   bit-banging. This follows the library guidance; no Wi-Fi/BLE stress test is
   archived here. The existing record reports a legacy-driver deprecation warning.
2. **Power.** Only the 8-pixel case exists on this bench, so the 30 / 60 / 144
   rows are extrapolated from the per-pixel cost rather than observed. The
   per-pixel cost itself still has to be measured with a meter.

`all R G B` was added specifically for the power measurement: eight separate
per-pixel commands would refresh the strip eight times instead of holding one
steady state.

The [photographic experiment report](task3-strip/experiment-report.md) now
archives six original images: bottom red; bottom red plus top blue/cyan; and
those two colours plus a white-appearing pixel at the third position from the
bottom. Three further images show off, one lit, and all eight lit, with
**0.8, 2.7, and 16.0 mV** meter indications. They do not establish current
without a documented measurement circuit. The photographed banner shows
luminance 96, whereas the archived source uses 80; quantitative comparisons
must preserve this distinction. No fresh build or hardware run was performed
for the photographic report update.

### Task 4 study-log addition

This is a record-based technical reflection, not an invented classroom exchange.
Checking the archived source showed GPIO13 despite an old Task 1 comment saying
GPIO16. Comparing images with source also exposed luminance **96 versus 80**.
The electrical review checked the AI-derived green-current estimate against the
photographed 1.0 V resistor drop and nominal 220 ohm, while rejecting a numerical
strip-current claim because the meter shows mV without a known shunt.
The learning is that a plausible calculation needs correct units, measurement
nodes, operating settings, and source-version context to become evidence.

The portfolio URL to share is
[Module05](https://github.com/SuYLanM/-iot-portfolio-mei-zhengyong/tree/main/Module05).
This is the configured origin (its repository name starts with a hyphen).
**Discord sharing: Pending**. When actually posted, add the message URL or
screenshot; neither a repository push nor this prepared link proves it was shared.
No jointly agreed learning or peer feedback is claimed here.

### Individual review of AI-assisted LED work

I compared source constants with documentation rather than trusting generated
comments: Task 1 drives GPIO13 despite its older GPIO16 comment. The RGB code
uses separate PWM branches and rejects invalid serial values. The measured green
resistor indication supports an approximate current; the blue node assignments
remain inconsistent, and the near-white estimate concerns one unidentified branch.
I kept those limitations rather than filling a complete current table by theory.

Codex helped draft firmware, troubleshoot terminal line endings, organize photos
and calculate budgets. I checked these outputs against source and original images.
The strip photos demonstrate pixel patterns but show mV, not current, and their
luminance 96 differs from source luminance 80. Calculated supply budgets therefore
remain planning estimates. I have not documented a measured gamma comparison,
radio stress test, or calibrated strip power. My [reflection](../Reflections/ref05.md)
adds an individual evidence-based account for my confirmation. Discord posting
and any jointly agreed learning remain pending; this review does not claim them.

## Evidence to add

1. Photo or video of the fade with the 220 ohm resistor visible.
2. Measured current at full duty (Task 1); red-only current and confirmed blue/per-channel measurements (Task 2).
3. Red-only RGB photo or video; green, blue and near-white measurement photographs are already linked in the Task 2 report.
4. Confirm blue probe-node assignments, the measured near-white branch, and PWM settings for a complete Task 2 comparison.
5. Strip off/single/eight-pixel photographs are archived; retain the luminance-96 versus source-luminance-80 distinction.
6. Measured strip current at `SetLuminance(80)`: baseline, one pixel, all eight,
   so the 30 / 60 / 144 rows can be derived rather than only asserted.
7. Actual Discord share link or screenshot and any later measurement records; the existing Study log preserves current limitations.


## Project navigation and serial control

| Task | Source | Build configuration | Development rules | Report and photographs |
|---|---|---|---|---|
| 1 | [main.cpp](task1-led/src/main.cpp) | [platformio.ini](task1-led/platformio.ini) | [AGENTS.md](task1-led/AGENTS.md) | [README](task1-led/README.md); dedicated fade photo/video pending |
| 2 | [main.cpp](task2-rgb/src/main.cpp) | [platformio.ini](task2-rgb/platformio.ini) | [AGENTS.md](task2-rgb/AGENTS.md) | [Report](task2-rgb/README.md), [9 original photos](task2-rgb/images/) |
| 3 | [main.cpp](task3-strip/src/main.cpp) | [platformio.ini](task3-strip/platformio.ini) | [AGENTS.md](task3-strip/AGENTS.md) | [Project README](task3-strip/README.md), [report](task3-strip/experiment-report.md), [6 original photos](task3-strip/images/) |

The shared [AGENTS.md](AGENTS.md) records board, pins, libraries and evidence
rules. All three projects use 115200 baud. Task 1 runs automatically. Task 2
accepts `R G B` (0..255) or `off`; for example `0 255 0` requests green.
Task 3 accepts `index R G B` (index 0..7), `all R G B`, or `clear`; for example
`0 255 0 0` selects pixel 0 red. Invalid input is rejected by the archived
parsers. These are usage examples, not a newly acquired serial transcript.
CR, LF and CRLF are accepted; the documented Windows workaround leaves
`send_on_enter` disabled. Detect the current port with `pio device list` rather
than assuming the historical COM8 remains assigned.

The Task 3 driver is `NeoEsp32Rmt0Ws2812xMethod`, RMT channel 0, with
`NeoGrbFeature` and `NeoPixelBusLg` from `NeoPixelBusLg.h`. The hardware method
is consistent with [NeoPixelBus guidance](https://github.com/Makuna/NeoPixelBus/wiki/ESP32-NeoMethods).
It reduces software-timing dependence; it is not proof of immunity to every
interrupt, supply fault or data-level problem. Strip power and data are separate:
5 V power does not establish a guaranteed input-high margin for 3.3 V data.
The photos demonstrate operation in this setup, not a worst-case logic-margin
test. Check the particular strip's input specification for a robust installation.

## Measured currents and calculated estimates

| Experiment / condition | Current record | Evidence and qualification |
|---|---|---|
| Task 1, external LED | **Not measured** | No resistor-drop/current reading archived for this LED. |
| Task 2, green | Approximately **4.5 mA**, calculated from measurement | 1.0 V resistor indication / nominal 220 ohm; [photos](task2-rgb/README.md#green-condition). |
| Task 2, red alone | **Not measured** | No red-only current or resistor-drop evidence. |
| Task 2, blue | **Not measured reliably** | Supplied photo-node assignments conflict; 12.7 mA is conditional arithmetic, not a validated current. A possible 0.4 V resistor reading would imply 1.8 mA only if confirmed. |
| Task 2, near-white | Approximately **3.2 mA**, qualified branch estimate | 0.7 V / nominal 220 ohm; branch unspecified, 0.2 V voltage-balance discrepancy. Not total RGB current. |
| Task 3, strip off / one / eight lit | **Not measured** | Meter photos show 0.8 / 2.7 / 16.0 **mV**, not mA; shunt and nodes unknown. |

No resistor tolerance, exact PWM setting, calibrated meter accuracy, or total
strip current is invented. These qualifications remain part of the result.

## Strip power budget — computed versus measured

For planning only, use the course's conservative **60 mA per pixel at full white**
model. Actual strip variants, idle current, brightness scaling, and supply voltage
can change the result. The luminance-80 column uses a simple proportional model:
`I80 = n × 60 mA × 80/255`; power uses a **nominal 5 V**, not a measured supply.

| Pixels | Full-white current, computed | Full-white power at 5 V, computed | Luminance 80 current, computed | Luminance 80 power at 5 V, computed | Measured current |
|---|---:|---:|---:|---:|---|
| 8 | 0.480 A | 2.40 W | 0.151 A | 0.75 W | **Not measured** |
| 30 | 1.800 A | 9.00 W | 0.565 A | 2.82 W | **Not measured**; no 30-pixel test |
| 60 | 3.600 A | 18.00 W | 1.129 A | 5.65 W | **Not measured**; no 60-pixel test |
| 144 | 8.640 A | 43.20 W | 2.711 A | 13.55 W | **Not measured**; no 144-pixel test |

Only the eight-pixel assembly is photographed. At full white, its modelled
480 mA leaves just 20 mA within a nominal 500 mA USB allowance for everything
else, so it is unsuitable as a whole-system budget. Luminance 80 provides more
headroom but is not a supply qualification. The photographs use an earlier
luminance 96 and must not validate the luminance-80 estimates. Larger strips need
a separately rated 5 V supply, shared ground, suitable wiring and power injection;
include the controller and design margin. Do not assume USB-C current capability
from connector shape alone. [Power guidance](https://learn.adafruit.com/adafruit-neopixel-uberguide/powering-neopixels)
explains supply sizing; the actual source must be checked.

## Task 4 — Share the repo audit

This audit follows the [official Module 5 task](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/05-leds-and-light-output.md#-task-4--share-the-repo-ai-first).
The three independent projects are already consolidated under `Module05/`;
no relocation or replacement of earlier evidence was necessary.

| Deliverable | Status in this repository |
|---|---|
| Three projects with source and configuration | Present; original firmware preserved. |
| Module README, wiring, REPL, driver and power table | Present, with measured/computed distinctions. |
| Module rules and per-project README / AGENTS | Present; actual GPIO13/14/27 mapping, GRB, eight pixels, 115200 baud and cap 80 checked against source. |
| Photographic evidence | Task 2: nine originals; Task 3: six originals; Task 1 dedicated fade evidence pending. |
| Measured current records | Partial; see current table. Missing readings stay Not measured. |
| Build products and local secrets | Module ignore rules added; tracked-file and link checks performed. No new firmware upload. |
| Source/version reproducibility | Board and dependency names documented. Platform/library versions are unpinned, so fresh builds can resolve differently; no fresh compile claimed in this audit. |
| Portfolio commit and push | Included in this Task 4 update; use Git history for the exact commit. |
| Repo URL in Study log | Recorded below. |
| Discord post | **Pending external evidence**; no post or peer confirmation supplied or created. |
