# Task 7 — ESP32 Master and Optional Wire1 Test Target

The existing sketch has been upgraded from a disabled adapter scaffold. [English report / actual logs](../../two-devices.md). **Compile and single-LCD hardware baseline verified; physical dual-device test Pending.**

Open [task07_two_devices.ino](task07_two_devices.ino) with its local_slave.h and test_protocol.h in the same folder. Select **ESP32 Dev Module / Core 3.3.12 / COM3 CP210x**. Installed dependency: **hd44780 1.3.2**. Serial **115200**, master SDA21/SCL22, requested 100 kHz, 50 ms Wire timeout. Compile PASS: flash 301376 / global RAM 23596 bytes. COM3 upload Hash verified.

## Commands and Actual Run

- `g`: three baseline scan/LCD rounds, no local slave and no test-protocol writes. This command was actually run. Every sweep found 0x27 only, 1 ACK / 111 NACK / 0 errors; all three LCD DDRAM matches passed. 0x42 never ACKed.
- `a`: **only after the two physical connections below**, initialize Wire1 as address-0x42 slave and run three master rounds. Not run yet.
- `b`: only after separately flashing/wiring the [ESP8266 candidate](../task07_esp8266_slave/README.md), run the armed test protocol without enabling local Wire1. Not run yet.

No automatic retry after a three-round session. No push-pull bus recovery. Only known LCD protocol is used at 0x27; a baseline ACK at unexpected 0x42 does not arm test writes. Dual success requires both address ACKs, a real LCD DDRAM match, and a valid 12-byte test response echoing the current sequence with valid signature/version/XOR. B has actual MCU uptime/counter data, **not a physical sensor reading**. Stale/short/bad replies are rejected, at most three read attempts.

## Preferred Local-Target Wiring — Physical Step Pending

Power off. Add **GPIO32→the shared GPIO21/LCD SDA net** and **GPIO33→the shared GPIO22/LCD SCL net**. Keep LCD attached by branching the nets on the breadboard if necessary. One ESP32 already shares ground; no new power connection or blue board is needed. GPIO32/33 do not overlap DS1302 DAT25/CLK26/CE27. Do not connect SDA to SCL. Never attach 5 V pull-ups. Actual pull-up values/rise time have not been measured.

Wire and Wire1 are separate physical I2C roles, not internally auto-connected. Local Wire1 has a preloaded response for classic ESP32 and protected callback packet snapshots; hardware coexistence is not yet verified. Do not enable local 0x42 and attach a blue-board 0x42 at the same time. The full professor task still needs two genuine sensor readings; LCD plus MCU is an explicitly adapted demonstrator.

[Current raw serial](../../evidence/task07-two-devices/master-baseline-serial-20261102.txt) · [Compile](../../evidence/task07-two-devices/compile-esp32-20261102.txt) · [Upload](../../evidence/task07-two-devices/upload-esp32-20261102.txt) · [Protocol layout / candidate evaluation](../../two-devices.md).
