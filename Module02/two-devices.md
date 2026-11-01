# Task 7 — Two Devices on One I2C Bus

**Software prepared and compiled; real single-LCD baseline verified; real dual-address/data test Awaiting physical jumpers.** A separately labelled software model passed eight tests. Neither model results nor a configured 0x42 constant are hardware evidence. The professor's complete two-sensor requirement is **not fulfilled**.

## Objective

The [official Task 7](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/02-i2c-and-sensor-addressing.md) asks for two differently addressed I2C sensors sharing SDA/SCL, both addresses in a scan, both readings in one sketch, and an explanation of coexistence. With existing inventory, prepare a practical LCD plus controlled MCU test-target adaptation while preserving that distinction.

The [existing Task 7 master sketch](code/task07_two_devices/README.md) was upgraded in place; no duplicate main report was created. Original Task 5/6/9 code, evidence and reports were not changed.

## Hardware Inventory

| Item | Actual role / status |
|---|---|
| Classic ESP32 Dev Module | COM3 CP210x; Arduino IDE 2.3.10; installed Core 3.3.12; master GPIO21 SDA / GPIO22 SCL |
| LCD1602 + PCF8574 | **Real 7-bit address 0x27**, established in Task 5 and re-observed in this Task 7 baseline; LCD DDRAM communication verified |
| DS1302 RTC | Three-wire DAT25/CLK26/CE27, **not an I2C target**; not accessed by the Task 7 sketch |
| WeMos D1 mini ESP8266 | Core 3.1.2 installed; built-in Wire software-slave candidate compiled, not uploaded/tested. Only COM3 was present |
| Breadboard / jumpers | Required to branch the bus for the proposed hardware test; no remote physical wiring performed |
| Additional I2C sensors | Not available in the provided inventory |

No new VCC/SDA/SCL voltage or pull-up-resistance measurement was performed. The student previously reported LCD VCC on 3V3. Do not connect 5 V pull-ups or join USB boards' power-rail outputs.

## Two Devices on One I2C Bus

Targets share two open-drain signal nets; the controller selects a 7-bit address before data transfer. Only the selected compatible target should respond. Separate target addresses do not require extra **master** SDA/SCL pins. Different addresses alone are insufficient: compatible supplies/pull-ups, wiring, bus capacitance/rise time, speed and protocol are also necessary. A stuck-low participant can block both targets.

Here A is the LCD at observed 0x27. B is a **proposed MCU test endpoint at configured 0x42**, not a temperature/other physical sensor. No 0x42 hardware response has yet been measured. A successful future LCD+MCU run would demonstrate bus sharing, not satisfy the literal two-sensor reading requirement by itself.

## Candidate Feasibility — Source and Compile Verification

| Scheme | Verified support | Constraints / decision |
|---|---|---|
| **A: same classic ESP32, Wire master + Wire1 slave** | Installed SoC header declares two I2C controllers and slave support; Core 3.3.12 exposes address/pin slave begin, callbacks and slaveWrite. Master plus slave code compiled together | **Preferred**, normally two added signal connections, no second USB board. Wire1 uses GPIO32/33 and needs physical ties to shared SDA/SCL; controllers do not magically connect internally. Initializing/communicating through Wire1 is not yet hardware tested |
| **B: ESP8266 built-in Wire software slave** | Installed Core 3.1.2 Wire has begin(sda,scl,address), receive/request callbacks and a software slave engine; full standalone sketch compiled | Backup; D2/GPIO4 SDA, D1/GPIO5 SCL, common GND; requires blue board USB and wiring. No third-party slave library installed. Software timing/reliability at 100 kHz remains unverified |

The ESP32 slave path uses its Core HAL rather than installing an unrelated MCP/third-party driver. Classic ESP32 requires preloaded slave TX data; the prepared code calls slaveWrite. A stale preloaded reply is rejected by sequence checking with at most three read attempts. The ESP8266 sketch uses the size_t receive callback overload and registers callbacks **after** begin, so interrupt attachment uses configured pins; it disables Wi-Fi and performs no Serial output/delay inside callbacks. Source support and successful compilation do not guarantee physical stability.

[Installed Core-source hashes and evidence verification](evidence/task07-two-devices/verification-20261102.json) record the actual files inspected.

## Wiring and Addressing

### Preferred A — Two Added Signal Connections, Not Yet Confirmed

Power off before adding these connections. Keep LCD attached to the shared nets; branch through the breadboard if a header is occupied. GPIO32/33 are the **slave role's** pads, not new master signals. They do not overlap LCD21/22 or RTC25/26/27.

