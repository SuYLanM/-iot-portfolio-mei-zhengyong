# Module 01 — Hardware Experiments

**Mei Zhengyong · HSBI Sensors & Actuators**
[Portfolio home](../README.md) · [Hardware image index](../Hardware/pictures/README.md)

The reports below document real ESP32 demonstrations. Their limitations remain part of the evidence; no instructor grade or complete module pass is claimed. Task numbers follow the portfolio reports. Button work retains its original Task 1 source filename and also supports the workbook's basic-electronics goal. The workbook's separate serial/plot/timing task remains pending.

## Task 1 — LED Blinking

**Status:** Core GPIO/LED demonstration documented; remaining quantitative checks are TODO.

[English report](evidence/Task01_LED_Blink.md) · [Arduino source](code/task01_led_blink/task01_led_blink.ino) · [Run instructions](code/task01_led_blink/README.md) · [Follow-up record](records/task01-electronics.md)

Hardware evidence: [Wiring / illuminated LED](../Hardware/pictures/esp32-led-hardware.jpg) · [500 ms commanded-state serial output](../Hardware/pictures/esp32-led-serial-output.png)

GPIO18 commands alternate every 500 ms; a green LED is illuminated in the photograph. The report identifies a 10 kΩ resistor by colour bands, not a meter measurement. TODO: verify resistor/current, actual LED forward voltage and complete optical blink timing.

## Task 2 — Button Debouncing

**Status:** Core button/debounce demonstration documented; remaining quantitative checks are TODO.

[English report](evidence/Task02_Button_Debounce.md) · [Arduino source](code/task01_button_debounce/task01_button_debounce.ino) · [Run instructions](code/task01_button_debounce/README.md) · [Follow-up record](records/task02-button.md)

Hardware evidence: [Wiring](../Hardware/pictures/esp32-button-wiring.jpg) · [Snapshot A: 93 / 39 / 19](../Hardware/pictures/esp32-button-released-presses19.png) · [Snapshot B: 171 / 79 / 39](../Hardware/pictures/esp32-button-debounce-presses39.png)

GPIO27 uses INPUT_PULLUP; GPIO18 drives the external LED. The unchanged sketch accepts a new state after 30 ms of stability. Screenshots preserve raw/debounced/press counters 93/39/19 and 171/79/39. TODO: controlled manual press counting, electrical checks and measured debounce timing. No strict one-physical-press/one-count result is claimed.

## Task 3 — Potentiometer ADC

**Status:** Real 10-bit / 12-bit endpoints documented; remaining quantitative checks are TODO.

[English report](evidence/Task03_ADC.md) · [Arduino source](code/task03_adc/task03_adc.ino) · [Run instructions](code/task03_adc/README.md) · [Follow-up record](records/task03-adc.md)

Hardware evidence: [Wiring](../Hardware/pictures/esp32-adc-wiring.jpg) · [10-bit minimum](../Hardware/pictures/esp32-adc-10bit-min.png) · [10-bit maximum](../Hardware/pictures/esp32-adc-10bit-max.png) · [12-bit minimum](../Hardware/pictures/esp32-adc-12bit-min.png) · [12-bit maximum](../Hardware/pictures/esp32-adc-12bit-max.png)

Endpoint screenshots document 0–1023 at 10 bits and 0–4095 at 12 bits. Ideal model voltage and API millivolts are not independent DMM measurements. The sketch defaults to 12 bits; repeat with ADC_BITS=10 as described in its instructions. TODO: complete sweep/plot, intermediate positions, independently resolvable voltage steps and DMM checks.

## Task 4 — LDR Light Sensor

**Status:** Qualitative hardware light response documented; remaining quantitative checks are TODO.

[English report](evidence/Task04_LDR.md) · [Arduino source](code/task04_ldr/task04_ldr.ino) · [Run instructions](code/task04_ldr/README.md) · [Follow-up record](records/task04-ldr.md)

Hardware evidence: [Wiring](../Hardware/pictures/esp32-ldr-wiring.jpg) · [Ambient](../Hardware/pictures/esp32-ldr-ambient.png) · [Brighter](../Hardware/pictures/esp32-ldr-illuminated.png) · [Covered](../Hardware/pictures/esp32-ldr-covered.png)

The real ambient/brighter/covered screenshots support qualitative response. Nominal 3.3 V and 10 kΩ assumptions produce resistance estimates; -1 is an invalid marker, not a measured negative resistance. TODO: actual supply/resistor measurements, controlled repeatability, uncertainty and settling time.

## Task 5 — Digital Filtering

**Pending / Not hardware tested.** [Arduino source](code/task05_filters/task05_filters.ino) · [Instructions](code/task05_filters/README.md) · [Blank experiment record](records/task05-filters.md). No hardware report or filter performance result is claimed. Use one real LDR/potentiometer stream for all filters, compare MA5/10/20, median and validity gating, and record actual delay. Keep optional synthetic spikes clearly separate.

## Serial Tools and Timing — Pending Follow-up

[Basic serial test](evidence/serial-test.md) documents ESP32 Dev Module, COM3 and 115200 baud. The [serial/timing sketch](code/task02_serial_timing/task02_serial_timing.ino), [instructions](code/task02_serial_timing/README.md) and [blank timing record](records/task02-serial.md) are prepared. A signal plot and controlled delay/millis/printing-overhead comparisons remain TODO.

## Pre-study and Preparation

| Item | Entry | Status |
|---|---|---|
| Module 0 | [Original pre-study](../00-pre-study.md) | Existing preparation retained; simulation is labelled separately |
| Task 0 | [Profile / Task Pod](who-am-i.md) | TODO personal answers and real agreement |
| Task A | [Theory study log](study-log.md) | AI-assisted draft; student review and verification pending |
| Task B | [Pod discussion section](study-log.md#agreed-learning-pod-discussion) | TODO actual discussion, agreed points and sharing |
| Task 6 | [VL53L0X signal chain](task06-chain.md) | Literature-based draft; no sensor build claimed |
| Task 7 / stretch tasks | [ADC follow-up](records/task03-adc.md) | Optional; not demonstrated |
| Reflection | [Original Reflection 1](../Reflections/ref01.md) | Prompts only; personal reflection pending |
| Module 02 | [Module entry](../Module02/README.md) | Pending |

## Running the Arduino Examples

Open one same-name sketch folder at a time. Target: classic ESP32 / **ESP32 Dev Module**, Espressif Core **3.3.12**; serial 115200 baud (COM3 was used in the supplied evidence). No third-party libraries are required. Confirm physical pins and components before rewiring. The source filenames are intentionally preserved.

[Code index](code/README.md) · [Validation record](validation.md) · [Repository audit](../docs/repository-audit.md). Blank tables mean unmeasured, never zero. Reports distinguish commanded GPIO states, ideal calculations, API estimates and independent measurements.
