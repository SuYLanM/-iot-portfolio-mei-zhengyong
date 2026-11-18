# AGENTS.md — Module 5, Task 3 (8-pixel WS2812B strip)

Project rules for any AI agent working in this folder.

## Board

- MH-ET LIVE ESP32 MiniKit, PlatformIO board id `mhetesp32minikit`
- Previously reported toolchain: `espressif32` 8.0.0, Arduino core 3.3.12.
  The configuration uses the Arduino framework but does not pin package versions;
  verify the resolved toolchain when rebuilding rather than assuming these reports.
- PlatformIO CLI `pio` is on PATH. Work from this folder (the one holding
  `platformio.ini`); keep the project on an ASCII path.

## Wiring

| Strip pad | Goes to |
|---|---|
| `VDC` (5 V supply) | board `5V` |
| `GND` | board `GND` |
| `DIN` (data in) | **GPIO 13** |
| `DOUT` (data out) | not connected |

- The course module specifies GPIO 16 for the data line. This board does not
  break out GPIO 16, so **GPIO 13** is used. Do not silently switch back to 16.
- Data is unidirectional: connect to the `DIN` end of the strip, otherwise no
  pixel lights.
- Ground must be shared between the board and the strip.
- A 330 ohm series resistor in the data line is optional insurance; it is not
  fitted here and is not required for a short strip.
- Never connect or disconnect the strip while it is powered.

## Library and driver

- `lib_deps = makuna/NeoPixelBus`. **Never FastLED** and never bit-banged
  timing: on the ESP32, Wi-Fi/BLE interrupts corrupt software-timed 800 kHz NRZ
  frames.
- Use `NeoPixelBusLg` (the class that has `SetLuminance()`), not plain
  `NeoPixelBus`. It lives in its own header:
  `#include <NeoPixelBusLg.h>` — including only `NeoPixelBus.h` gives
  "'NeoPixelBusLg' does not name a type".
- Driver: `NeoEsp32Rmt0Ws2812xMethod` (RMT channel 0, hardware waveform). The
  existing record reports a legacy-RMT deprecation warning. Do not treat
  this as a Wi-Fi/BLE stress-test result or compatibility with every core version.
- Colour order is `NeoGrbFeature` (the strip is GRB, not RGB).

## Power

- `strip.SetLuminance(80)` or lower. 8 x 60 mA = 480 mA at full white, which is
  close to a 500 mA budget before adding the board. This is a planning
  assumption, not a measurement or a guarantee of available port power.
  Preserve the earlier luminance-96 photos without changing the source cap of 80.
- Never power the strip from a GPIO or from the 3V3 pin.

## Firmware rules

- `src/main.cpp` must start with `#include <Arduino.h>`.
- Serial protocol: `index R G B` with `index` 0..7 and values 0..255, plus
  `all R G B` and `clear`. Echo every applied change and reject malformed, out-of-range or
  out-of-bounds input with an error line.
- Accept CR, LF and CRLF as line endings, and drop other control bytes (a stray
  NUL arrives when the port opens).
- Baud rate is 115200.
- Do not enable `monitor_filters = send_on_enter`: it buffers input until it sees
  CRLF, so a bare-CR Enter from a Windows terminal transmits nothing.

## Commands

```sh
pio run -t compiledb   # refresh compile_commands.json after dependency changes
pio run                # build
pio run -t upload      # flash
pio device monitor -b 115200
```
