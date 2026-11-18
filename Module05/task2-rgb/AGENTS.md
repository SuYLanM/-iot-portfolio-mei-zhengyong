# AGENTS.md — Module 5, Task 2 (RGB LED)

Project rules for any AI agent working in this folder.

## Board

- MH-ET LIVE ESP32 MiniKit, PlatformIO board id `mhetesp32minikit`
- Previously reported toolchain: `espressif32` 8.0.0, Arduino core 3.3.12.
  The configuration uses the Arduino framework but does not pin package versions;
  verify the resolved toolchain when rebuilding rather than assuming these reports.
- PlatformIO CLI `pio` is on PATH. Work from this folder (the one holding
  `platformio.ini`); keep the project on an ASCII path.

## Wiring

| Channel | GPIO | Series resistor | LED pin |
|---|---|---|---|
| Red | 13 | 220 ohm | red leg |
| Green | 14 | 220 ohm | green leg |
| Blue | 27 | 220 ohm | blue leg |
| Common | — | — | GND (common cathode) |

- The course module specifies GPIO 16/17/18. This board does not break out
  GPIO 16, so the mapping above is used instead. Do not silently switch back to
  16/17/18.
- Each channel has its own 220 ohm resistor; never share one resistor between
  channels.
- A common-anode part wires its common pin to 3V3 and needs
  `COMMON_ANODE = true`, which inverts the PWM duty.

## Firmware rules

- `src/main.cpp` must start with `#include <Arduino.h>`; PlatformIO compiles
  `.cpp` as C++ and does not add it automatically.
- Drive the channels with `analogWrite()` (LEDC on core 3.x).
- Serial protocol: `R G B` as three integers 0..255, plus `off`. Echo the
  applied values, and reject malformed or out-of-range input with an error line.
  Keep the `rgb>` prompt.
- Accept CR, LF and CRLF as line endings, and ignore other control bytes: a
  Windows terminal sends a bare CR on Enter, and a stray NUL arrives when the
  serial port opens.
- Baud rate is 115200.
- Do not enable `monitor_filters = send_on_enter` despite the course prompt: it
  buffers input until it sees CRLF, so on Windows a bare-CR Enter transmits
  nothing and the monitor appears dead.

## Commands

```sh
pio run -t compiledb   # refresh compile_commands.json after dependency changes
pio run                # build
pio run -t upload      # flash
pio device monitor -b 115200
```

`compile_commands.json` and `.pio/` are ignored by Git; never commit them.
