# Preparation Validation

Date: 2026-10-21. These checks concern software preparation, not measured hardware performance.

## Requirement and API review

- Official Module 01 file reviewed at blob SHA `5747f8e74d932fce5aca23608006c2f51f80ddd6` on `main`; tasks below its draft boundary remain provisional.
- [Espressif ADC](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html) and [GPIO](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/gpio.html) API references reviewed, plus installed Core 3.3.12 ADC header/source.
- [ST VL53L0X datasheet](https://www.st.com/resource/en/datasheet/vl53l0x.pdf), DS11555 Rev 6, reviewed for the literature-based Task 6 diagram.

## Compilation

Target: `esp32:esp32:esp32` (ESP32 Dev Module), installed Espressif Core 3.3.12, using the CLI bundled with the local Arduino IDE.

All six sketches **compiled successfully** with CLI exit code 0. No upload command was used. Build products are outside the repository in a task-specific temporary working directory.

| Sketch | Compile | Program bytes | Global variable bytes |
|---|---|---|---|
| task01_led_blink | PASS | 271796 | 22124 |
| task01_button_debounce | PASS | 272096 | 22156 |
| task02_serial_timing | PASS | 272216 | 22164 |
| task03_adc | PASS | 278332 | 26316 |
| task04_ldr | PASS | 278420 | 22196 |
| task05_filters | PASS | 278292 | 22316 |

These are build statistics from the preparation session, not experimental readings. Default configurations were compiled then; the subsequently imported ADC report now documents real runs at both 10 and 12 bits. Plotting and delay/printing comparisons remain TODO. Compilation does not establish physical correctness, button bounce capture, ADC accuracy or filter latency.

## Documentation and preservation

- Six same-name sketch folders, each with an English README.
- Local Markdown file targets, sketch folder/name pairing and README presence checked: PASS. Markdown whitespace checks and `git diff --check`: PASS.
- Module 0 notes, root `sketch.ino` / `diagram.json`, eight original Wokwi images and tracked Reflection files checked against HEAD: contents unchanged (allowing only Git's line-ending representation).
- Existing `Hardware/pictures/esp32-serial-test.png` was visually inspected and left unchanged; it was already untracked before this work.

## Current Hardware Status (2026-10-26)

[Task 1 LED](evidence/Task01_LED_Blink.md), [Task 2 button](evidence/Task02_Button_Debounce.md), [Task 3 ADC](evidence/Task03_ADC.md) and [Task 4 LDR](evidence/Task04_LDR.md) provide real observations with their stated limits. The original button and LDR reports/images already published were preserved byte-for-byte in the repository audit.

Current source filenames and contents are preserved. Task 5 and the separate serial/plot/timing comparisons remain untested on hardware. Independent current/voltage checks, controlled button counting, ADC full sweeps and LDR repeatability remain TODO. No new hardware experiment or generated evidence image was produced during organization.

The initial serial screenshot was renamed to remove its duplicate suffix, with identical image bytes. [Repository audit](../docs/repository-audit.md) records publication scope and current link/preservation checks. The earlier preparation's six successful builds are listed above; the 2026-10-26 source compilation check is recorded in that audit separately.
