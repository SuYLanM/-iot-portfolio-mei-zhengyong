# Task 3 — Bus Comparison (Research Draft)

**Prepared comparison; not the merged class table.** The assigned bus, other pods' pitches, class discussion and personal surprises remain Pending. See [survey notes](bus-survey-notes.md) for history and source verification. Signal counts exclude ground/power; differential systems also need managed common-mode/reference paths.

| Bus / layer | Wires and connectors | Signaling / drive | Topology and addressing | Speed / distance qualification | Use / possible kit example | Debug tool |
|---|---|---|---|---|---|---|
| UART logic | TX/RX; optional RTS/CTS | Single-ended logic, normally push-pull, 3.3 V here | Usually point-to-point; no native device address | 115200 baud proposed; short board wiring, not a long-cable standard | Console / module, exact kit device Pending | Serial monitor + UART decoder |
| RS-232 electrical | TX/RX/reference minimum; DE-9/DB-25 conventions vary | Bipolar single-ended; inverted relative to TTL UART; level transceiver | Point-to-point DTE/DCE; no native addressing | Legacy lengths/rates depend on capacitance and hardware | Legacy instrument; no kit device confirmed | Correct-level adapter, scope |
| RS-485 electrical | One twisted pair half-duplex or two pairs full-duplex; reference strategy | Differential, tri-statable drivers | Multi-drop; one enabled driver at a time in ordinary half-duplex; no inherent addresses/arbitration | Rate decreases with cable length; do not combine 10 Mb/s and 1200 m as one guarantee | Industrial nodes; transceiver Pending | Differential probe / compatible receiver + UART decoder |
| Modbus RTU protocol | Commonly RS-485, also possible over other serial electrical layers | Inherits physical layer, not a new voltage standard | Controller/client polls addressed devices; function codes/register model | Device baud/parity/turnaround and RTU timing must match; distance inherited | Industrial meter; actual register map Pending | Protocol-aware serial capture, CRC/frame checking |
| Classical high-speed CAN | CAN_H/CAN_L pair + reference strategy | Differential dominant/recessive; transceiver required | Multi-controller broadcast; arbitration/message identifiers, not sensor slave addresses | Up to 1 Mb/s in classical CAN; attainable distance is bit-timing/transceiver/cable dependent | Motor-control network; actual motor controller Pending | CAN adapter/decoder, differential scope |
| I2C | SDA/SCL + GND, no universal connector | Open-drain with compatible pull-ups, wired-low sharing | Addressed multi-target; multi-controller arbitration possible | Start at 100 kHz; 400 kHz if every component/rise time permits; capacitance-limited board wiring | Sensor/OLED; none scanned yet | Scanner + digital decoder; analog scope for rise time |
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

## Class Contributions — Pending

| Actual discussion date / participating pods | Shared table / artefact link | Changes after pitches | 3–5 jointly agreed points |
|---|---|---|---|
| TODO | TODO | TODO | TODO |

Personal “what surprised me” note: TODO, to be written after actual study/discussion. The proposed application choices above are AI-assisted research, not a class vote or an invented personal discovery.
