# Module 02 — Hardware Evidence Register

**Task 9 static CH0 LOW/HIGH captures are now verified:** [original files, replay screenshots and checks](task09-logic-analyzer/README.md). The later [real ESP8266 PWM capture](task09-esp8266-pwm.md) is also available. Other rows remain at their existing status; this register is not a declaration that all labs are complete. [Task checklist](../task-checklist.md).

For each actual experiment record: date, board/module markings, sketch/settings, source/library version, wiring/power/pull-ups, operators and their actual roles, raw observations, calculations, uncertainty/limitations, conclusions and remaining work. Put original pictures in [pictures](../pictures/README.md) and use valid relative links only after files exist.

| Task | Required actual evidence | Result / artefact link |
|---|---|---|
| 1 | Defined divider topology/parts, paired input/output readings and comparison | [Two original voltage photos](task01-voltage-divider/README.md) documented; nodes, resistance values and original 5 V divider proof Pending |
| 2 | Actual assigned bus profile, pitch notes and real shared artefact | Pending assignment/discussion |
| 3 | Shared class table and actual discussion/personal note | [Pod 4 CAN row and research answers](../bus-comparison.md) Prepared; classroom artefact and personal confirmation Pending |
| 4a/4b | Student study/verification, neighbouring-pod agreement and Discord post | Pending |
| 5 | First scan, changed connection scan, address-to-module identification | [NACK baseline and scanner record](#scanner-report--task-5-partial-baseline) available; real device ACK/mapping/unplug comparison Pending |
| 6 | Library value and raw register reconstruction under matched settings | [Report / empty result table](../register-reading.md) Prepared; sensor/library/configuration/readings Pending |
| 7 | A-only/B-only/both scans and actual readings of both devices | [Report / address table](../two-devices.md) Prepared; actual two-device tests Pending |
| 8 | Conflict/fix evidence if hardware available; otherwise explicitly theoretical | Theory prepared; physical experiment Pending |
| 9 | Install/version/pins, PWM/I2C captures, measured timebase and class decision | [CH0 LOW/HIGH verified](task09-logic-analyzer/README.md), 10,000 samples each; [ESP8266 PWM verified](task09-esp8266-pwm.md); [I2C NACK-only capture verified](task09-esp8266-i2c.md); sensor ACK/calibrated timebase/class decision Pending |
| 10 | Optional decoded transaction and measured SCL | Optional / not tested |

## Scanner Report — Task 5 Partial Baseline

**Completed evidence reuse:** the preserved [Task 9 ESP8266 NACK experiment](task09-esp8266-i2c.md) and [actual serial text](task09-esp8266-i2c/source-single-scan-115200.txt) document a real no-target scan: **0 ACK, 112 NACK, 0 other errors** over ordinary addresses 0x08–0x77. The [raw acquisition](task09-esp8266-i2c/pulseview-i2c-esp8266-nack-01.sr) and its [decoder screenshot](task09-esp8266-i2c/i2c-nack-detail.jpg) show 12 complete **requests** (0x08–0x13), all NACK. They are a reusable scanner/transport baseline, not 12 found sensors. They were produced by the linked Task 9 source, not a new run of the revised Task 5 sketch.

**Prepared / Awaiting hardware:** [Task 5 dual-board-compatible scanner](../code/task05_i2c_scanner/README.md) and [Chinese procedure](../wiring-zh.md). With actual responding modules, finish the original task by matching each ACK address to a datasheet, powering off to remove one sensor, then powering on and comparing scans. No real ACK or unplug/address-difference result is currently available.

| Run | Required configuration | Actual ACK addresses / NACK / errors | Device identity / datasheet | Evidence / status |
|---|---|---|---|---|
| Existing no-target baseline | WeMos ESP8266, D2 SDA / D1 SCL, no sensor; requested 100 kHz | No ACK; 112 NACK; 0 other errors in the separate serial scan | No sensor present; no address-to-model mapping | [Real baseline](task09-esp8266-i2c.md), recorded 2026-10-28; sample-derived SCL estimate ~74.53 kHz at configured uncalibrated 1 MHz |
| Sensor connected | Model, revision, wiring, pull-ups and supply Pending | Pending | Pending | Pending |
| One sensor removed, after power-off | Identify removed module and all remaining connections | Pending | Pending | Pending |
| Sensor reconnected, after power-off | Restore original verified configuration | Pending | Pending | Pending |

### Blank Hardware Report Fields

| Date / operator / actual board and port | Sketch commit / core | SDA/SCL / pull-ups and supply | Connected module models | Observed 7-bit ACK addresses | Datasheet mapping | Address removed/restored / errors | Raw log / photo |
|---|---|---|---|---|---|---|---|
| | | | | | | | |
| | | | | | | | |

Record a predicted address separately from the actual scan. If no ACK occurs, diagnose voltage, common ground, SDA/SCL, address-pin state, reset and pull-ups. Do not fill a model from an address alone, copy a datasheet value into the observed column, or infer physical device count from one ACK. A no-target NACK run cannot satisfy the sensor removal comparison. Conclusion on device identification/unplug behavior: **Pending**.