| Connect | To the same electrical node as |
|---|---|
| ESP32 GPIO32, Wire1 slave SDA | ESP32 GPIO21 and LCD SDA |
| ESP32 GPIO33, Wire1 slave SCL | ESP32 GPIO22 and LCD SCL |
| LCD existing VCC / GND | Preserve ESP32 3V3 / GND |
| DS1302 wiring | Preserve; no changes needed |

One board already has a common ground. No extra power wire or blue board is needed. Keep the existing 3.3 V-compatible pull-up arrangement, check effective parallel pull-ups and do not add 5 V. No resistor value or measured rise time is assumed.

### Backup B — Only If Separately Chosen

| Blue ESP8266 | Existing ESP32/LCD shared bus |
|---|---|
| D2 / GPIO4 | SDA / ESP32 GPIO21 / LCD SDA |
| D1 / GPIO5 | SCL / ESP32 GPIO22 / LCD SCL |
| GND | ESP32 GND |
| USB supply | Separate USB; **do not join 3V3 or 5V outputs** |

The two schemes are alternatives. Do not enable A and connect B at the same 0x42 address. Both target and master pins need compatible 3.3 V pull-ups. No blue-board upload is claimed.

## Arduino Implementation

[ESP32 task07_two_devices.ino](code/task07_two_devices/task07_two_devices.ino) is the existing sketch path. It uses built-in Wire plus installed hd44780 1.3.2; 115200 console, requested 100 kHz, 50 ms Wire timeout. It scans 0x08–0x77 and logs address ACKs, aggregate NACKs/errors and actual input levels. A non-idle/error bus stops the test instead of forced recovery.

Commands:

- **g:** exactly three baseline rounds; Wire1 stays disabled, controlled-test protocol is not armed. LCD at 0x27 is re-probed and, if present, initialized/written/read back. Unexpected 0x42 ACKs do not automatically authorize test-command writes.
- **a:** after Scheme A physical wiring, start Wire1 slave at configured 0x42, then run three master rounds. Failed slave init prevents that run.
- **b:** after separately flashing/wiring the [ESP8266 slave sketch](code/task07_esp8266_slave/README.md), run three rounds with that protocol armed, without enabling Wire1.

A scan ACK is not data-read success. LCD communication compares two actual 16-character DDRAM readbacks with written text. B communication sends `[0xA5, sequence]` with STOP, waits 20 ms, and requests a 12-byte packet at most three times; it rejects short, stale, bad-signature/version/flags or bad-XOR packets. Both addresses **and** both protocol checks must pass for a round to print both_address_and_comm_pass=1. No such success has been observed yet.

| Packet bytes | Meaning |
|---|---|
| 0–1 | ASCII T7 signature |
| 2 / 3 | Version 1 / echoed 8-bit sequence |
| 4–7 | MCU uptime_ms, little-endian uint32 |
| 8–9 | Accepted command counter, little-endian uint16 |
| 10 / 11 | Flags 0 / XOR of bytes 0–10 |

MCU uptime/counter are diagnostic test data, not sensor readings. XOR detects some corruption but is not authentication or a robust CRC. Both standalone Arduino folders carry the same small test_protocol.h so they can be opened directly without installing a custom protocol library.

## Actual Compilation and Hardware Test

| Program | Actual build | Hardware execution |
|---|---|---|
| ESP32 master + optional local slave | **PASS**, Core 3.3.12; flash 301376 bytes / global RAM 23596 bytes; [compile log](evidence/task07-two-devices/compile-esp32-20261102.txt) | [COM3 upload Hash verified](evidence/task07-two-devices/upload-esp32-20261102.txt); **g baseline run only**, no Wire1 initialization |
| ESP8266 software slave | **PASS**, Core 3.1.2 / d1_mini; global RAM 29040 bytes, instruction RAM 61671/65536 bytes (94%), flash code 240112 bytes; [compile log](evidence/task07-two-devices/compile-esp8266-20261102.txt) | Not uploaded; no second serial port/blue board connected during inventory |

[Original 13-second hardware serial capture](evidence/task07-two-devices/master-baseline-serial-20261102.txt):

| Round | SDA / SCL before scan | Found addresses | Probed / ACK / NACK / errors | LCD DDRAM match | 0x42 response / dual success |
|---|---|---|---|---|---|
| 1 | HIGH / HIGH | {0x27} | 112 / 1 / 111 / 0 | 1 | No ACK / 0 |
| 2 | HIGH / HIGH | {0x27} | 112 / 1 / 111 / 0 | 1 | No ACK / 0 |
| 3 | HIGH / HIGH | {0x27} | 112 / 1 / 111 / 0 | 1 | No ACK / 0 |

