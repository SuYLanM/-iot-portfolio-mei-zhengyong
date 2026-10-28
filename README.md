# HSBI Sensors & Actuators — Learning Portfolio

**Student:** Mei Zhengyong
**Course:** Sensors and Actuators / Sensorik und Aktorik, HSBI, WS 2026/27

This portfolio documents preparation, ESP32 experiments, Arduino source code and learning evidence. Reports separate observations from calculations and identify measurements still needed. Hardware demonstrations are not claims of instructor grading or complete fulfillment of every course requirement.

## Group Members

- Mei Zhengyong — GitHub: [@SuYLanM](https://github.com/SuYLanM)
- Felix — GitHub: [@Felix-Gaoxinyu](https://github.com/Felix-Gaoxinyu)

## Module 01 Hardware Experiments

[Open the Module 01 experiment guide](Module01/README.md) for wiring, serial screenshots, code instructions and follow-up records.

| Task | Evidence status | Report / plan | Arduino code | Hardware evidence |
|---|---|---|---|---|
| 1 — LED Blinking | Core GPIO/LED demonstration documented | [Report](Module01/evidence/Task01_LED_Blink.md) | [Sketch](Module01/code/task01_led_blink/task01_led_blink.ino) | [Photo](Hardware/pictures/esp32-led-hardware.jpg) · [all evidence](Module01/evidence/Task01_LED_Blink.md#observations-and-evidence) |
| 2 — Button Debouncing | Core button/debounce demonstration documented | [Report](Module01/evidence/Task02_Button_Debounce.md) | [Sketch](Module01/code/task01_button_debounce/task01_button_debounce.ino) | [Photo](Hardware/pictures/esp32-button-wiring.jpg) · [all evidence](Module01/evidence/Task02_Button_Debounce.md#5-recorded-real-hardware-evidence) |
| 3 — Potentiometer ADC | Real 10-bit / 12-bit endpoints documented | [Report](Module01/evidence/Task03_ADC.md) | [Sketch](Module01/code/task03_adc/task03_adc.ino) | [Photo](Hardware/pictures/esp32-adc-wiring.jpg) · [all evidence](Module01/evidence/Task03_ADC.md#actual-wiring) |
| 4 — LDR Light Sensor | Qualitative hardware light response documented | [Report](Module01/evidence/Task04_LDR.md) | [Sketch](Module01/code/task04_ldr/task04_ldr.ino) | [Photo](Hardware/pictures/esp32-ldr-wiring.jpg) · [all evidence](Module01/evidence/Task04_LDR.md#original-evidence) |
| 5 — Digital Filtering | **Pending / Not hardware tested** | [Experiment plan](Module01/records/task05-filters.md) | [Sketch](Module01/code/task05_filters/task05_filters.ino) | No hardware evidence yet |

Task labels follow this portfolio's experiment reports. The button sketch retains its original `task01_button_debounce.ino` name. In the course workbook, button/debounce work is part of Task 1; the separate serial plotting and delay/millis comparison remains [pending](Module01/records/task02-serial.md). Task 5 code is preparation only.

## Module 02 — Buses and I2C Preparation

[Module 02 guide and Task 1–10 checklist](Module02/README.md) covers pin/divider theory, bus research, I2C study, scanner/register/multi-device code and scope feasibility. **Prepared / not hardware tested**: no discovered addresses, sensor values, instrument captures, pod assignment or class decision are claimed. Task 10 is optional.

## Pre-study and Preparation

- [Module 0 pre-study](00-pre-study.md): original notes, datasheet study, project idea and Wokwi evidence; retained as submitted.
- [Original Wokwi project](https://wokwi.com/projects/476869641648482305), [pre-study sketch](sketch.ino), [circuit file](diagram.json). Root-level Wokwi images are simulation evidence, distinct from real hardware photographs.
- [Personal profile and Task Pod template](Module01/who-am-i.md): TODO personal answers and actual pod agreement.
- [Task A / Task B study log](Module01/study-log.md): theory draft; actual student/pod verification and discussion remain TODO.
- [Task 6 signal-chain draft](Module01/task06-chain.md): literature-based diagram, not a tested VL53L0X build.
- [Module 02](Module02/README.md): theory and Arduino preparation available; all hardware tests, bus assignment and classroom decisions remain Pending.

## Learning Reflections

[Reflection instructions](Reflections/README.md) · [Reflection 1](Reflections/ref01.md). Reflection 1 currently contains prompts, not a completed personal reflection. Later reflections have not been supplied.

## Repository Structure

```text
README.md                  Portfolio overview
00-pre-study.md             Original Module 0 preparation
Module01/
  README.md                 Detailed experiment navigation
  evidence/                 English reports and evidence references
  code/                     Separate Arduino sketch folders and instructions
  records/                  Follow-up plans and blank measurement tables
Hardware/pictures/          Original hardware photographs and screenshots
Module02/                   Bus/I2C theory and code; hardware tests pending
Reflections/                Reflection instructions and original prompts
sketch.ino, diagram.json    Original Wokwi preparation source
wokwi_*.png                 Original simulation screenshots
docs/                      Audit and preserved course-template text
```

[Hardware evidence index](Hardware/pictures/README.md) · [Software validation](Module01/validation.md) · [Repository audit](docs/repository-audit.md)

Documentation preparation used AI assistance; the real evidence is identified in each report. See the [official course module](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/01-foundations-measurement-and-sensor-chain.md) for learning goals.
