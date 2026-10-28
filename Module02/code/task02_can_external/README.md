# Task 2 CAN — B: External Communication

**Prepared / CAN Hardware Test Pending.** [Sketch](task02_can_external.ino) · [CAN report](../../bus-can.md) · [Chinese/English wiring guide](../../can-wiring.md) · [Blank results](../../evidence/task02-can-test-record.md).

## Parts and Wiring

Classic ESP32 Dev Module, USB cable, verified SN65HVD230 3.3 V module, short twisted pair, two appropriate end terminations (including any already on modules), and **a second active CAN controller with transceiver** or compatible active CAN adapter. Use an appropriate common reference for the bench setup.

| ESP32 | SN65HVD230 |
|---|---|
| GPIO5 TWAI TX output | TXD / D input |
| GPIO4 TWAI RX input | RXD / R output |
| 3V3 | VCC |
| GND | GND |
| Shared bus CANH / CANL | CANH / CANL respectively |

Power off for wiring. Confirm actual module labels, GPIO5 boot-strapping interaction, mode circuitry, supply and onboard termination before connecting. RS/STB/enable wiring remains **Pending module identification**; do not assume a generic module has a particular strap. SN65HVD230's RS LOW selects high speed; HIGH selects standby with driver off. Refer to the actual board schematic and [TI data sheet](https://www.ti.com/lit/ds/symlink/sn65hvd230.pdf). GPIO logic does not connect directly to CANH/CANL. Place 120 Ω across H/L at each of the **two physical trunk ends only**.

## Build and Run

1. Open `task02_can_external.ino`; select **ESP32 Dev Module**, installed Core **3.3.12**, actual USB port and Serial Monitor **115200 baud, 8N1**. No MCP2515 library is needed; the program uses the native TWAI node driver.
2. Board A defaults to `NODE_B=false`, standard ID 0x321. On board B set `NODE_B=true` and recompile; it publishes ID 0x322. Both use **500 kbit/s Classical CAN**. These IDs are test configuration, not discovered addresses.
3. Payload: A1/B2 node marker, four-byte little-endian sequence, CA 55 AA. It is generated test data, not a real sensor reading. The program sends approximately once per second and accepts all **standard** IDs; it does not configure an extended-frame acceptance filter.
4. Compare both real consoles: the other node's RX ID/DLC/eight bytes must match each transmission. An adapter must be active and compatible; a listen-only analyzer cannot supply ACK. Capture analyzer output if available. Record actual byte/sequence agreement rather than assume zero loss.

## Completion and Errors

- `TX queue ... ESP_OK` means the driver accepted the frame; it is **not delivery**.
- `TX_ACKED` comes from the successful TX-completion callback in normal mode, with no-ACK and self-reception disabled. It is link-layer success only. Verify the actual peer's application RX separately.
- Single-shot (`fail_retry_cnt=0`) bounds retries. A missing external ACK is reported through the driver's ACK error flag and failed completion; it is not treated as success. Bit/form/stuff errors, arbitration loss, controller state, TEC/REC and cumulative bus errors are printed. Arbitration loss can be normal contention.
- Queue timeout and a 750 ms TX-completion deadline are distinct. After a completion deadline expires, the pending buffer is retained and no new TX is issued until its callback arrives. Correct the fault; reset if completion never arrives. This protects the driver's pointer-based TX lifetime.
- Bus-off pauses new TX. After fixing the cause, send `r` in Serial Monitor to request recovery; the driver must observe the protocol recovery sequence. Recovery is not a test pass. A lone missing-ACK case need not progress to bus-off because CAN error-counter rules include exceptions.
- ISR callbacks only enqueue records; no Serial or blocking operation is performed there. Finite queues may drop events at high load. This example is intended for a quiet bench test, not lossless capture, production control or an active vehicle network.

For a planned missing-peer negative test, power off before changing the wiring, then restart and record actual errors. Keep both end terminations correct. Do not deliberately short bus lines or inject overvoltage.

## Software Validation

**Compile PASS — 2026-10-28.** Installed Arduino CLI, `esp32:esp32:esp32`, Core 3.3.12 / IDF 5.5.5. Result: 287616 bytes flash, 22204 bytes global RAM.

Command from repository root: `arduino-cli compile --fqbn esp32:esp32:esp32 --jobs 4 Module02/code/task02_can_external` (build output redirected to a temporary directory). No upload was performed. Physical communication and genuine captures remain **Pending**.
