# Module 01 — Hardware Experiments

**Mei Zhengyong · HSBI Sensors & Actuators**
[Portfolio home](../README.md) · [Hardware image index](../Hardware/pictures/README.md)

The reports below document real ESP32 demonstrations. Their limitations remain part of the evidence; no instructor grade or complete module pass is claimed. Task numbers follow the portfolio reports. Button work retains its original Task 1 source filename and also supports the workbook's basic-electronics goal. The workbook's separate serial/plot/timing task remains pending.

## Learning Goals

The rows follow the numbering and meaning of the [official module goals](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/01-foundations-measurement-and-sensor-chain.md).
Status describes the linked evidence, not an instructor assessment.

| Official goal (paraphrased) | How I learned or checked it | Existing evidence | Evidence status |
|---|---|---|---|
| 1. Explain sensors as transducers within a full signal chain. | I traced light through the LDR divider and ADC; the separate ToF chain is literature-based. | [LDR code](code/task04_ldr/task04_ldr.ino), [wiring](../Hardware/pictures/esp32-ldr-wiring.jpg), [chain draft](task06-chain.md) | Qualitative demonstration; Needs further evidence for the proposed ToF chain. |
| 2. Distinguish true value, error, uncertainty, accuracy, precision, resolution, range and sensitivity. | I compared ADC code ranges and kept API voltage estimates separate from independent reference measurements. | [ADC report](evidence/Task03_ADC.md), [vocabulary notes](study-log.md) | Partial; Needs further evidence for numerical uncertainty and reference accuracy in this module. |
| 3. Explain conditioning, ADC, sampling, quantization and coding. | I used a divider to condition resistance into voltage and compared 10-bit and 12-bit endpoints; timing theory is not a measured bandwidth test. | [ADC sketch](code/task03_adc/task03_adc.ino), [10-bit maximum](../Hardware/pictures/esp32-adc-10bit-max.png), [timing preparation](records/task02-serial.md) | Partial; Needs further evidence for sampling/aliasing and controlled timing. |
| 4. Safely drive a simple actuator and read a digital input with bounce mitigation. | I documented LED output and an INPUT_PULLUP button with a 30 ms stability rule; counter snapshots alone do not prove one count per physical press. | [LED report](evidence/Task01_LED_Blink.md), [button code](code/task01_button_debounce/task01_button_debounce.ino), [counter screenshot](../Hardware/pictures/esp32-button-debounce-presses39.png) | Demonstrated at basic level; Needs further evidence for current and controlled debounce timing. |
| 5. Use Arduino serial tools to inspect, log and visualize measurements. | I inspected serial screenshots for GPIO and ADC behaviour; a separate controlled plot/timing exercise is still prepared rather than measured. | [Serial record](evidence/serial-test.md), [LDR illuminated capture](../Hardware/pictures/esp32-ldr-illuminated.png), [serial/timing sketch](code/task02_serial_timing/task02_serial_timing.ino) | Inspection demonstrated; Needs further evidence for the planned timing/plot exercise. |
| 6. Measure ADC resolution, read an LDR and apply initial digital filters. | I observed endpoints and light response. The Module 1 filter sketch is prepared; later real filter measurements belong to Module 3, not this session. | [ADC record](records/task03-adc.md), [LDR report](evidence/Task04_LDR.md), [filter preparation](code/task05_filters/README.md), [later measured filters](../Module03/reports/task05-noise-filtering.md) | Partial; Needs further evidence for local voltage-step calibration and Module 1 filter execution. |

## Group Members

- Mei Zhengyong — GitHub: [@SuYLanM](https://github.com/SuYLanM)
- Felix — GitHub: [@Felix-Gaoxinyu](https://github.com/Felix-Gaoxinyu)

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
| Reflection | [Reflection 1](../Reflections/ref01.md) | Evidence-based draft completed; personal confirmation pending |
| Module 02 | [Module entry](../Module02/README.md) | See current evidence/status in Module02 |

## Running the Arduino Examples

Open one same-name sketch folder at a time. Target: classic ESP32 / **ESP32 Dev Module**, Espressif Core **3.3.12**; serial 115200 baud (COM3 was used in the supplied evidence). No third-party libraries are required. Confirm physical pins and components before rewiring. The source filenames are intentionally preserved.

[Code index](code/README.md) · [Validation record](validation.md) · [Repository audit](../docs/repository-audit.md). Blank tables mean unmeasured, never zero. Reports distinguish commanded GPIO states, ideal calculations, API estimates and independent measurements.


## Study log

### Evidence-led review and AI verification

I completed basic LED, button, potentiometer ADC and LDR demonstrations. The
ADC screenshots distinguish 0–1023 from 0–4095, and the LDR screenshots compare
ambient light, illumination and cover. The button counter snapshots support the
software debounce exercise, but I cannot turn them into an independently counted
press test. I learned to distinguish a commanded output, an ADC code and a
reference measurement. Codex helped prepare sketches, explanations and evidence
navigation; I checked those suggestions against the archived code and captures.
The Module 1 filters and timing comparisons remain prepared rather than tested.
Later Module 3 results are linked as progression, not retroactive Module 1 data.

My [individual reflection](../Reflections/ref01.md) is a retrospective draft for
my confirmation. Felix is listed in the existing group record; specific work
allocation, feedback, discussions and Discord sharing remain unconfirmed.
