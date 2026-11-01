# Task 7 — Two Devices on One Bus

**Prepared / Awaiting hardware. No real two-device experiment has been completed.** [Source / instructions](code/task07_two_devices/README.md) · [Scanner](code/task05_i2c_scanner/README.md).

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

## Shared Wiring and Software Plan

| Controller choice | SDA shared by A and B | SCL shared by A and B | Other requirements |
|---|---|---|---|
| Blue WeMos D1 mini / ESP8266 | D2 / GPIO4 | D1 / GPIO5 | Common GND, each module's datasheet-approved supply, pull-ups to compatible 3.3 V only |
| Separate classic ESP32 target | GPIO21 | GPIO22 | Same electrical checks; do not flash the protected black analyzer |

```text
Controller SDA -----+----- module A SDA
                    +----- module B SDA
Controller SCL -----+----- module A SCL
                    +----- module B SCL
Controller GND -----+----- module A GND
                    +----- module B GND
```

Each signal needs a reviewed effective pull-up, which may already be fitted to a breakout. Supply wiring depends on the actual module; the schematic does not authorize joining two regulator outputs. If probing with the unchanged analyzer, source SDA→black GPIO21/CH0, source SCL→black GPIO19/CH1 and common GND; keep black GPIO22→GPIO23 local clock only. Never connect a source SCL to analyzer GPIO22.

1. Obtain two identified, compatible I2C **sensor devices** with distinct confirmed addresses; configurable-address versions of the same sensor can also work if configured correctly. A sensor plus OLED can demonstrate bus sharing, but does not by itself satisfy reading **two sensors** as requested by Task 7.
2. Use the [existing scanner](code/task05_i2c_scanner/README.md) to record A alone, B alone and both. Power off for each change. Map actual ACK addresses to each known module/datasheet. Existing no-target NACK traffic is not a substitute.
3. In the [same Task 7 sketch](code/task07_two_devices/task07_two_devices.ino), set actual `ADDRESS_A`/`ADDRESS_B`, `UNIT_A`/`UNIT_B`; implement `initializeDevices()`, `readDeviceA()` and `readDeviceB()` using verified libraries/register maps. Include both devices' real initialization and error checks. Only then enable `TWO_DEVICE_CONFIGURATION_VERIFIED`.
4. The framework probes both configured addresses, initializes adapters and prints separate validity flags plus finite values with units. ACK alone is not a sensor reading. Default hooks remain false; failed readings are not printed as zero. Scans and adapter reads must both pass before any hardware success can be claimed.
5. Compare each sensor's behavior alone and on the shared bus at the same mode/rate. Record errors, per-device timestamps/units and whether one failing device affects the other. A permanently stuck-low device can block all bus traffic; software validity flags do not create electrical isolation.

## Hardware Evidence and Conclusion — Pending

Use the existing device/address and run tables above for A-only, B-only and both. Add raw serial logs with **both real readings**, a labelled wiring photo, actual core/library versions and any protocol capture. Explain why no extra SDA/SCL pin was needed and which limits were observed versus only theoretical. Actual conclusion: Pending.

[Chinese Task 5–7 operation guide](wiring-zh.md). Compilation validates the scaffold only. Models, actual addresses, adapters, measurements and a successful dual-sensor conclusion remain Pending.
