# CAN Technical Notes and Verification

**Research Prepared / hardware untested.** [One-page profile](bus-can.md) · [Wiring](can-wiring.md) · [Blank records](evidence/task02-can-test-record.md).

## History and Applications

Increasingly electronic engine management and ABS created a need to exchange information between previously separate automotive controllers. Bosch sought a reliable, economical embedded network rather than expensive computer LAN hardware or a collection of basic serial links. CiA's retrospective identifies substantial development in 1984 and the SAE Detroit presentation in February 1986. We use these documented milestones rather than claim an exact project-start date from conflicting summaries. Industrial CANopen drives and distributed sensor nodes apply the same CAN link outside cars; the application protocol defines units, commands and device behavior. These are researched use cases, not equipment demonstrated by our group. [CiA history](https://www.can-cia.org/fileadmin/cia/documents/publications/cnlm/previous_issues/17-1_cnlm.pdf), [TI applications](https://www.ti.com/lit/ds/symlink/sn65hvd230.pdf).

## Signaling, Access and Identifiers

An ISO 11898-2 high-speed CAN segment is normally a linear twisted-pair trunk. A node's controller generates frames and monitors bit timing; its transceiver drives/receives the differential pair. The receiver senses **CANH minus CANL**, within specified common-mode limits. Dominant is logical 0 with a positive differential; recessive is logical 1 with a small differential and released driver. One dominant transmitter overrides recessive transmitters. Do not apply generic 5 V-transceiver voltage drawings as measured SN65HVD230 values, or treat CANH/CANL as ESP32 GPIO signals. [TI electrical characteristics and bus-state description](https://www.ti.com/lit/ds/symlink/sn65hvd230.pdf).

