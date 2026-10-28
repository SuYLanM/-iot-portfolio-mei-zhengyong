# Task 8 — TCA9548A Selection Template

## Parts / Pins / Wiring

A **confirmed TCA9548A** breakout, ESP32/USB, actual downstream devices, appropriate pull-ups and verified supply/logic. Upstream SDA21/SCL22/GND; downstream SDn/SCn each to its intended branch. Address straps/reset must follow the actual breakout datasheet. First evaluation keeps all bus pull-ups compatible with 3.3 V logic; do not infer ESP32 5 V tolerance from the switch's translation feature.

## Configuration / Output

MUX_CONFIGURATION_VERIFIED=false and MUX_ADDRESS=-1 prevent bus activity. After confirming the chip/strap-derived address within the documented 0x70–0x77 options and channel 0–7, enable the guard. The sketch writes one channel bit, reports Wire status, then writes zero to deselect all. It does not claim a successful downstream sensor read. Never enable two conflicting same-address branches simultaneously. A stuck LOW branch may require hardware RESET rather than a command that cannot traverse the blocked bus.

Record actual straps, mux and sensor addresses, before/after scans and sensor values in [the conflict note](../../address-conflict.md). This is a software framework and theory only; no mux or physical conflict experiment has been tested.

## Run and Status

Prepared and compile-checked for **ESP32 Dev Module / Espressif Core 3.3.12**, Arduino IDE 2.3.10. Open the same-name `.ino` in this folder, select the actual board/port and verify. After inspecting actual hardware, power off to wire, reconnect and upload yourself; no upload was performed during preparation. Open Serial Monitor at **115200 baud**. Compilation is not hardware testing. [Chinese wiring guide](../../wiring-zh.md) · [Evidence register](../../evidence/README.md).
