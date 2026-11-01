# Task 5 — Repeating I2C Scanner

**Prepared / Awaiting voltage checks and real DS3231/LCD scans.** The existing [Task 9 no-target baseline](../../evidence/task09-esp8266-i2c.md) is reusable evidence, not a new run of this revised sketch. [Task 5 report and blank hardware record](../../evidence/README.md#scanner-report--task-5-partial-baseline) · [Chinese guide](../../wiring-zh.md).

## Supported Boards and Wiring

| Board selection | Core checked | SDA | SCL |
|---|---|---|---|
| ESP32 Dev Module, classic target | Espressif 3.3.12 | GPIO21 | GPIO22 |
| LOLIN(WEMOS) D1 R2 & mini | ESP8266 Community 3.1.2 | D2 / GPIO4 | D1 / GPIO5 |

The current requested controller is **ESP32 Dev Module, GPIO21 SDA / GPIO22 SCL**. The student confirms the DS3231 module and PCF8574 LCD1602 both support 3.3 V and intends an entirely 3.3 V setup, with no translator or 5 V. Actual idle bus-voltage checks remain Pending. Select the actual board/port. **If reusing the black ESP32, uploading this sketch replaces its on-board LogicAnalyzer firmware, while GitHub source and saved captures remain intact.** This preparation performs no upload. The previous ESP8266 option remains supported without requiring a duplicate sketch. If separately probing its bus, SDA goes to black CH0/GPIO21 and SCL to black CH1/GPIO19; retain analyzer-local GPIO22→GPIO23 and common ground.

Required: a positively identified I2C module for ACK, datasheet-approved supply, common ground, short SDA/SCL wires and reviewed pull-ups to compatible 3.3 V. A module VIN label does not prove the pull-up rail. Do not join two regulator outputs or attach 5 V pull-ups. Confirm breakout schematic and address pin state before wiring. Scan probes may affect unusual devices; inspect the actual device specification first.

## Program and Output

Open [task05_i2c_scanner.ino](task05_i2c_scanner.ino) in Arduino IDE 2.3.10. Console is 115200 baud. The same sketch uses ESP8266 `Wire.begin(SDA,SCL)` plus `setClock(100000)` or ESP32 `Wire.begin(21,22)` followed by `Wire.setClock(100000)` (through BUS_HZ=100000) with platform-appropriate timeout APIs. It requests 100 kHz, not a verified physical clock. ESP8266 begin is void; that branch returning ready does not prove target presence.

A scan starts after setup, repeats roughly three seconds after each scan, and can be requested with `s`. Reserved addresses are excluded: 0x08–0x77 are probes, not found devices. Output includes the scan sequence number, actual ACK addresses, probed/detected-address totals, NACK/error totals and a low-idle-line warning. With no ACK it prints exactly **No I2C devices found**. Error totals distinguish a failed bus from an assumed empty bus. There is no preset LCD or RTC address. ACK does not identify a model or count colliding devices. Blocking scan duration can increase with errors; an idle-level check cannot prove electrical compliance.

## Required Real Procedure

1. Before GPIO attachment, inspect each module at the student-confirmed 3.3 V and measure idle SDA/SCL-to-GND in DC-voltage mode with GPIO disconnected; after combining modules recheck. See the detailed Chinese guide. Do not attach any 5 V pull-up to ESP32. Upload only after electrical checks and deliberate board selection.
2. Run A: DS3231 module only; save every observed address and counts. Power off, add the LCD and run B. Do not assume exactly two addresses because a breakout can contain another I2C chip. Map actual addresses by the hardware and scan differences.
3. Run C: power off all devices, remove one complete module and its bus wiring, then power on and scan. Record actual vanished address(es). Run D restores it after power-off and checks restoration.
4. Save logs, actual board/module pictures and a reasoned conclusion. With no sensor, report the NACK baseline only. Do not populate Pending addresses from examples.

## Evidence Reuse and Validation

The preserved Task 9 full serial scan returned 0 ACK / 112 NACK / 0 other errors; its short raw capture shows twelve NACK requests, not twelve devices. Sensor identification and unplug/reconnect observations remain Pending. The revised Task 5 code has not been uploaded or hardware-tested. Compilation checks for both installed cores are recorded in [preparation validation](../../validation.md#task-5-ds3231lcd-preparation--2026-11-02).

## RTC/LCD Extension Gate

The second independent sketch is intentionally deferred until scan results and electrical safety are confirmed. The report records the planned **RTClib by Adafruit + Adafruit BusIO** and **hd44780 by Bill Perry / hd44780_I2Cexp** libraries, installation steps, explicit observed LCD address, and no automatic clock setting. No RTC timestamp or LCD output is claimed. [Full English report](../../evidence/README.md#scanner-report--task-5-partial-baseline).
