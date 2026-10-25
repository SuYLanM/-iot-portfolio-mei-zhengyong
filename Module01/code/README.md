# Arduino Source Index

Open each `.ino` from its own same-name folder in Arduino IDE. Board: ESP32 Dev Module, Core 3.3.12. Keep all sketch names unchanged. A successful compile is not a hardware measurement.

| Portfolio experiment | Source | Instructions | Hardware evidence |
|---|---|---|---|
| Task 1: LED Blinking | [task01_led_blink.ino](task01_led_blink/task01_led_blink.ino) | [README](task01_led_blink/README.md) | [Report](../evidence/Task01_LED_Blink.md) |
| Task 2: Button Debouncing | [task01_button_debounce.ino](task01_button_debounce/task01_button_debounce.ino) | [README](task01_button_debounce/README.md) | [Report](../evidence/Task02_Button_Debounce.md) |
| Task 3: Potentiometer ADC | [task03_adc.ino](task03_adc/task03_adc.ino) | [README](task03_adc/README.md) | [Report](../evidence/Task03_ADC.md) |
| Task 4: LDR Light Sensor | [task04_ldr.ino](task04_ldr/task04_ldr.ino) | [README](task04_ldr/README.md) | [Report](../evidence/Task04_LDR.md) |
| Serial/timing follow-up | [task02_serial_timing.ino](task02_serial_timing/task02_serial_timing.ino) | [README](task02_serial_timing/README.md) | Pending plot/timing experiment |
| Task 5 filters | [task05_filters.ino](task05_filters/task05_filters.ino) | [README](task05_filters/README.md) | Pending / not hardware tested |

`task01_button_debounce.ino` implements the experiment reported as portfolio Task 2. Numbering reflects the original preparation, not a missing or renamed source. ADC defaults to 12 bits; its documented 10-bit run requires changing the configuration and reuploading.
