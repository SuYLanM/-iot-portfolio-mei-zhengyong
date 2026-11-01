# Task 5 — Passive GPIO and I2C Initialization Diagnostic

**Compiled, uploaded and run on the real ESP32 / COM3 on 2026-11-02.** This is a separate diagnostic sketch, not the normal scanner. [Source](task05_gpio_diagnostic.ino) · [Task 5 report](../../evidence/README.md#passive-gpio-diagnostic-follow-up--2026-11-02).

## Build and Commands

Arduino IDE 2.3.10 / ESP32 Dev Module / installed Espressif Core 3.3.12. Compile PASS: 298152 flash bytes, 23500 global RAM bytes. The first build reached partition generation but failed because the temporary build directory lacked partitions.csv; copying the installed board's standard default.csv to that temporary file and rebuilding succeeded. No partition layout in the course repository or installed Core was changed. Upload on COM3 completed with Hash of data verified. No other board was flashed.

Open the same-name .ino in this folder. Serial: 115200 baud, 8N1. The sketch waits for commands; it does not scan repeatedly or run diagnostics on boot. Sending `d` runs one diagnostic. It calls Wire.end before repeating a test if Wire was previously started. All plain-GPIO phases use INPUT or INPUT_PULLUP, never OUTPUT. Each phase samples both inputs 100 times, five milliseconds apart, then dumps their actual GPIO configuration.

1. INPUT, no pulls.
2. Only GPIO21 weak internal pull-up.
3. Both weak internal pull-ups.
4. After Wire.begin(21,22,100000), 50 ms timeout.
5. After explicitly enabling the input path with gpio_input_enable, without pinMode/releasing the I2C routing.

No explicit bus-clear/recovery pulses or push-pull HIGH writes are implemented. Input configuration must not be changed with pinMode while using Wire, because that can detach the peripheral. Internal weak pull-ups are a diagnostic aid, not a qualified replacement for external I2C pull-ups. Digital levels are not voltage measurements.

`s` is a **manual**, gated single-scan command; it was **not sent in this experiment**. Do not send it until electrical/wiring checks are adequate and both inputs are stably HIGH. It scans 0x08–0x77 only when Wire is initialized and all 100 pre-scan samples on each line are HIGH. It prints each actual Wire result (0 ACK, 2 address NACK, other codes errors), and aborts if a line is LOW after a transaction. No recovery is attempted. Current software remains idle waiting for serial input after the completed tests; Wire is initialized but no periodic transactions occur.

## Real Result

Three full diagnostics completed. Across all five phases in all three runs, each input was LOW for 100/100 samples. Configuration dumps verify InputEn=1 and OutputEn=0 in plain-input phases; internal Pullup changes 0→1 as requested and Pulldown stays 0. Wire.begin returned 1, configured clock 100000, I2C open-drain routing and input enabled. Explicit gpio_input_enable returned ESP_OK for both pins and did not change the LOW readings. Thus the earlier scanner LOW result was reproduced independently of Wire initialization and was not fixed by enabling input or weak pull-ups. No address, ACK/NACK or physical fault identity was established.

[First raw diagnostic](../../evidence/task05-i2c-scanning/lcd-passive-gpio-diagnostic-20261102.txt) · [Two repeated diagnostics](../../evidence/task05-i2c-scanning/lcd-passive-gpio-repeat-20261102.txt) · [Hashes/parsed verification](../../evidence/task05-i2c-scanning/lcd-passive-gpio-verification-20261102.json).

## Next Isolation Step — Student Physical Action Required

Disconnect USB/all relevant power. Disconnect the signal leads from **ESP32 GPIO21 and GPIO22 only**, including an old GPIO22→GPIO23 jumper or any other branch attached to either pin if present. Keep the LCD 3V3/GND power wiring unchanged; do not dismantle unrelated circuits. This separates ESP32 inputs from external bus loads so the next weak-pull-up test can distinguish an external wiring/module load from an input/board problem. It has not yet been done and no result is predicted as measured.

## Source Verification

The installed Core's esp32-hal-gpio.c delegates digitalRead to gpio_get_level, warning about non-GPIO ownership; esp32-hal-i2c-ng.c configures I2C internal pull-ups. [Espressif GPIO documentation](https://docs.espressif.com/projects/esp-idf/en/v5.5.3/esp32/api-reference/peripherals/gpio.html) notes that input must be enabled for a meaningful gpio_get_level result. Actual configuration dumps here confirm that input was enabled, so that possible software false-LOW mechanism did not explain this test.
