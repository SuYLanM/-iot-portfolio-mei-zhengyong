# Task 8 — I2C Address Conflict Analysis

**Prepared theoretical engineering note; no physical conflict experiment or simulation performed.** Actual pair of same-address devices: Pending. [Mux template](code/task08_mux_template/README.md).

## Why a Scanner Can Mislead

Both targets may pull SDA LOW to ACK the same address. The scanner then sees one responding address, not two identities. During a write both may accept the data; during a read their open-drain outputs combine as wired LOW, which can yield wrong or even plausible-looking values. Reads are not guaranteed to fail visibly. Different simultaneous data is not ordinary push-pull HIGH-versus-LOW contention, but the transaction meaning is still ambiguous.

## Resolution Options

| Method | Required verification | Trade-off / failure to avoid |
|---|---|---|
| Address pin/jumper | Datasheet truth table, valid strap rails and power-up sampling | Only available choices; re-scan and do not leave strap floating |
| Configuration register/API | Documented address-change sequence and persistence | Isolate/reset one same-address device at a time before configuration; both may change if commanded together; verify after reset |
| TCA9548A switch | Actual module identity, straps, supply, pull-ups, reset and channel wiring | Put conflicting targets on separate channels and enable only one matching-address segment at a time |
| Separate controller/bus or shutdown control | Board peripherals, pins and target reset behavior | Costs pins/resources; switched-off devices must not clamp/backfeed the shared bus |

[TI TCA9548A datasheet](https://www.ti.com/lit/ds/symlink/tca9548a.pdf) documents eight channels and strap-selected 7-bit addresses 0x70–0x77. These are **documented options, not discovered hardware addresses**. One control byte selects channel bits. The example remains disabled until the actual address and channel are verified; it selects one channel, reports status and deselects all. Downstream sensor initialization/reads still need actual device adapters. Do not enable two same-address branches together. A stuck branch may require hardware RESET; a bus already stuck LOW may not accept a software deselect.

The switch can support level translation under its datasheet conditions, but that does not make the upstream ESP32 5 V tolerant or prove an arbitrary breakout safely wired. For the first lab keep all reviewed pull-ups at compatible 3.3 V levels.

## Planned Evidence / Actual Status

If two matching devices are available: scan/read separately, connect together under reviewed wiring, document actual symptoms, apply one verified remedy and re-scan/read. If they are unavailable, keep this as theory and record the missing hardware. A clearly labelled future simulation could supplement it; none is claimed here.

| Date / actual pair / starting addresses | Observed conflict, not assumed | Resolution / exact configuration | Before/after scan and read evidence | Remaining limits |
|---|---|---|---|---|
| Pending | Pending | Pending | Pending | Pending |
