# Module 02 — Preparation Validation

Date: 2026-10-28. Scope: new Module02 documents/code and root Module 02 navigation only. Baseline main/origin/main: `999867240dc5b050de1cd9a14eea9f51d2bd4d2d`. Existing Module02 contained only a Pending landing page and pictures README, with no real experimental files. Root README_IMPORT.txt remains untracked and untouched.

## Requirements and Research

Entire official Module 2 read at blob `25aff30988742c068340f554b1fdf951fac2db24`. [Task checklist](task-checklist.md) distinguishes theory, bench, pod/class activity and optional Task 10. [Source register](references.md) records primary documentation, pinned external commits, mismatched README/source claims and retrieval failures. No external repository screenshot is student evidence.

## Compilation — PASS

Seven sketches compiled on 2026-10-28 with the installed Arduino CLI, Espressif Core **3.3.12**, target **esp32:esp32:esp32**. Each folder contains its same-name .ino and English README. No new library was installed. Build products remained outside this repository. No upload/serial-port interaction was performed.

| Sketch | Compile | Program bytes | Global variable bytes |
|---|---|---|---|
| task01_uart_loopback | PASS | 270300 | 22236 |
| task05_i2c_scanner | PASS | 294220 | 23508 |
| task06_library_template | PASS | 293396 | 23508 |
| task06_register_template | PASS | 293944 | 23508 |
| task07_two_devices | PASS | 293652 | 23508 |
| task08_mux_template | PASS | 293312 | 23500 |
| task09_pwm_reference | PASS | 277663 | 22304 |

These are build statistics, not physical readings. Unknown sensor templates retain disabled guards, invalid address/pointer placeholders and false-returning library hooks. Compilation checks scaffold syntax/API compatibility, not a sensor that has not been selected or an implemented future adapter. After configuration changes, compile again and collect real bench evidence.

## Calculations and Scope Checks

Divider nominal/worst-case values independently recalculated using explicit supply/tolerance assumptions. They are labelled calculations; no voltage/current measurement claimed. Source snapshots confirm analyzer CLK_OUT22/CLK_IN23, CH0=21/CH1=19 and source baud921600. Web scope source uses Core 2 LEDC calls; local Core 3 compatibility is Pending. No external instrument firmware was compiled/flashed, and no measured sample rate is claimed.

## Before Publication

Run full Markdown file/fragment checks and git diff --check, review staged paths, scan candidate text for credential material, verify protected files against the pre-work snapshot, and confirm no compiled artifacts or fake captures are staged. Commit only new Module02 preparation and root navigation. Preserve Module01, Module 0, all original data, Reflections and unrelated files. The existing Module02/pictures/README.md remains unchanged.

## Publication Check Results

- Working-tree local Markdown file/fragment checks: **PASS (321 references)**; staged-tree checks will be repeated before commit.
- Seven sketch folder/name/README pairs: PASS. All seven default builds succeeded; disabled sensor adapters remain unimplemented as labelled.
- Only pre-existing README.md and Module02/README.md were edited; all added files are under Module02. Every pre-existing Module01, Hardware image, Module 0, Reflection, source/archive and unrelated file matches its initial SHA-256 snapshot.
- New file set contains no image/log/CSV pretending to be a measurement, compiled output or external binary. Credential-pattern scan: no matches.
- Exact pins, addresses, sensor-specific scaling, assigned bus and instrument performance remain Pending where not known. Scope research commits and their source discrepancies are retained in the reference register.

## Not Tested / Not Completed

No Module 02 UART success, divider measurement, I2C discovery, sensor read, physical address conflict, PWM/instrument capture, actual class adoption, assigned bus, presentation, neighbouring-pod discussion, personal learning statement or Discord post has been recorded. All such evidence remains Pending; Task 10 remains Optional.

## Follow-up Preparation Checks — 2026-11-02

