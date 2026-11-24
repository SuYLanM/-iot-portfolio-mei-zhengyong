# Module 02 — Buses, I2C and Sensor Addressing

**Task 9 A stages verified: real CH0 LOW/HIGH and ESP8266-source PWM captures, 10,000 samples each.** [Original .sr files, replay screenshots and checks](evidence/task09-logic-analyzer/README.md). [PWM record and sample-derived measurements](evidence/task09-esp8266-pwm.md) include a new grounded retest; the initial run lacked its dedicated ground wire and is retained as historical evidence; [NACK-only I2C capture](evidence/task09-esp8266-i2c.md) is now verified; sensor ACK/data, independent timebase calibration and the class decision remain Pending; other Module 02 hardware tasks retain their existing status.

[Portfolio home](../README.md) · [Official Module 2](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/02-i2c-and-sensor-addressing.md) · [Requirements checklist](task-checklist.md) · [Sources / verification](references.md)

## Learning Goals

The rows follow the numbering and meaning of the [official module goals](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/02-i2c-and-sensor-addressing.md).
Status describes the linked evidence, not an instructor assessment.

| Official goal (paraphrased) | How I learned or checked it | Existing evidence | Evidence status |
|---|---|---|---|
| 1. Compare direct pins/PWM with UART, RS-232/485, Modbus, CAN, I2C/I3C, SPI and OneWire. | I prepared the CAN profile and bus comparison, distinguishing modulation, electrical layers and addressed protocols. | [CAN profile](bus-can.md), [comparison](bus-comparison.md), [pin/divider report](pins-and-dividers.md) | Research prepared; Needs further evidence for personal explanation and class comparison/pitch. |
| 2. Explain I2C electrical operation, addressing and transaction features. | I related stuck-low diagnostics to a shared bus and used real ACK/NACK records; repeated-start and stretching explanations remain theory. | [Theory notes](study-log.md), [LOW diagnostic](evidence/task05-i2c-scanning/lcd-line-diagnostic-serial-20261102.txt), [positive scans](evidence/ds1302-lcd-extension/scanner-serial-20261102.txt) | Partial practical support; Needs further evidence for repeated-start/stretching observations. |
| 3. Wire and scan the bus and identify responding devices. | I obtained 0x27 in five sweeps, then no address in four sweeps after the LCD SDA connection was removed. This was SDA disconnection, not full module removal. | [Scanner](code/task05_i2c_scanner/README.md), [before log](evidence/ds1302-lcd-extension/scanner-serial-20261102.txt), [after log](evidence/task05-i2c-scanning/sda-disconnect/serial-20261102.txt) | Hardware verified within this LCD configuration. |
| 4. Compare library readings with direct sensor-register reads. | I prepared the BME280 example but did not obtain matching real sensor values. DS1302 is a three-wire RTC, not a replacement I2C sensor. | [Register report](register-reading.md), [example code](code/task06_library_vs_register/README.md), [RTC extension](evidence/ds1302-lcd-extension/README.md) | Needs further evidence: actual sensor-library and direct-register comparison. |
| 5. Operate two bus devices and resolve an address conflict. | I verified LCD 0x27 and a wired ESP32-controlled target at 0x42, including data checks. The second target is an MCU slave; conflict resolution remains theoretical. | [Dual-target report](two-devices.md), [real log](evidence/task07-two-devices/dual-wired-serial-20261102.txt), [photo](evidence/task07-two-devices/task07_lcd_dual_address_demo.jpg), [conflict notes](address-conflict.md) | Adaptation demonstrated; Needs further evidence for two real sensors and a physical conflict/fix. |
| 6. Evaluate a low-cost ESP32 instrument and agree class debugging tools. | I retained native captures for LOW/HIGH, grounded PWM and NACK-only I2C; sample-derived timing is not independent timebase calibration. | [Instrument report](scope-decision.md), [grounded capture](evidence/task09-esp8266-pwm/pulseview-pwm-esp8266-grounded-retest-01.sr), [replay image](evidence/task09-esp8266-pwm/pwm-grounded-retest-01-replay.jpg) | Instrument evidence partial; Needs further evidence for sensor-data decode and actual class agreement. |

## Group Members

