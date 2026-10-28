# Task 2 CAN — A: Local Self-Test

**Prepared / Awaiting hardware validation.** [Sketch](task02_can_self_test.ino) · [CAN report](../../bus-can.md) · [Wiring details](../../can-wiring.md) · [Blank results](../../evidence/task02-can-test-record.md).

## Requirements and Mechanism

Classic ESP32 Dev Module, USB cable, Arduino IDE 2.3.10 and installed Espressif Core 3.3.12 (ESP-IDF 5.5.5). Use built-in `esp_twai.h` / `esp_twai_onchip.h`; no MCP2515 library or third-party CAN library.

No external transceiver, CAN cable, termination or jumper is required. **Leave GPIO5 completely disconnected.** The driver maps TWAI TX and RX to the **same GPIO5**, providing local output-to-input feedback through the chip GPIO matrix/pad. The pin still physically drives logic levels; this is not an electrically isolated controller loopback mode. It must not be driven by another device. The [reviewed driver source](https://github.com/espressif/esp-idf/blob/v5.5.5/components/esp_driver_twai/esp_twai_onchip.c) enables the RX input separately before routing TX.

`enable_self_test=true` disables external ACK checking; `enable_loopback=true` enables own-frame reception. On classic ESP32 these flags alone do not create the missing feedback path. The same-pin configuration supplies it. Never use this configuration as a real CAN bus test.

## Run and Interpret

1. Disconnect other hardware from GPIO5; confirm the classic ESP32 board/pinout.
2. Open the same-name sketch folder. Select **ESP32 Dev Module**, Core **3.3.12** and the actual USB port; compile and upload.
3. Open Serial Monitor at **115200 baud, 8N1**. No text input is required.
4. The configured bit rate is **500 kbit/s**. Approximately once per second, the program sends standard ID **0x123**, DLC=8. The first four bytes encode a sequence little-endian; the last four are test constants CA FE 55 AA. These are chosen synthetic test payloads, not sensor measurements.
5. TX/RX lines print actual ID, DLC and bytes when run. PASS requires successful TX completion plus matching ID, flags, DLC and all payload bytes. A timeout/mismatch prints FAIL; a TX completion timeout halts to avoid reusing a buffer still owned by the driver.
6. Save authentic serial output and describe failures in the blank record. No successful runtime result is supplied yet.

Callbacks run in ISR context and copy records into queues; printing and comparison run in `loop()`. TX storage persists until completion. Queues are finite; this is a slow teaching example, not a stress-test tool.

**What a future PASS proves:** local controller/driver frame transmission and matched local reception. **Still unverified:** external transceiver, CANH/CANL, ACK from another node, termination, wiring, cable length, real bit timing and EMC robustness.

## Software Validation

**Compile PASS — 2026-10-28.** Installed Arduino CLI, `esp32:esp32:esp32`, Core 3.3.12 / IDF 5.5.5. Result: 286248 bytes flash, 22180 bytes global RAM.

Command from repository root: `arduino-cli compile --fqbn esp32:esp32:esp32 --jobs 4 Module02/code/task02_can_self_test` (build output redirected to a temporary directory). No upload was performed. Hardware execution remains **Pending**.
