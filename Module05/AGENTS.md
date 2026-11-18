# Module 05 development and evidence rules

- Each task is an independent PlatformIO Arduino project: `task1-led`,
  `task2-rgb`, `task3-strip`. Board ID is `mhetesp32minikit`
  (MH-ET LIVE ESP32 MiniKit). All serial communication is 115200 baud.
- Preserve existing source and photographic evidence. Change only Module05
  for this module's work. Do not invent measurements or hardware verification.
- Task 1: one external LED, GPIO13, nominal 220 ohm series resistor to anode,
  cathode to GND; PWM uses `analogWrite()`.
- Task 2: one common-cathode RGB LED; R=GPIO13, G=GPIO14, B=GPIO27;
  separate nominal 220 ohm resistors; common to GND. `COMMON_ANODE=false`.
  Commands are `R G B` (each 0..255) and `off`.
- Task 3: exactly 8 WS2812B pixels; board 5V to VDC, GND shared,
  DIN=GPIO13, DOUT unused. Do not power the strip from GPIO or 3V3.
  Use `makuna/NeoPixelBus`, `NeoPixelBusLg.h`, `NeoGrbFeature`,
  `NeoEsp32Rmt0Ws2812xMethod` (RMT channel 0), `NeoGammaNullMethod`.
  Keep source `LUMINANCE=80` or lower; do not change it to the photographed 96.
  The 60 mA/pixel budgeting assumption is not a measured current or an
  unconditional USB-power guarantee. Do not switch to FastLED or bit-banging.
  Commands: `index R G B` (index 0..7), `all R G B`, `clear`.
- GPIO13 is reused across tasks. Do not treat the three circuits as concurrent
  loads. Do not upload or change wiring during a documentation-only task.
- GPIO mappings intentionally differ from course examples. Inspect source
  constants rather than silently substituting GPIO16/17/18.
- All `src/main.cpp` files begin with `#include <Arduino.h>`.
  Keep CR/LF/CRLF input handling and control-byte rejection. Do not enable
  `monitor_filters = send_on_enter`; its Windows issue is documented.
- Run builds from the selected project's directory: `pio run`;
  `pio run -t compiledb` after dependency changes. Upload only when requested.
  Version reports are historical: configurations currently use unpinned
  `espressif32` and NeoPixelBus, so fresh installs may resolve differently.
- Never commit `.pio/`, `compile_commands.json`, binaries or secrets. Scope
  staging to Module05 and inspect the staged diff before a normal push.
- Preserve original photo bytes. Label currents derived from voltage and nominal
  resistance. Keep blue ambiguity, the unknown near-white branch, and strip mV
  units explicit. Discord sharing stays Pending until actual evidence exists;
  never post on the user's behalf implicitly.