Scope: Task 1 photo import; Task 3 CAN/class preparation; Task 5–7 scanner/scaffold and report preparation. Prior Task 9 reports, raw data, images and code remain unchanged. User's existing UART working-tree change and README_IMPORT.txt are excluded.

Two original Task 1 JPEGs were safely extracted and inspected; repository copies are byte-identical to the ZIP entries, with hashes in their [evidence index](evidence/task01-voltage-divider/README.md). Photo A visibly shows approximately 3.307 V; Photo B is partly obscured and its approximately 2.7 V value remains a student estimate pending confirmation. No probe nodes or resistor values were invented.

| Existing sketch | ESP32 Dev Module / Core 3.3.12 | WeMos d1_mini / ESP8266 Core 3.1.2 | Hardware status |
|---|---|---|---|
| task05_i2c_scanner | Compile PASS | Compile PASS | Revised sketch not uploaded; original Task 9 NACK evidence reused separately |
| task06_library_template | Compile PASS | Compile PASS | Disabled scaffold; actual library/model/adapter Pending |
| task06_register_template | Compile PASS | Compile PASS | Disabled transport/linear-decoder scaffold; actual sensor/mode/register configuration Pending |
| task07_two_devices | Compile PASS | Compile PASS | Disabled scaffold; two sensor adapters and real reads Pending |

Eight Arduino CLI builds exited 0, using --jobs 4 and temporary build directories outside the repository. No third-party sensor library was installed, and no board was flashed or queried in this preparation. The source library hooks deliberately return false; blank units and invalid addresses prevent fabricated readings. A future device adapter/configuration needs a fresh compile and real hardware validation.

The existing CAN report was checked against all nine suggested Task 3 columns; the exact prepared row and an explicitly labelled reflection draft are in [bus-comparison.md](bus-comparison.md). The actual shared class table remains Pending. Scanner/report navigation reuses the existing evidence register and links to preserved Task 9 baseline files without duplicating or modifying them.

## Task 5 DS3231/LCD Preparation — 2026-11-02

The student confirms a PCF8574 LCD1602 backpack and DS3231 module supporting 3.3 V, with all devices intended to use 3.3 V and no 5 V connection. This is recorded as user-confirmed hardware information; actual idle SDA/SCL voltages, wiring, addresses and scans remain Pending. No upload, GPIO connection, serial query or new acquisition was performed.

The existing `task05_i2c_scanner` was updated in place: ESP32 uses Wire.begin(21,22), then sets 100000 Hz; ordinary addresses 0x08–0x77 are probed, actual response/probe totals are printed, and no-ACK scans print No I2C devices found with separate bus-error context. The original ESP8266 compatibility branch is retained. Both builds below exited 0; temporary output remains outside the repository.

| Target / installed core | Compile | Flash / code bytes | Global RAM bytes |
|---|---|---|---|
| ESP32 Dev Module / 3.3.12 | PASS | 294564 | 23508 |
| ESP8266 d1_mini / 3.1.2 | PASS | 239220 | 29120 |

The English report reuses the existing [Task 5 section](evidence/README.md#scanner-report--task-5-partial-baseline), with Objective, Hardware, Wiring, I2C Addresses, Scanner Results, Device Removal Test, RTC and LCD Communication, Troubleshooting and Evidence. The [Chinese guide](wiring-zh.md) provides per-module idle-voltage checks, the confirmed all-3.3 V connection plan and a conditional mixed-voltage translation alternative.

Library metadata/source reviewed: RTClib by Adafruit 2.1.4 (depends on Adafruit BusIO), hd44780 by Bill Perry 1.3.2, including hd44780_I2Cexp's explicit-address constructor. The second sketch is deferred until measured electrical conditions and actual address mapping are supplied, as requested. These libraries/the combined application were not installed, compiled, uploaded or hardware-tested here; no default RTC time or LCD address is claimed. All Task 9 firmware/source/data/reports, other experiment evidence and unrelated working-tree modifications are preserved.
