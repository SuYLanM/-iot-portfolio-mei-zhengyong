# Task 3 — Bus Comparison (Research Draft)

**Prepared comparison; not the merged class table.** Our confirmed assignment is **Pod 4 — CAN**. Other pods' pitches, the shared class table and actual classroom conclusions remain Pending. See [survey notes](bus-survey-notes.md) for history and source verification. Signal counts exclude ground/power; differential systems also need managed common-mode/reference paths.

| Bus / layer | Wires and connectors | Signaling / drive | Topology and addressing | Speed / distance qualification | Use / possible kit example | Debug tool |
|---|---|---|---|---|---|---|
| UART logic | TX/RX; optional RTS/CTS | Single-ended logic, normally push-pull, 3.3 V here | Usually point-to-point; no native device address | 115200 baud proposed; short board wiring, not a long-cable standard | Console / module, exact kit device Pending | Serial monitor + UART decoder |
| RS-232 electrical | TX/RX/reference minimum; DE-9/DB-25 conventions vary | Bipolar single-ended; inverted relative to TTL UART; level transceiver | Point-to-point DTE/DCE; no native addressing | Legacy lengths/rates depend on capacitance and hardware | Legacy instrument; no kit device confirmed | Correct-level adapter, scope |
| RS-485 electrical | One twisted pair half-duplex or two pairs full-duplex; reference strategy | Differential, tri-statable drivers | Multi-drop; one enabled driver at a time in ordinary half-duplex; no inherent addresses/arbitration | Rate decreases with cable length; do not combine 10 Mb/s and 1200 m as one guarantee | Industrial nodes; transceiver Pending | Differential probe / compatible receiver + UART decoder |
| Modbus RTU protocol | Commonly RS-485, also possible over other serial electrical layers | Inherits physical layer, not a new voltage standard | Controller/client polls addressed devices; function codes/register model | Device baud/parity/turnaround and RTU timing must match; distance inherited | Industrial meter; actual register map Pending | Protocol-aware serial capture, CRC/frame checking |
| Classical high-speed CAN | CAN_H/CAN_L pair + reference strategy | Differential dominant/recessive; transceiver required | Multi-controller broadcast; arbitration/message identifiers, not sensor slave addresses | Up to 1 Mb/s in classical CAN; attainable distance is bit-timing/transceiver/cable dependent | Motor-control network; actual motor controller Pending | CAN adapter/decoder, differential scope |
| I2C | SDA/SCL + GND, no universal connector | Open-drain with compatible pull-ups, wired-low sharing | Addressed multi-target; multi-controller arbitration possible | Start at 100 kHz; 400 kHz if every component/rise time permits; capacitance-limited board wiring | Sensor/OLED; Task 9 no-target scan observed NACK only, no device identified | Scanner + digital decoder; analog scope for rise time |
| I3C | SDA/SCL + GND | Mixed open-drain/push-pull phases, device-defined voltage | Dynamic addressing, controller coordination; legacy coexistence conditional | SDR clock up to 12.5 MHz; short interconnect, not a cable bus promise | Modern sensors, none confirmed; classic ESP32 Wire is not I3C | I3C-capable analyzer/controller |
| SPI | SCK/MOSI/MISO + CS per target; variants exist | Normally push-pull; inactive target must release shared MISO | Controller/targets selected by CS; no universal in-band addressing | MHz-class common; device timing/routing set actual rate and length, no universal limit | SD/ADC/flash; exact kit example Pending | Logic analyzer incl. CS, CPOL/CPHA checks |
| OneWire / 1-Wire | DQ + GND; power often separate | Shared open-drain, pull-up; parasite power may need strong pull-up | One controller, ROM-ID-selected devices | Time-slot/device-specific; network cable limits depend on topology/power/load | DS18B20 documented example, not confirmed inventory | 1-Wire decoder + rise-time/power checks |

