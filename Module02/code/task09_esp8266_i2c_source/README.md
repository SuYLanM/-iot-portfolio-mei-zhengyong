# Task 9 — WeMos D1 mini ESP8266 I2C Capture Source

**Prepared / Hardware test Pending.** [Sketch](task09_esp8266_i2c_source.ino) · [Task 9 report](../../scope-decision.md). This separate sketch replaces PWM only on the blue ESP8266 after wiring confirmation. Never flash the black ESP32 analyzer. Existing ESP32 source and Tasks 1–8 are unchanged.

## Board and software

Arduino IDE 2.3.10, ESP8266 Community 3.1.2, **LOLIN(WEMOS) D1 R2 & mini**, `esp8266:esp8266:d1_mini`; Serial 115200. Recheck the blue COM port: COM4 was used for PWM; COM3 is the protected analyzer. Use `Wire.begin(D2,D1)` and `Wire.setClock(100000)`. The ESP8266 three-argument begin overload takes an address, not a clock frequency. [Official ESP8266 Wire documentation](https://arduino-esp8266.readthedocs.io/en/3.1.2/libraries.html#i2c-wire-library); installed 3.1.2 Wire.h and software-I2C implementation were inspected.

## Wiring — power off both boards before changes

| Blue source B | Black analyzer A | Meaning |
|---|---|---|
| D2 / GPIO4 SDA | GPIO21 / CH0 | Data probe |
| D1 / GPIO5 SCL | GPIO19 / CH1 | Clock probe; move old PWM wire off analyzer GPIO21 |
| GND | GND | Dedicated common reference |
| No source connection | A GPIO22→A GPIO23 | Retain analyzer-local clock jumper |

Do not join board 3V3/5V outputs. Do not connect SCL to analyzer GPIO22, which is an output. Use only 3.3 V logic. Confirm the sensor model and its pull-ups before connecting it. A starting proposal is **one 4.7 kΩ pull-up from SDA to blue 3V3 and one 4.7 kΩ from SCL to blue 3V3**, subject to actual capacitance/rise-time checks. Do not blindly parallel module pull-ups. Installed ESP8266 software Wire enables weak internal pull-ups; these are not a validated substitute for suitable external pull-ups at 100 kHz.

No sensor is required for a NACK-only test with verified pull-ups. A real compatible responding device is required to demonstrate ACK or sensor readings.

## Capture procedure

1. Confirm wiring, available pull-ups/module and the blue COM port before upload. The sketch starts **paused**; no PWM generation or Wi-Fi application is used.
2. On the source console at 115200: `s` requests one scan; `c` enables repeated scans with a 100 ms idle gap after each scan; `p` pauses. Only non-reserved 7-bit addresses 0x08–0x77 are probed. The program reports actual ACK addresses and separate NACK/error totals. No address is pre-filled as a discovered device.
3. PulseView: real ESP32 Logic Analyzer v0.31 / ols / COM3 / 921600; enable CH0+CH1, request 1 MHz and 10 k samples, RLE off, external clock off. A CH1 LOW-level trigger can avoid idle-only windows; use continuous scans or request a scan after arming. A no-trigger capture can fall in the idle gap and require retry. One capture need not contain the full scan.
4. Add the I2C decoder, SDA=CH0 and SCL=CH1. Save a transaction showing START, seven address bits, write bit, ninth-clock ACK (SDA LOW) or NACK (SDA HIGH), and STOP. These empty writes contain no sensor data. Check actual clock timing rather than treating the requested rate as a measurement.
5. Save original `.sr`, real decoder/settings screenshots, source serial text and a wiring photograph. NACK-only capture is not proof of sensor communication. No acquisition or decoder result is claimed before real evidence exists.

## 中文接线提示

先断开两块板 USB。把原来的蓝色 D1→黑色 GPIO21 PWM 线移到黑色 GPIO19（CH1/SCL）；新增蓝色 D2→黑色 GPIO21（CH0/SDA）。保留蓝色 GND→黑色 GND，以及黑色 GPIO22→GPIO23。两根信号线分别通过合适上拉电阻接蓝色 3V3；不要连接两块板的电源输出。传感器型号、上拉条件及蓝色 COM 口确认后才烧录和采集。

## Software validation

**Compile PASS — 2026-10-28:** installed ESP8266 Core 3.1.2, `esp8266:esp8266:d1_mini`; global RAM 29108 bytes, instruction RAM including reserved cache 60475 bytes, flash code 239172 bytes. Build artifacts remain outside the repository. Upload / serial scan / PulseView I2C decode: **Pending**.
