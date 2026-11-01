# Task 5 — Repeating I2C Scanner

**Latest: five real complete scans found LCD address 0x27; removal comparison Pending.** After student rewiring on 2026-11-02, each sweep returned 1 ACK / 111 NACK / 0 errors. [New raw log](../../evidence/ds1302-lcd-extension/scanner-serial-20261102.txt) · [Latest report](../../evidence/README.md#latest-scan-after-rewiring--2026-11-02). Earlier blocked attempts below are retained as history. See the [real Task 5 report and logs](../../evidence/README.md#scanner-report--task-5-partial-baseline). The student now identifies the RTC as **DS1302**, not DS3231; it is not an I2C device and is excluded. This run only targets the LCD1602 PCF8574 backpack.

## Open and Build

Open [task05_i2c_scanner.ino](task05_i2c_scanner.ino) in Arduino IDE 2.3.10. Select **ESP32 Dev Module**, installed Espressif Core **3.3.12**; the observed CP210x port was **COM3**. Console: **115200 baud, 8N1**.

The current diagnostic source compiled successfully (294644 flash bytes / 23508 global RAM bytes) and was uploaded on 2026-11-02 with **Hash of data verified**. It replaced on-board firmware on that ESP32, without changing any stored Task 9 files. The original source also compiled/uploaded (294564 / 23508 bytes). The new source adds only separate idle SDA/SCL level reporting; it does not bypass the idle guard. This revision was compiled for ESP32; no fresh ESP8266 build is claimed.

## Pins and Electrical Constraints

| Target | SDA | SCL | Bus / console |
|---|---|---|---|
| Classic ESP32 Dev Module, current run | GPIO21 | GPIO22 | Requested 100 kHz / 115200 baud |
| Existing ESP8266 branch, not used for this run | D2 / GPIO4 | D1 / GPIO5 | Requested 100 kHz / 115200 baud |

LCD intended wiring: VCC→ESP32 3V3, GND→GND, SDA→GPIO21, SCL→GPIO22. The student confirms VCC on 3V3 and USB power; GND, signal landings, pull-ups and absence of other attached paths remain physically unverified. No voltage was measured. Keep ESP32 signals at compatible 3.3 V levels; never attach 5 V pull-ups. An LCD without visible text can still have a communicating backpack. Do not wire DS1302 DAT/CLK/RST as I2C.

Before correcting wiring, disconnect USB/all power. Trace only these LCD wires, isolate DS1302 signals if attached, remove the obsolete GPIO22–23 analyzer jumper if present, and inspect any static-test or external-board connections on GPIO21/22. A jumper alone is not a proven fault. Do not dismantle unrelated circuits. [Chinese preparation guide](../../wiring-zh.md) contains earlier conditional DS3231 preparation; the current report supersedes that hardware assumption.

## Operation

On ESP32 the sketch calls `Wire.begin(21,22)`, requests 100000 Hz and sets a 50 ms operation timeout. It samples idle SDA/SCL once per scan attempt; if either is LOW it prints both states and skips address probes. This is a digital check, not a voltage measurement or pull-up validation. Otherwise it scans 0x08–0x77 (112 ordinary 7-bit addresses), prints ACK addresses, NACK/error counts and `SCAN_END`. No ACK prints **No I2C devices found**; errors must still be checked. It repeats about three seconds after each attempt; sending `s` requests an attempt. A scan error/skip is not evidence of an empty bus. An ACK is not independent chip identification.

## Earlier Blocked Result and Remaining Removal Procedure

The original capture contains five BUS_NOT_IDLE attempt records. The diagnostic capture contains four records with **SDA_GPIO21=LOW SCL_GPIO22=LOW** each time. No addresses were probed, no full sweep completed and ACK/NACK counts are unavailable. The LCD's actual address remains unknown. Both files, a live IDE screenshot and a verification JSON are linked in the report.

After power-off wiring/pull-up correction, capture at least three full LCD-connected scans. Then power off and remove the LCD, capture three more full scans and compare address sets; check that the remaining bus still has valid idle pull-ups. Physical removal awaits the student. The old [Task 9 NACK-only baseline](../../evidence/task09-esp8266-i2c.md) remains separate evidence from another board/sketch. No RTC/LCD time-display sketch is part of this run.
