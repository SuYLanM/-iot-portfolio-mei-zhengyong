# Task 3 — Potentiometer ADC Precision Dance

Status: [imported ADC hardware report](../../evidence/Task03_ADC.md) documents the B10K potentiometer setup and real 10/12-bit endpoint comparisons. **TODO: complete sweep/plot, intermediate stationary readings, independently measured voltage steps and multimeter comparison.** Do not repeat endpoint results in a second record; use the [remaining-work record](../../records/task03-adc.md).

## Parts, GPIO and wiring

Classic ESP32, USB cable, breadboard/jumpers, nominal B10K potentiometer as recorded in the report (TODO meter-confirm actual resistance and continuity), multimeter. No LED or button needed. Power off: connect outer terminals to 3.3 V and GND, wiper to **GPIO34 / ADC1**. Confirm pinout; never apply 5 V. GPIO34 is input-only and is not an LED output. Stay within the board supply limits.

## Run

1. Open `task03_adc.ino`; ESP32 Dev Module / Core 3.3.12 / COM3, 115200 baud. Start with `ADC_BITS=12`, `PLOT_OUTPUT=false`.
2. After inspected wiring, verify/upload, open Serial Monitor and reset to get the CSV header. Assign one pod member to measure and the other to record (TODO real names).
3. Sweep slowly through the full range. Save all rows. The `distinct_observed` counter counts sampled unique codes since reset, **not** the ADC's theoretical total. A finite manual sweep at nominal 20 Hz will miss codes; repeat/slow down and disclose limitations.
4. At several stationary positions, measure wiper-to-GND using the multimeter and record both API and ideal estimates. Record DMM range, accuracy/resolution and uncertainty. Investigate saturation/erratic endpoints without exceeding 3.3 V.
5. Repeat after changing `ADC_BITS=10`, uploading and resetting. Use matching conditions. Set `PLOT_OUTPUT=true` for a separate visual sweep; reupload and capture a plot.

## Output and interpretation

CSV: `t_ms,bits,raw,ideal_V,api_mV,min_raw,max_raw,distinct_observed`. Plot mode outputs named `raw`, `ideal_mV`, `api_mV` traces.

The classroom model uses `ideal_V = raw * 3.3 / 2^bits`, ideal step `3.3 / 2^bits`: approximately 0.806 mV (12 bit) and 3.223 mV (10 bit). **These are calculations, not measurements.** `3.3` is an assumed input full scale, not the chip's internal ADC reference. `analogSetPinAttenuation(..., ADC_11db)` fixes attenuation; the classic ESP32's documented measurement range at this setting is roughly 0.15–3.1 V. Real endpoints and gain differ from the ideal model.

`analogReadMilliVolts()` returns an API-calibrated estimate from a **second conversion**; it is not a DMM reading, nor calibration of the preceding raw sample. Hold input steady and compare repeats. Calibration quality and attenuation matter. Do not infer physical accuracy from bit count or a smooth plot.

Fill [ADC record](../../records/task03-adc.md): observed range, distinct codes, measured adjacent-code voltage separation where resolvable, theoretical LSB, 10/12-bit comparison, DMM error and endpoint observations. If the DMM cannot resolve a step, record that limitation instead of inventing it. [Espressif ADC API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html).
