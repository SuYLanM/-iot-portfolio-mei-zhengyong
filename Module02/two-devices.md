# Task 7 — Two Devices on One Bus

**Framework prepared; actual models, addresses, reads and captures Pending.** [Source / instructions](code/task07_two_devices/README.md) · [Scanner](code/task05_i2c_scanner/README.md).

Two compatible targets share SDA/SCL because only the addressed target should participate in each transaction. The shared controller sequences reads; no second data pin is needed. Different addresses are necessary but not sufficient: supply/pull-up voltage, capacitance, clock speed, reset states and driver/software behavior must also work together.

Start at 100 kHz with short leads. Inspect existing pull-ups before adding more: parallel pull-ups increase sink current; adding targets/cables increases capacitance and slows rising edges. Select a speed supported by both devices and verified rise times. Address space has reserved values; adding twenty targets can fail long before address bits run out. One stuck-low participant can disable the whole bus.

Procedure: identify and scan A alone, power down and connect B alone, then power down and connect both. Save each scan and map addresses using datasheets. Acknowledgement does not identify model or count colliding devices. Implement both initialization/read hooks in the supplied sketch, then log valid/error results for each without replacing failed readings with zeros.

| Actual device | Model / interface | Datasheet address options (not observations) | Actual scanned 7-bit address | Configurable? / method | Voltage and pull-ups | Evidence |
|---|---|---|---|---|---|---|
| A | Pending | Pending | Pending | Pending | Pending | Pending |
| B | Pending | Pending | Pending | Pending | Pending | Pending |

| Run / date | Connected devices | Actual scan output link | A reading/unit | B reading/unit | Errors / bus clock / conditions |
|---|---|---|---|---|---|
| | | | | | |

No model or actual address has been selected. Use the [conflict analysis](address-conflict.md) if same-address devices must coexist. Sources: [NXP/Arduino](references.md#r4-i2c-standard), actual device datasheets still to select.
