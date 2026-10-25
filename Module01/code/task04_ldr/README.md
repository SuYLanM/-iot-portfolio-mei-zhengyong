# Task 4 — LDR Divider and Approximate Resistance

Status: the [Task 4 report](../../evidence/Task04_LDR.md) documents real qualitative light-response observations. TODO: exact LDR identification, actual resistor/supply measurement, controlled repeatability and independent voltage checks; do not treat the existing results as calibrated metrology.

## Parts, GPIO and wiring

ESP32, USB cable, breadboard/jumpers, bare two-terminal LDR, nominal 10k fixed resistor (TODO measure), multimeter and phone flashlight. No LED/button needed. With power disconnected:

```text
3.3 V --- LDR --- measurement node --- fixed 10k --- GND
                         |
                       GPIO34
```

Use 3.3 V only and a common ground. This sketch assumes the LDR is the upper resistor; swapping components requires changing the resistance formula. An LDR module with comparator/potentiometer is not automatically equivalent to a bare LDR: inspect it first.

## Run

1. Confirm parts and wiring. Measure actual fixed resistance with power off; measure divider supply when ready. Replace nominal `FIXED_RESISTOR_OHM` and `SUPPLY_V`, and record the changes.
2. Open `task04_ldr.ino`, select ESP32 Dev Module / Core 3.3.12 / COM3; verify and upload after review. Serial Monitor: 115200, CSV default. Reset to capture header.
3. Cover, shade, expose to room light, then flashlight. Use at least dark/shade/bright and several repeated readings each. Record distances, settling time and illumination setup so conditions can be repeated; do not label estimated conditions as calibrated lux.
4. Set `PLOT_OUTPUT=true`, reupload for signal plots if useful. Retain original readings for Task 5.

## Output and calculation

CSV: `t_ms,raw,ideal_V,api_mV,R_est_ohm,R_valid`; plot mode: `raw`, `ideal_mV`, `api_mV`, `R_valid`. Nominal sample interval: 100 ms; real timing depends on execution.

`ideal_V` is the classroom full-scale estimate (see Task 3). `api_mV` is a second calibrated-API conversion, not an independent reference. Resistance uses that voltage and the divider equation `R_LDR=R_fixed*(V_supply-Vout)/Vout`.

The calculation is disabled outside a conservative 0.15–2.9 V band, at saturated raw codes, or when `Vout>=SUPPLY_V`. `R_est_ohm=-1` is an invalid marker, never a measured negative resistance. `R_valid=1` only permits the calculation; it does not certify accuracy. The estimate assumes known resistance/supply, negligible ADC loading and sufficiently accurate voltage conversion. For this divider orientation, Vout rises as light increases.

Complete [LDR record](../../records/task04-ldr.md): repeated raw/voltage/resistance entries, range, repeatability, one worked calculation and assumptions. Discuss slow response, nonlinearity, temperature dependence and endpoint limitations. The exact LDR model is TODO; consult its actual datasheet before stating a numerical response time.
