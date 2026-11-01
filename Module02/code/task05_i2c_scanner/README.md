# Task 5 — Repeating I2C Scanner

**Prepared / Awaiting real sensor ACK and unplug comparison.** The existing [Task 9 no-target baseline](../../evidence/task09-esp8266-i2c.md) is reusable evidence, not a new run of this revised sketch. [Task 5 report and blank hardware record](../../evidence/README.md#scanner-report--task-5-partial-baseline) · [Chinese guide](../../wiring-zh.md).

## Supported Boards and Wiring

| Board selection | Core checked | SDA | SCL |
|---|---|---|---|
| ESP32 Dev Module, classic target | Espressif 3.3.12 | GPIO21 | GPIO22 |
| LOLIN(WEMOS) D1 R2 & mini | ESP8266 Community 3.1.2 | D2 / GPIO4 | D1 / GPIO5 |

Select the actual board and port; blue WeMos previously used COM4. **Do not overwrite the black ESP32 analyzer on COM3.** This sketch can use the blue ESP8266 as the controller. If separately probing its bus, SDA goes to black CH0/GPIO21 and SCL to black CH1/GPIO19; retain analyzer-local GPIO22→GPIO23 and common ground.

Required: a positively identified I2C module for ACK, datasheet-approved supply, common ground, short SDA/SCL wires and reviewed pull-ups to compatible 3.3 V. A module VIN label does not prove the pull-up rail. Do not join two regulator outputs or attach 5 V pull-ups. Confirm breakout schematic and address pin state before wiring. Scan probes may affect unusual devices; inspect the actual device specification first.

## Program and Output

Open [task05_i2c_scanner.ino](task05_i2c_scanner.ino) in Arduino IDE 2.3.10. Console is 115200 baud. The same sketch uses ESP8266 `Wire.begin(SDA,SCL)` plus `setClock(100000)` or ESP32 `Wire.begin(SDA,SCL,100000)` with platform-appropriate timeout APIs. It requests 100 kHz, not a verified physical clock. ESP8266 begin is void; that branch returning ready does not prove target presence.

A scan starts after setup, repeats roughly three seconds after each scan, and can be requested with `s`. Reserved addresses are excluded: 0x08–0x77 are probes, not found devices. Output includes actual ACK addresses, NACK/error totals and a low-idle-line warning. ACK does not identify a model or count colliding devices. Blocking scan duration can increase with errors; an idle-level check cannot prove electrical compliance.

## Required Real Procedure

1. Power off, identify controller/module, verify supply, common ground and pull-ups; wire according to the selected row. Upload only to the intended controller.
2. Save the actual module-connected scan, map ACK addresses to the known module's datasheet, and record core/port/pins/clock request.
3. Power off, remove one identified sensor, power on and scan again. Record the actual vanished address; reconnect only with power off and verify restoration.
4. Save logs, actual board/module pictures and a reasoned conclusion. With no sensor, report the NACK baseline only. Do not populate Pending addresses from examples.

## Evidence Reuse and Validation

The preserved Task 9 full serial scan returned 0 ACK / 112 NACK / 0 other errors; its short raw capture shows twelve NACK requests, not twelve devices. Sensor identification and unplug/reconnect observations remain Pending. The revised Task 5 code has not been uploaded or hardware-tested. Compilation checks for both installed cores are recorded in [preparation validation](../../validation.md#follow-up-preparation-checks--2026-11-02).
