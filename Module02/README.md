# Module 02 — Buses, I2C and Sensor Addressing

**Task 9 A stages verified: real CH0 LOW/HIGH and ESP8266-source PWM captures, 10,000 samples each.** [Original .sr files, replay screenshots and checks](evidence/task09-logic-analyzer/README.md). [PWM record and sample-derived measurements](evidence/task09-esp8266-pwm.md) include a new grounded retest; the initial run lacked its dedicated ground wire and is retained as historical evidence; [NACK-only I2C capture](evidence/task09-esp8266-i2c.md) is now verified; sensor ACK/data, independent timebase calibration and the class decision remain Pending; other Module 02 hardware tasks retain their existing status.

[Portfolio home](../README.md) · [Official Module 2](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/02-i2c-and-sensor-addressing.md) · [Requirements checklist](task-checklist.md) · [Sources / verification](references.md)

## Group Members

- Mei Zhengyong — GitHub: [@SuYLanM](https://github.com/SuYLanM)
- Felix — GitHub: [@Felix-Gaoxinyu](https://github.com/Felix-Gaoxinyu)

Individual experimental/discussion contributions will be recorded only after they occur. The group has selected **Task 2 — Pod 4: CAN Bus**. **Research Prepared / CAN Hardware Test Pending**; the classroom pitch, sharing and final class instrument choice remain Pending.

## Task Navigation

| Task | English document / report framework | Arduino preparation | Current status |
|---|---|---|---|
| 1 Pins, PWM, UART, divider | [pins-and-dividers.md](pins-and-dividers.md) | [UART loopback](code/task01_uart_loopback/README.md), [GPIO35 divider](code/task01_voltage_divider/README.md) | Prepared / Awaiting hardware validation; 3.3 V baseline and original 5 V measurement Pending |
| 2 Pod 4 — CAN Bus | [CAN profile](bus-can.md), [technical notes / comparison](can-technical-notes.md), [2–3 min speech](can-pitch.md), [research log](can-research-log.md) | [A local self-test](code/task02_can_self_test/README.md), [B external CAN](code/task02_can_external/README.md) | **Research Prepared / CAN Hardware Test Pending**; presentation and Discord sharing Pending |
| 3 Class bus comparison | [bus-comparison.md](bus-comparison.md) | — | Research draft Prepared; actual class merge/discussion Pending |
| 4 / 4a / 4b I2C study | [study-log.md](study-log.md) | [Code index](code/README.md) | Theory Prepared; student/pod validation and sharing Pending |
| 5 I2C scan | [Blank scan report](evidence/README.md) | [Scanner](code/task05_i2c_scanner/README.md) | Code Prepared; **not hardware tested** |
| 6 Library versus register | [Comparison / decoding checklist](register-reading.md) | [Library template](code/task06_library_template/README.md), [register template](code/task06_register_template/README.md) | Disabled scaffolds Prepared; sensor/adapters/tests Pending |
| 7 Two devices | [Address table / dual-device report](two-devices.md) | [Framework](code/task07_two_devices/README.md) | Prepared; actual models/addresses/readings Pending |
| 8 Address conflict | [address-conflict.md](address-conflict.md) | [Mux template](code/task08_mux_template/README.md) | Theory Prepared; no physical conflict/fix or simulation claimed |
| 9 A-class instrument preparation | [English decision report](scope-decision.md), [Chinese operation manual](task09-logic-analyzer-guide-zh.md), [build/source audit](task09-analyzer-build.md), [verified LOW/HIGH evidence](evidence/task09-logic-analyzer/README.md), [further records](evidence/task09-logic-analyzer-record.md) | [PWM target](code/task09_pwm_reference/README.md), [ESP8266 PWM source actually used](code/task09_esp8266_pwm_generator/README.md), [I2C capture target](code/task09_i2c_capture_source/README.md), [upstream Core 3 patch](patches/task09-core3-clock-retune.patch) | **Static LOW/HIGH and real ESP8266 PWM capture verified**; [PWM evidence](evidence/task09-esp8266-pwm.md); 1 MHz configured, not calibrated; I2C NACK-only capture verified; sensor ACK/data and class decision Pending |
| 10 Decode waveform | [Optional guide](i2c-decode-guide.md) | Use a verified Task 9 instrument later | **Optional / Not yet tested** |

The original required English output filenames are retained. Task 2 now uses **bus-can.md** for the group's confirmed CAN selection; the [original generic profile](bus-profile-template.md), [pitch template](bus-pitch.md) and [candidate research](bus-survey-notes.md) remain available. Tasks 1–9 are core activities below the official draft boundary; Task 10 is a stretcher. Prepared documents or compiled stubs do not demonstrate hardware learning goals or instructor grading.

## Wiring, Code and Evidence

- Task 9 I2C stage: [ESP8266 I2C source and two-board wiring](code/task09_esp8266_i2c_source/README.md), compile/upload PASS; [real NACK capture record](evidence/task09-esp8266-i2c.md). Sensor ACK/data remain Pending. The grounded retest is the official PWM result; the initial run is retained only as historical evidence.
- [Detailed Chinese wiring / safety guide](wiring-zh.md).
- [CAN wiring / module checks](can-wiring.md) · [Blank CAN hardware and classroom record](evidence/task02-can-test-record.md). Local self-test is separate from two-node physical CAN verification.
- [Independent Arduino sketches and English instructions](code/README.md), target classic ESP32 Dev Module / Core 3.3.12. Task 1 now includes a separate GPIO35 fixed-resistor divider baseline; it does not replace the original 5 V measurement. Default scanner SDA21/SCL22 must be checked against the actual board and sensor modules.
- Unknown sensors/registers/addresses remain disabled placeholders; their templates emit Pending messages, not fabricated values. No third-party sensor library was selected.
- [Evidence register and blank report framework](evidence/README.md) · [Pictures directory](pictures/README.md). Task 9 original LOW/HIGH acquisitions and genuine replay screenshots are now [available](evidence/task09-logic-analyzer/README.md); wiring photos and dynamic captures are still Pending.
- [Original software validation](validation.md) · [Task 9 follow-up build/host validation](task09-analyzer-build.md). Earlier Core 3.3.12 preparation is retained; original firmware also compiled with Core 3.3.7. The user confirmed IDE flashing, and original-firmware LOW/HIGH acquisition was verified without an assistant reflash or applied patch. GPIO22/23 conflicts and pending runtime timing checks remain explicit in the scope record. B-class research is unchanged.

## Next Actual Session

1. Identify board/modules, meter-check voltage/pull-ups/divider and test UART.
2. Review the Pod 4 CAN profile together, rehearse/deliver the actual pitch, record other pods' points and perform real Discord sharing. Inspect CAN modules before the optional supporting hardware demonstrations.
3. Scan one device and changed configurations, then implement library/register adapters for the real sensor.
4. Test two devices, and an address conflict only if suitable hardware is available.
5. Evaluate instruments on separate reviewed boards, compare actual captures, and make the real class decision.
6. Complete personal/pod understanding, neighbouring-pod discussion and real Discord sharing. Optional Task 10 follows working instrumentation.

Existing Module 01, Module 0, Reflection and original template archive remain intact. [Original Module 02 template](../docs/archive/module02-template.txt) is historical material, not completed evidence.
