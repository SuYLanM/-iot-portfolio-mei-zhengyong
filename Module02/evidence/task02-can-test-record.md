# Pod 4 CAN — Hardware and Classroom Record

**Blank template / Pending.** [Report](../bus-can.md) · [Wiring](../can-wiring.md). Enter actual evidence only after testing. No local self-test, physical CAN result, classroom activity or individual contribution is pre-filled as completed.

## Equipment and Configuration

| Date / operators | Board / module markings | IDE / Core | Serial port / baud | Sketch commit / node role | CAN rate / timing | Instrument / firmware |
|---|---|---|---|---|---|---|
| | | | | | | |

| Actual transceiver / module schematic | VCC measured / meter | RS / mode connection | End A termination | End B termination | Power-off H/L resistance | Cable length / stubs / reference |
|---|---|---|---|---|---|---|
| | | | | | | |

## A — Local Feedback Test

Record GPIO5 unconnected, exact sketch/core, observed TX and RX ID/DLC/data, sequence, timeout/mismatch count and the actual PASS/FAIL output. A local match does not establish external CAN communication.

| Timestamp | Actual TX ID / DLC / bytes | Actual RX ID / DLC / bytes | Observed PASS/FAIL / reason | Real capture link |
|---|---|---|---|---|
| | | | | |

## B — Two Active Nodes / CAN Adapter

| Run / direction | Sender ID / bytes / sequence | Sender completion / error | Peer RX ID / bytes / sequence | TEC / REC / bus errors | Real console / analyzer evidence |
|---|---|---|---|---|---|
| | | | | | |

| Planned condition | Actual setup change | Actual flags / observations | Interpretation / corrective action | Retest / evidence |
|---|---|---|---|---|
| Two active nodes, matching rate | | | | |
| Missing active peer (power-off rewiring) | | | | |
| Optional recovery if bus-off actually occurs | | | | |

Do not claim a bus-off event merely because ACK is absent. Do not claim a physical communication pass from queue acceptance, local feedback or ACK alone. Record matching peer-side frames and limitations. Measured rate/amplitude/latency require a suitable instrument and documented method; configured 500 kbit/s is not an independent measurement.

## Real Hardware Conclusion — Pending

- What actually worked:
- Observed failures and explanation:
- Evidence filenames / capture conditions:
- Remaining limitations and next test:

## Other Pods' Pitches and Sharing — Pending

| Date / other pod / bus | Actual point learned | Source / uncertainty | Follow-up question |
|---|---|---|---|
| | | | |

- Presentation date, speakers and actual duration:
- Neighbour-pod discussion / agreed points:
- Chosen table or diagram and actual Discord post URL:
- Mei Zhengyong's actual contribution:
- Felix's actual contribution:
- Each member's own I2C open-drain/pull-up explanation and questions still open:
