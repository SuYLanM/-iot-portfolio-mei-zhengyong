# Task 10 — See the Bits on the Wire

**Optional / Not yet tested.** The course marks this as a stretcher, not a separate guaranteed grading requirement. Depends on a physically verified Task 9 instrument and pin map. [Scope decision record](scope-decision.md).

1. Capture actual SDA/SCL with the digital analyzer, not the browser analog scope. Confirm ground, voltage, channel mapping and measured sample rate; do not connect SCL to the analyzer's reserved GPIO22.
2. Mark START (SDA falls while SCL is HIGH). Read the next eight bits MSB-first: seven address bits and R/W. Show the 7-bit address and address byte separately.
3. Identify the ninth-clock ACK/NACK and which receiver owns it. An address NACK differs from the controller's normal final-byte NACK during a read.
4. Decode data and optional repeated START; identify STOP. Measure actual clock periods and compare with the controller's configured clock, allowing stretching/gaps rather than simply trusting Wire.getClock().
5. Optional controlled degradation: only with low-voltage reviewed wiring and power off before changes, compare a longer lead or changed pull-up. Record what happens; do not assume it must fail. Restore the reviewed baseline afterwards. A digital trace shows logic decisions, not analog rise-time amplitude; use an appropriate analog instrument to quantify slopes.

| Actual capture / sample rate / channel map | START timestamp | 7-bit address / address byte | R/W | Address ACK | Data / receiver ACKs | Repeated START / final read NACK | STOP | Measured SCL versus configured | Actual integrity observation |
|---|---|---|---|---|---|---|---|---|---|
| Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending |

No synthetic waveform, guessed address or annotated screenshot is presented as measured evidence. Sources: [NXP specification](references.md#r4-i2c-standard), selected device map still Pending.
