# Task 7 — ESP8266 Built-in Wire Software-Slave Candidate

**Compiled / Not uploaded / Not hardware verified.** [Sketch](task07_esp8266_slave.ino) and test_protocol.h must stay in this same-name folder. Target **LOLIN(WEMOS) D1 R2 & mini / esp8266:esp8266:d1_mini / Core 3.1.2**. No third-party slave library is used. Core source support is verified, not reliability at 100 kHz.

Actual compile PASS: global RAM 29040/80192 bytes, instruction RAM 61671/65536 bytes (94%), flash code 240112/1048576 bytes. [Real compile log](../../evidence/task07-two-devices/compile-esp8266-20261102.txt). Only black ESP32 COM3 was present, so no blue-board port was guessed or flashed.

The Core's Wire.begin(D2,D1,uint8_t(0x42)) uses D2/GPIO4 SDA and D1/GPIO5 SCL. Callbacks are attached after begin; the receive callback uses size_t. Wi-Fi is disabled. Callback code consumes at most 64 bytes, accepts only A5/sequence two-byte commands, and produces the report's bounded 12-byte T7 packet with actual uptime/counter. No Serial or delay inside callbacks. This is a programmed MCU test device, not a sensor. 0x42 is a configured candidate, **not an observed address**.

Only if this backup is chosen: power off and join blue D2→ESP32/LCD SDA21, blue D1→ESP32/LCD SCL22, blue GND→ESP32 GND. Use separate USB power and do not join 3V3/5V outputs. Check compatible 3.3 V pull-ups; no resistor value is assumed. Do not also enable local Wire1 at 0x42. Use ESP32 master command `b` after slave upload/wiring; successful ACK alone is insufficient, validate its packet too.

[Report / references / protocol](../../two-devices.md) · [Preferred ESP32-local alternative](../task07_two_devices/README.md). Actual interoperation and stability remain Pending.