- Mei Zhengyong — GitHub: [@SuYLanM](https://github.com/SuYLanM)
- Felix — GitHub: [@Felix-Gaoxinyu](https://github.com/Felix-Gaoxinyu)

Individual experimental/discussion contributions will be recorded only after they occur. The group has selected **Task 2 — Pod 4: CAN Bus**. **Research Prepared / CAN Hardware Test Pending**; the classroom pitch, sharing and final class instrument choice remain Pending.

## Task Navigation

Status wording applies to the stated scope: **Completed** means an identified evidence/preparation substep, not the entire module; **Prepared / Awaiting hardware** means code/report scaffolds exist without sensor validation; **Pending classroom activity** means actual shared artefacts, discussion or presentation are still missing.


| Task | English document / report framework | Arduino preparation | Current status |
|---|---|---|---|
| 1 Pins and Voltage Divider | [English report](pins-and-dividers.md), [two original photos](evidence/task01-voltage-divider/README.md) | [GPIO35 baseline](code/task01_voltage_divider/README.md), [optional UART](code/task01_uart_loopback/README.md) | Completed: photo integration; probe nodes/resistances and original 5 V divider proof Pending; fixed-resistor code Prepared |
| 2 Pod 4 — CAN Bus | [CAN profile](bus-can.md), [technical notes / comparison](can-technical-notes.md), [2–3 min speech](can-pitch.md), [research log](can-research-log.md) | [A local self-test](code/task02_can_self_test/README.md), [B external CAN](code/task02_can_external/README.md) | **Research Prepared / CAN Hardware Test Pending**; presentation and Discord sharing Pending |
| 3 Class Bus Comparison | [CAN nine-column row, research answers and draft reflection](bus-comparison.md) | — | Prepared: Pod 4 CAN contribution; Pending classroom activity: shared table, actual discussion and student-confirmed reflection |
| 4 / 4a / 4b I2C study | [study-log.md](study-log.md) | [Code index](code/README.md) | Theory Prepared; student/pod validation and sharing Pending |
| 5 I2C Scanning | [Before/after report and raw logs](evidence/README.md#sda-disconnection-comparison--2026-11-02) | [ESP32 / ESP8266 scanner](code/task05_i2c_scanner/README.md) | **Completed — LCD scanning and SDA-disconnection comparison:** five Before sweeps {0x27}; four After sweeps {}, 112 NACK / 0 errors each. SDA-only variant; full sensor/module removal and restoration not performed |
| 6 Library vs Direct Register Reading | [English report / real diagnostic logs](register-reading.md), [Chinese guide](wiring-zh.md#task-6库读取与直接寄存器读取) | [Current-hardware diagnostic](code/task06_current_hardware_diagnostic/README.md), [BME280 library/direct example](code/task06_library_vs_register/README.md) | Completed: both builds, diagnostic upload and three real BUS_BLOCKED rounds; sensor example Prepared / Not Hardware Verified; core sensor measurements/comparison 0/3 verified |
| 7 Two Devices on One Bus | [English report / real dual-target run](two-devices.md#wired-dual-target-hardware-run--2026-11-02), [Chinese guide](wiring-zh.md#task-7两只不同地址传感器共用总线) | [ESP32 master + Wire1](code/task07_two_devices/README.md), [compile-only ESP8266 alternative](code/task07_esp8266_slave/README.md), [separate model tests](code/task07_simulation/README.md) | **LCD+MCU adaptation Hardware Verified:** three real {0x27,0x42} scans, 2 ACK / 110 NACK / 0 errors, both data checks pass; first stale replies rejected, second accepted. Original two-sensor requirement remains unmet |
| 8 Address conflict | [address-conflict.md](address-conflict.md) | [Mux template](code/task08_mux_template/README.md) | Theory Prepared; no physical conflict/fix or simulation claimed |
| 9 A-class instrument preparation | [English decision report](scope-decision.md), [Chinese operation manual](task09-logic-analyzer-guide-zh.md), [build/source audit](task09-analyzer-build.md), [verified LOW/HIGH evidence](evidence/task09-logic-analyzer/README.md), [further records](evidence/task09-logic-analyzer-record.md) | [PWM target](code/task09_pwm_reference/README.md), [ESP8266 PWM source actually used](code/task09_esp8266_pwm_generator/README.md), [I2C capture target](code/task09_i2c_capture_source/README.md), [upstream Core 3 patch](patches/task09-core3-clock-retune.patch) | **Static LOW/HIGH and real ESP8266 PWM capture verified**; [PWM evidence](evidence/task09-esp8266-pwm.md); 1 MHz configured, not calibrated; I2C NACK-only capture verified; sensor ACK/data and class decision Pending |
| 10 Decode waveform | [Optional guide](i2c-decode-guide.md) | Use a verified Task 9 instrument later | **Optional / Not yet tested** |

**Separate extension:** [DS1302 + LCD1602 real hardware report](evidence/ds1302-lcd-extension/README.md) · [Arduino sketch](code/ds1302_lcd_extension/README.md). RTC read-only progression and LCD DDRAM readback verified; visible screen confirmation Pending. This does not complete Task 6 I2C sensor comparison.

The original required English output filenames are retained. Task 2 now uses **bus-can.md** for the group's confirmed CAN selection; the [original generic profile](bus-profile-template.md), [pitch template](bus-pitch.md) and [candidate research](bus-survey-notes.md) remain available. Tasks 1–9 are core activities below the official draft boundary; Task 10 is a stretcher. Prepared documents or compiled stubs do not demonstrate hardware learning goals or instructor grading.

[Task 9 final integrity audit](scope-decision.md#final-integrity-audit--2026-10-28) · [Prepared short classroom explanation](scope-decision.md#short-classroom-explanation--prepared-not-yet-delivered) · [Time and effort: approximately 90 minutes, retrospective estimate](scope-decision.md#time-and-effort--retrospective-estimate). Exact installation-to-first-capture timing is unrecorded; actual classroom demo and final class decision remain Pending.

## Wiring, Code and Evidence

- Task 9 I2C stage: [ESP8266 I2C source and two-board wiring](code/task09_esp8266_i2c_source/README.md), compile/upload PASS; [real NACK capture record](evidence/task09-esp8266-i2c.md). Sensor ACK/data remain Pending. The grounded retest is the official PWM result; the initial run is retained only as historical evidence.
- [Detailed Chinese wiring / safety guide](wiring-zh.md).
- [CAN wiring / module checks](can-wiring.md) · [Blank CAN hardware and classroom record](evidence/task02-can-test-record.md). Local self-test is separate from two-node physical CAN verification.
- [Independent Arduino sketches and English instructions](code/README.md), Tasks 5–7 now support classic ESP32 Dev Module / Core 3.3.12 and WeMos ESP8266 / Core 3.1.2. Task 1 now includes a separate GPIO35 fixed-resistor divider baseline; it does not replace the original 5 V measurement. Default scanner SDA21/SCL22 must be checked against the actual board and sensor modules.
- Unknown sensors/registers/addresses remain disabled placeholders; their templates emit Pending messages, not fabricated values. No third-party sensor library was selected.
- [Evidence register and blank report framework](evidence/README.md) · [Pictures directory](pictures/README.md). Task 9 original LOW/HIGH acquisitions and genuine replay screenshots are now [available](evidence/task09-logic-analyzer/README.md); Task 1 photographs are documented with unconfirmed nodes; Tasks 5 and 7 have the LCD/MCU evidence above; Task 6 real sensor comparison remains Pending.
- [Original software validation](validation.md) · [Task 9 follow-up build/host validation](task09-analyzer-build.md). Earlier Core 3.3.12 preparation is retained; original firmware also compiled with Core 3.3.7. The user confirmed IDE flashing, and original-firmware LOW/HIGH acquisition was verified without an assistant reflash or applied patch. GPIO22/23 conflicts and pending runtime timing checks remain explicit in the scope record. B-class research is unchanged.

## Next Actual Session

1. Identify board/modules, meter-check voltage/pull-ups/divider and test UART.
2. Review the Pod 4 CAN profile together, rehearse/deliver the actual pitch, record other pods' points and perform real Discord sharing. Inspect CAN modules before the optional supporting hardware demonstrations.
3. Scan one device and changed configurations, then implement library/register adapters for the real sensor.
4. Test two devices, and an address conflict only if suitable hardware is available.
5. Evaluate instruments on separate reviewed boards, compare actual captures, and make the real class decision.
6. Complete personal/pod understanding, neighbouring-pod discussion and real Discord sharing. Optional Task 10 follows working instrumentation.

Existing Module 01, Module 0, Reflection and original template archive remain intact. [Original Module 02 template](../docs/archive/module02-template.txt) is historical material, not completed evidence.


## Study log

### Evidence-led review and individual contribution

I retained the initial SDA/SCL LOW failure and the later positive scanner run.
The five connected sweeps found 0x27; four SDA-disconnected sweeps found none.
I then documented 0x27 and 0x42 with real data checks, identifying the second
as an ESP32-controlled target rather than a second sensor. These are stronger
checks than assuming that a configured address must respond. DS1302 uses its own
three-wire interface; neither it nor LCD output completes the sensor-register task.
Codex helped prepare scanner/diagnostic firmware, parse logs and investigate
instrument feasibility. I checked results with actual ACK/NACK logs, display
readback and native analyzer captures. Stale slave replies were rejected before
fresh ones were accepted. Sensor library/register comparison, physical address
conflict resolution and a final class instrument choice remain unverified.

The detailed [theory log](study-log.md), [instrument decision](scope-decision.md)
and [individual reflection](../Reflections/ref02.md) separate my archived work
from prepared CAN/classroom material. Specific Felix/pod feedback and Discord
sharing await my confirmation; no new hardware run was performed for this review.