Every controller may start when the bus is idle. During arbitration, a transmitter sending recessive but observing dominant loses and becomes a receiver. The winner continues intact: **non-destructive bitwise arbitration**, unlike collision-and-backoff Ethernet. For example, two standard data frames 0x100 and 0x200 starting together favor 0x100. This is a logical example, not a capture. Standard identifiers are 11-bit (0x000–0x7FF); extended identifiers are 29-bit (0x00000000–0x1FFFFFFF). SRR/IDE/RTR also participate: a standard data frame wins against an extended data frame with the same first 11 ID bits. Compare full arbitration fields, not unlike-format numbers alone. Distinct publishers should not transmit different data under the same arbitration identity; a collision after arbitration becomes an error. [CiA data exchange / arbitration](https://www.can-cia.org/can-knowledge/can-cc).

IDs normally identify message content and priority. Raw CAN does not assign I2C-style slave addresses; higher layers can encode source/destination information. High-priority traffic can delay lower priorities indefinitely unless network load and worst-case timing are engineered. Arbitration is deterministic; arbitrary message delivery time is not automatically bounded.

## Frames, CRC, ACK and Fault Handling

Classical base data frame, excluding inserted stuff bits:

```text
SOF | arbitration (11-bit ID + RTR) | control (IDE, reserved, DLC)
    | data (0..8 bytes) | CRC sequence (15 bits) + delimiter
    | ACK slot + delimiter | EOF (7 recessive bits) | intermission
```

The extended frame adds SRR/IDE and an 18-bit identifier extension. DLC is a length code; the examples use DLC=8 with 8 actual bytes and standard data frames, not remote requests. In Classical CAN, bit stuffing applies from SOF through the CRC sequence: after five equal bits, the transmitter inserts the opposite bit. Receivers check stuffing, bit consistency, fixed fields, CRC and ACK. [Bosch CAN 2.0 protocol reproduced by NXP](https://www.nxp.com/docs/en/reference-manual/BCANPSV2.pdf), protocol/message-transfer/error sections.

The transmitter sends recessive in the ACK slot; another correctly receiving active controller supplies dominant. ACK confirms link-layer reception by at least one controller, not recipient identity, correct semantic interpretation or execution of a command. A listen-only analyzer does not ACK; the transmitting controller cannot provide its own external ACK. Application counters and peer-side logs are therefore required. Acceptance filters do not turn ACK into a per-application receipt.

Detected errors can generate error flags and abort the current frame. Ordinary operation may retry automatically; our external teaching example deliberately uses **single-shot** (`fail_retry_cnt=0`) so a missing peer does not cause endless retries. It reports TX completion separately from queue acceptance and logs ACK/bit/form/stuff/arbitration flags. Arbitration loss is expected contention, not automatically a defective bus.

Transmit/receive error counters support error-active, error-passive and bus-off confinement. Classical CAN becomes error-passive at a counter of at least 128; bus-off occurs when TEC exceeds 255. Missing-ACK counter behavior has protocol exceptions: do not promise that one disconnected peer always causes bus-off. The example pauses transmissions during bus-off and accepts a manual recovery request only after faults are corrected. Recovery requires observing the protocol's idle sequence (128 occurrences of 11 recessive bits), not an immediate successful communication result. [Bosch protocol / NXP](https://www.nxp.com/docs/en/reference-manual/BCANPSV2.pdf), [Espressif driver](https://docs.espressif.com/projects/esp-idf/en/v5.5/esp32/api-reference/peripherals/twai.html).

## Classical CAN Versus CAN FD

| Feature | Classical CAN | CAN FD |
|---|---|---|
| Payload per data frame | 0–8 bytes | Up to 64 bytes; DLC has a non-linear mapping above 8 |
| Identifier | 11 or 29 bits | 11 or 29 bits |
| Timing | One nominal rate, up to 1 Mbit/s | Nominal arbitration rate; optional faster data phase through BRS |
| CRC sequence | 15 bits | 17 or 21 bits depending on payload; additional safeguards |
| Remote frames | Supported by protocol | Not supported |
| Our classic ESP32 | Supported by TWAI | **Not supported**; FD frames can be interpreted as errors |

Bosch introduced FD in 2012. No single FD “maximum speed” is claimed for every network: controller, FD-rated transceiver, topology and timing determine achievable data-phase speed. The SN65HVD230 is specified up to **1 Mbit/s**, not an FD performance upgrade. FD-capable controllers can exchange Classical frames; an active Classical-only node cannot simply coexist with arbitrary FD traffic. Our examples configure **500 kbit/s Classical CAN**, with no FD or BRS. [Bosch FD overview](https://www.bosch-semiconductors.com/products/ip-modules/can-protocols/can-fd/), [CiA FD protocol](https://www.can-cia.org/can-knowledge/can-fd-the-basic-idea), [ESP32 limits](https://docs.espressif.com/projects/esp-idf/en/v5.5/esp32/api-reference/peripherals/twai.html).

## Rate, Length and Termination

125, 250 and 500 kbit/s and 1 Mbit/s are common nominal Classical configurations, not throughput guarantees; framing, stuffing and load reduce useful payload throughput. All nodes must have compatible nominal timing and oscillator/sample-point margins. CiA calls 40 m at 1 Mbit/s **theoretical**, with shorter real implementations possible. Lower rates can allow longer networks, but no fixed “500 kbit/s equals X metres” guarantee is made for our untested hardware. Round-trip propagation, cable delay, transceiver delay, sample point, stubs, capacitance, connector quality and common-mode range limit operation. [CiA physical-layer guidance](https://www.can-cia.org/can-knowledge/can-hs-transmission).

Terminate the **two physical ends** of an approximately 120 Ω characteristic-impedance trunk with 120 Ω each, across CANH/CANL. Matching reduces reflections. Do not add one at every node or replace the pair with a resistor to ground. With power removed, two ordinary 120 Ω terminations ideally read about **60 Ω in parallel** across H/L, subject to connected circuitry; this is a diagnostic expectation, not our measured resistance. Check module/analyzer onboard termination switches before adding resistors. Keep stubs short; a star or long branch can create reflections. Differential signaling tolerates some common noise but does not eliminate ground/reference and isolation design.

## Comparison With Other Connections

| Link | Signals plus reference | Access / addressing | Strength and trade-off |
|---|---|---|---|
| CAN | CANH/CANL through transceivers | Multi-master; message-ID priority and broadcast | Robust distributed control, CRC/ACK/confinement; extra hardware, short Classical payload and load-dependent latency |
| I2C | SDA/SCL with pull-ups | Usually one controller, addressed targets; multi-controller possible | Compact sensor wiring; capacitance/rise-time and address conflicts constrain it |
| SPI | SCLK, MOSI, MISO, CS per target in a common full-duplex arrangement | Controller-selected target; no universal ID/framing | High board-level throughput; more wires and device-specific modes/protocols |
| UART | TX/RX, compatible single-ended logic | Usually fixed point-to-point peers, asynchronous characters | Simple console/modules; no built-in shared-bus arbitration or CAN-style link ACK/CRC |
| RS-485 | Differential pair via transceivers; reference arrangement | Electrical layer; access/addressing require an upper protocol | Long multi-drop links; termination and driver-enable management remain necessary |

RS-232 adds incompatible bipolar electrical levels to serial communication and needs a transceiver. Modbus RTU defines serial messages/addresses/CRC, commonly over RS-485; it is not equivalent to CAN arbitration. OneWire uses a shared open-drain data line with device ROM identifiers, while I3C evolves board-level sensor communication with dynamic addressing and additional signaling modes. See the separately [source-checked bus comparison](bus-comparison.md) and [source register](references.md).

## Engineering Curiosities and Limits

- **Urgent motor-control status versus slow temperature:** assigning different IDs can favor urgent traffic, but only after a real load/latency analysis. This is a proposed engineering example, not a tested safety design.
- **An ACK without application delivery:** a controller can ACK a valid frame yet filter it out of the application receive queue. Use a sequence number and peer log, not just a TX return value.
- **Fault containment has boundaries:** a bus-off controller stops participating, but cannot repair a cable short or transceiver stuck dominant. TI describes using SN65HVD230 standby to silence a runaway transmitter; the actual module must expose usable mode control before relying on it. [TI mode description](https://www.ti.com/lit/ds/symlink/sn65hvd230.pdf).
- Raw CAN supplies neither encryption nor sender authentication; CRC detects accidental corruption, not malicious commands. Application design must handle permissions, stale data and replay where relevant.

## Driver Verification and Source Limits

Locally checked: Arduino ESP32 **3.3.12** platform version, installed **ESP-IDF 5.5.5** header, `esp_twai_onchip.h`, `esp_twai.h`, `esp_twai_types.h`, and classic ESP32 `twai_ll.h`. Both sketches use the installed node API, with ISR callbacks copying to FreeRTOS queues; no Serial calls or blocking waits run in callbacks. Persistent TX frames/buffers remain alive until completion. The legacy `driver/twai.h` API is not mixed into these sketches.

The [v5.5.5 driver source](https://github.com/espressif/esp-idf/blob/v5.5.5/components/esp_driver_twai/esp_twai_onchip.c) configures RX input and TX output separately through the GPIO matrix. Program A selects the **same unused GPIO5 for both**, retaining input feedback from the locally driven pad. `enable_self_test` disables ACK checking and `enable_loopback` enables reception of transmitted frames; these flags alone do not create an electrically isolated internal bus on classic ESP32. GPIO5 is still physically driven and must remain disconnected. No transceiver, jumper, termination or CANH/CANL is involved. Source review and compilation do not establish that the local test has run successfully.

Sources were reviewed on 2026-10-28. The original Bosch-hosted CAN 2.0 PDF could not be fetched; the NXP-hosted protocol reproduction was used instead. No ISO paywalled document or classroom/hardware observation is claimed as independently inspected. Module identity, actual mode straps, wiring, electrical levels, communication, latency and achieved distance remain **Pending**.