Sources: [UART](references.md#r3-arduino-wire-and-uart), [RS-232](references.md#r5-rs232), [RS-485](references.md#r6-rs485), [Modbus](references.md#r7-modbus), [CAN](references.md#r8-can), [I2C](references.md#r4-i2c-standard), [I3C/SPI](references.md#r9-i3c-and-spi), [1-Wire](references.md#r10-sensor-and-switch-datasheets). Exact kit instances and measured rate/length remain Pending.

## Voltage Interpretation — Source-checked Examples, Not Wiring Measurements

| Interface | Voltage meaning / constraint |
|---|---|
| UART/GPIO/PWM here | 0/3.3 V nominal logic; input thresholds and absolute limits follow the actual ESP32 I/O domain |
| RS-232 | Bipolar line signaling; receiver decisions outside ±3 V, with a proper line transceiver; not 0/3.3 V UART voltage |
| RS-485 | Differential receiver threshold and allowed common-mode voltage are separate quantities; select the transceiver from its data sheet, not a generic “5 V bus” label |
| Modbus RTU | No independent voltage level; inherits its RS-485/RS-232/etc. physical implementation |
| CAN | MCU-side logic supply and CAN_H/CAN_L bus states are different; transceiver-specific common-mode/threshold limits apply |
| I2C | Our intended pull-up rail is compatible 3.3 V; module VCC/VIN is not proof of its pull-up voltage |
| I3C | Device-defined low-voltage CMOS/mixed drive; legacy coexistence is conditional, not automatic 3.3 V compatibility |
| SPI | Selected device logic domains must match or be translated; there is no universal 3.3 V SPI electrical specification |
| OneWire | DS18B20 data sheet permits 3.0–5.5 V supply; this does not allow a 5 V pull-up on an unprotected ESP32 input. Use reviewed compatible levels |

Connector/pin voltage must be measured and compared with each actual part. No table entry establishes safe direct attachment to an unknown module.

## Questions and Proposed Application Choices

1. **Point-to-point / multi-drop / multi-controller:** UART and RS-232 normally connect fixed peers. RS-485 is a shared electrical medium, but a higher protocol must schedule transmitters. I2C and CAN support shared addressed/identified traffic and arbitration in their respective modes; SPI normally has one controlling clock source and CS-selected targets. A multi-drop medium is not automatically multi-controller.
2. **Differential and termination:** RS-485 and high-speed CAN use differential pairs. Terminate the two physical ends as required by the cable/transceiver (often 120 Ω), not every node; short stubs matter. I2C and 1-Wire use pull-ups, which are not transmission-line termination resistors. SPI may need damping according to routing, not blanket parallel 120 Ω resistors.
3. **Clocking:** UART/serial RS-232/RS-485 uses configured baud and start/stop framing; Modbus adds protocol framing. I2C/I3C and SPI carry explicit clock signals. CAN synchronizes from transitions with bit stuffing rather than a separate clock conductor. OneWire uses defined time slots, not a shared SCL pin. “Self-clocking” must be defined before comparing these mechanisms.
4. **Open-drain versus push-pull:** released open-drain lines rise through pull-ups; devices assert LOW rather than drive conflicting HIGHs. Push-pull actively drives both levels and requires explicit ownership. Differential describes how a receiver compares wires; it is not a synonym for open-drain. Shared pull-up voltage must be accepted by every attached I/O; multiple rails require an appropriate bidirectional interface, not a resistor divider on SDA alone.
5. **20 m across a room:** propose RS-485 with a defined higher-layer protocol, termination and common-mode/isolation design; this is a design recommendation, not our tested 20 m link.
6. **20 sensors on one board:** propose I2C if addresses are distinct/configurable and pull-up/capacitance/rise-time limits permit; use multiplexing for collisions. Twenty physical sensors are not guaranteed simply because 7 bits exist. SPI is an alternative when speed, CS budget and target behavior suit it.
7. **Motor driver:** use PWM/direction with a suitable power driver for a local simple motor, or CAN for a networked drive already supporting CAN/CANopen. Never connect the motor winding to a logic pin. The driver's interface and safety requirements decide; a bus cannot replace a power stage.
8. **Oldest named interface:** TI dates RS-232 to 1962, earlier than the other specifically dated standards here; UART's underlying asynchronous-serial concept is older, but no single UART invention date is established by this research. RS-232's modem DTE/DCE heritage remains visible in handshake and connector conventions.

## Pod 4 CAN Row — Prepared Contribution, Not the Shared Class Result

This row follows all nine suggested course columns. The existing [CAN profile](bus-can.md) covers wires/connectors, electrical signaling and drive, topology/IDs, conditional speed/length, applications and kit example; its troubleshooting section supplies the debug approach. The only format gap was that those facts were spread over the profile rather than assembled in the exact class-table layout. No CAN hardware result is added by this formatting.

| Bus | Wires | Signaling | Drive style | Topology / addressing | Speed / length | Typical use | Kit example | Debug tool |
|---|---|---|---|---|---|---|---|---|
| Classical high-speed CAN | CANH/CANL twisted pair plus reference strategy; terminate physical ends, normally 120 ohm across pair at each end | Differential CANH−CANL; MCU logic and cable voltages are separate | CAN transceiver asserts dominant 0 or releases recessive 1; simultaneous transmitters arbitrate bitwise | Multi-controller broadcast on a trunk with short stubs; 11-/29-bit message IDs express priority/content, not I2C-style device addresses | Classical CAN up to 1 Mbit/s; portfolio example requests 500 kbit/s; length depends on bit timing, cable/stubs/transceivers, not a measured setup limit | ECUs, networked drives, industrial sensors/actuators | Proposed ESP32 TWAI + 3.3 V SN65HVD230 transceiver; not hardware validated; classic ESP32 is not CAN FD | Active compatible CAN peer/analyzer, TWAI alerts/error counters; differential scope for physical faults; two-end termination check |

[Verified CAN sources and qualifications](can-technical-notes.md). A passive/listen-only peer does not supply normal CAN ACK. No motor controller, CAN cable performance or classroom test is claimed.

## What Surprised Me — Proposed English Reflection for Student Review

**Draft wording only:** the following is an AI-assisted reflection prepared at the student's request. Review and personalize it before presenting it as a personal learning experience; it is not evidence that a discussion has occurred.

> What surprised me is that differential wiring alone does not determine how a bus is shared. RS-485 needs a higher-level rule to decide who may transmit, whereas CAN includes priority arbitration. I also found the distinction between a CAN message identifier and an I2C device address useful: they solve different problems. This makes me want to check both the physical layer and the protocol before choosing a bus for a sensor or motor-control system.

## Class Contributions — Pending Classroom Activity

The numbered answers above are prepared research responses for all six discussion themes, not a record of a class vote. Take the CAN row to class, compare it against the actual shared artefact and revise it only using real feedback.

| Actual discussion date / participating pods | Shared table / artefact link | Actual Pod 4 row/contribution | Changes after pitches | Actual agreed points |
|---|---|---|---|---|
| Pending | Pending | Pending classroom contribution; prepared row above | Pending | Pending |

Completed preparation: CAN row, comparison research and proposed discussion answers. Still required: actual shared table/link or screenshot, actual Pod 4 contribution, and a student-confirmed personal surprise note. No class outcome or individual contribution is fabricated.
