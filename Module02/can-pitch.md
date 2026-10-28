# Pod 4 — CAN Bus: English Classroom Speech

**Prepared speech; delivery and actual speaking duration Pending.** Approximately 330 words, intended for 2–3 minutes at a deliberate classroom pace. [Profile / diagram](bus-can.md) · [Comparison](can-technical-notes.md#comparison-with-other-connections).

Hello, we are Mei Zhengyong and Felix, and our group selected CAN, the Controller Area Network.

Bosch developed CAN to connect automotive electronic controllers reliably and economically. It was publicly presented in 1986. Today, CAN also connects industrial drives, robots and distributed sensors.

Our diagram separates three things. The ESP32's TWAI controller handles frames, arbitration and errors. The SN65HVD230 transceiver converts its logic signals into differential signals. CANH and CANL form the shared physical bus. A CAN controller is not itself a transceiver.

CAN is multi-master: any node may request transmission when the bus is idle. Dominant zero overrides recessive one. During arbitration, a node that sends one but reads zero stops transmitting, while the winning frame continues intact. Within the same frame format, a lower identifier has higher priority. The identifier usually describes a message, rather than simply naming its destination.

Classical CAN supports eleven-bit or twenty-nine-bit identifiers and up to eight data bytes. CAN FD permits up to sixty-four bytes and an optional faster data phase. Our classic ESP32 supports Classical CAN only. We will configure five hundred kilobits per second; this is a setting, not a measured result.

Frames contain a CRC, and another active controller acknowledges valid reception. ACK does not prove that an application processed the data. A single normal-mode node, or a listen-only analyzer, cannot supply the external acknowledgment we need.

Compared with I2C, CAN adds differential signaling and priority arbitration for distributed control. SPI is attractive for fast board-level peripherals. UART is simpler for a console but lacks CAN's shared-bus protocol and fault handling.

Our proposed physical bus uses a twisted pair with one one-hundred-and-twenty-ohm termination at each end, not at every node. Exact cable length depends on timing and wiring.

We have prepared a local self-test and a two-node example. Hardware communication, classroom discussion and Discord sharing remain pending. Our next step is to verify the actual modules and compare both nodes' real receive logs.
