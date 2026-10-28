# Module 02 — Buses, I2C and Sensor Addressing

**Preparation status: theory and compile-checked code prepared; all Module 02 hardware experiments Pending.** No scanned device addresses, physical readings, captures, classroom decisions or completed presentations are claimed.

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
| 9 Instrument sprint | [scope-decision.md](scope-decision.md) | [PWM reference](code/task09_pwm_reference/README.md) | Pinned candidate research Prepared; build/flash/captures/class choice Pending |
| 10 Decode waveform | [Optional guide](i2c-decode-guide.md) | Use a verified Task 9 instrument later | **Optional / Not yet tested** |

The original required English output filenames are retained. Task 2 now uses **bus-can.md** for the group's confirmed CAN selection; the [original generic profile](bus-profile-template.md), [pitch template](bus-pitch.md) and [candidate research](bus-survey-notes.md) remain available. Tasks 1–9 are core activities below the official draft boundary; Task 10 is a stretcher. Prepared documents or compiled stubs do not demonstrate hardware learning goals or instructor grading.

## Wiring, Code and Evidence

- [Detailed Chinese wiring / safety guide](wiring-zh.md).
- [CAN wiring / module checks](can-wiring.md) · [Blank CAN hardware and classroom record](evidence/task02-can-test-record.md). Local self-test is separate from two-node physical CAN verification.
- [Independent Arduino sketches and English instructions](code/README.md), target classic ESP32 Dev Module / Core 3.3.12. Task 1 now includes a separate GPIO35 fixed-resistor divider baseline; it does not replace the original 5 V measurement. Default scanner SDA21/SCL22 must be checked against the actual board and sensor modules.
- Unknown sensors/registers/addresses remain disabled placeholders; their templates emit Pending messages, not fabricated values. No third-party sensor library was selected.
- [Evidence register and blank report framework](evidence/README.md) · [Pictures directory](pictures/README.md). No Module 02 photos/logs are supplied yet; create links only after actual files exist.
- [Software validation](validation.md). External instrument firmware was researched, not installed or compiled here. GPIO22/23 conflict and Core 2/3 compatibility are explicit in the scope record.

## Next Actual Session

1. Identify board/modules, meter-check voltage/pull-ups/divider and test UART.
2. Review the Pod 4 CAN profile together, rehearse/deliver the actual pitch, record other pods' points and perform real Discord sharing. Inspect CAN modules before the optional supporting hardware demonstrations.
3. Scan one device and changed configurations, then implement library/register adapters for the real sensor.
4. Test two devices, and an address conflict only if suitable hardware is available.
5. Evaluate instruments on separate reviewed boards, compare actual captures, and make the real class decision.
6. Complete personal/pod understanding, neighbouring-pod discussion and real Discord sharing. Optional Task 10 follows working instrumentation.

Existing Module 01, Module 0, Reflection and original template archive remain intact. [Original Module 02 template](../docs/archive/module02-template.txt) is historical material, not completed evidence.