The renewed 0x27 ACK and DDRAM reads support that LCD SDA is currently connected; this is a real software observation rather than relying on the earlier restoration assumption. All three records explicitly show local_slave_enabled=0 / protocol_armed=0 for the session and test_comm=0. Physical Wire1 jumpers remain unconfirmed. There is **no real two-address pass**, no fabricated B response and no forced re-scan loop.

## Software Simulation — Separate Evidence

The [Python model/test source](code/task07_simulation/test_address_routing.py) never opens a COM port or accesses I2C. [Actual unittest output](evidence/task07-two-devices/simulation-results-20261102.txt) records **8 tests passed**. Synthetic inputs include addresses 0x27/0x42, sequence 9, uptime fixture 123456 and counter fixture 7. These numbers are **not hardware measurements**.

| Test input / condition | Expected and actual software result |
|---|---|
| Two registered target callbacks, one logical SDA/SCL pair | Model scan lists 0x27 and 0x42; passed |
| Request to 0x27 | Only LCD callback invoked; passed |
| 0x42 command A5/09 | Valid model packet, little-endian fixture reconstructed; passed |
| Stale sequence / truncated packet / bit corruption | Each rejected; three tests passed |
| Remove modeled 0x42 | Modeled NACK, 0x27 still routes; passed |
| Duplicate configured address | Model rejects configuration; passed |

The model validates address dispatch and protocol error handling, not ESP32 firmware execution, physical line levels, timing, rise time, arbitration, clock stretching or actual collision behavior. Its duplicate-address rejection is a configuration rule; real colliding devices need not conveniently disappear or raise a software exception. A simulated address pair does not satisfy a real dual-device scan.

## Results and Evidence Reuse

- Real current address: **LCD PCF8574 0x27**, three new full scans and three DDRAM matches.
- Configured candidate: **MCU test endpoint 0x42**, compiled but **not observed in hardware**.
- Prior real evidence: [Task 5 connected scan and SDA-disconnect comparison](evidence/README.md#sda-disconnection-comparison--2026-11-02), [DS1302/LCD extension](evidence/ds1302-lcd-extension/README.md), [Task 6 prepared BME280 example and genuine blocked diagnostics](register-reading.md). These files were reused by links, not rewritten.
- [New Task 7 raw evidence folder](evidence/task07-two-devices/) contains real CLI and serial logs plus separately named simulation output. [Verification JSON](evidence/task07-two-devices/verification-20261102.json) records hashes and parsed actual results. No sensor photo or synthetic screenshot was generated.

## Limitations and Discussion

No second physical I2C sensor exists in this inventory; DS1302 cannot fill that role. Neither MCU alternative has a completed physical-slave test or a proven reliability result. Scheme A adds physical slave-pad connections but leaves the **master** SDA/SCL pair unchanged; it is an emulated target on the same silicon, not an independent sensor. Scheme B uses software timing and has limited remaining instruction RAM; 100 kHz operation requires actual testing. No unverified third-party slave library was installed.

When physical wiring is available, save a fresh both-address scan and check LCD DDRAM and matching 0x42 packet content in the same round, including bounded-failure logs. Do not claim success merely because a controller initialized or 0x42 ACKed. Shared supply/reference, pull-up rail/effective resistance, capacitance, wiring and slowest participant constrain real operation. Actual voltages and physical clock/rise times remain unmeasured. A powered-off device with signal connections can load/backfeed the bus; power off before any wiring change.

## Conclusion

Completed work: functional ESP32 master code at the existing path, compile-checked local-slave capability and standalone ESP8266 candidate, real COM3 LCD scan/DDRAM baseline, and eight passing explicitly synthetic model tests. **Real dual-address/data verification remains Pending**. Even a future LCD+MCU pass will be a bus-sharing demonstrator; the professor's full requirement to read two actual I2C sensors remains unmet without those sensors. No completion/grade or classroom outcome is invented.

## References

- [Official Module 2, Task 7](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/02-i2c-and-sensor-addressing.md).
- [Espressif Wire 3.3.12 header](https://github.com/espressif/arduino-esp32/blob/3.3.12/libraries/Wire/src/Wire.h), [slave HAL](https://github.com/espressif/arduino-esp32/blob/3.3.12/cores/esp32/esp32-hal-i2c-slave.c), [I2C slave API / classic ESP32 preload note](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/i2c.html).
- [ESP8266 Core 3.1.2 Wire](https://github.com/esp8266/Arduino/blob/3.1.2/libraries/Wire/Wire.cpp), [built-in software I2C engine](https://github.com/esp8266/Arduino/blob/3.1.2/cores/esp8266/core_esp8266_si2c.cpp).
- [hd44780](https://github.com/duinoWitchery/hd44780), [NXP I2C references](references.md#r4-i2c-standard).
