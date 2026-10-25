# Task 1 — Remaining Electronics Record

Status: **partially documented hardware experiment**. The [LED hardware report](../evidence/Task01_LED_Blink.md) is the source for the actual setup, resistor identification, upload, serial state timing and illuminated-LED photograph. Those results are not duplicated here. The companion [button report](../evidence/Task02_Button_Debounce.md) now documents core debounce functionality; controlled comparisons and the remaining electrical checks are still incomplete. Empty cells below are TODOs, never zeros.

| Missing metadata / independent measurement | Actual observation — TODO |
|---|---|
| Test date / operator / partner | |
| Independent confirmation of GPIO18/GND continuity; GPIO27 for the documented button experiment | |
| Breadboard row/rail connectivity check | |
| Actual green LED part / Vf source and polarity check | |
| Meter-confirmed resistor value / tolerance (colour-band identification is in the report) | |
| Supply voltage and assumed/measured Vf | |
| R=(Vs−Vf)/I calculation for the actual LED; measured current if tested | |
| Button switched leg pairs / continuity check | |
| Repeat with verified 220–330 Ω resistor, if available; evidence | |
| Full optical blink cycle / independently measured timing, if tested | |

## Button companion evidence

Observed counters are retained only in the [Task 2 report](../evidence/Task02_Button_Debounce.md). Use the [button follow-up record](task02-button.md) for controlled manual press counts and before/after comparisons; do not fill duplicate tables with inferred measurements.

TODO: explain both pull-up and pull-down using the README diagrams; why an input must not float; why an external 10k may help with noisy long wires. If using the optional capacitor, confirm its value and record actual comparison. If skipping Task 1, record prior experience and actual instructor quick-check evidence; no exemption has been claimed.
