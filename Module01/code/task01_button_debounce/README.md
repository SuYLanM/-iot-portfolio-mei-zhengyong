# Task 1 — Button, Pull-up/Pull-down and Debouncing

Status: the [Task 2 hardware report](../../evidence/Task02_Button_Debounce.md) documents the core internal-pull-up button/debounce demonstration. TODO: exact button/contact identification, actual resistor/current, controlled one-count-per-physical-press comparison and independent timing. The original sketch filename is retained.

## Parts, GPIO and wiring

Use the inspected [LED circuit](../task01_led_blink/README.md) on GPIO18, one momentary button, breadboard and jumpers. GPIO27 is the button input. A four-leg tactile button usually has two internally connected leg pairs: with power off, identify pairs using continuity and connect across the switched pairs. TODO: confirm the actual button.

Default (internal pull-up, no external button resistor required):

```text
3.3 V --- internal pull-up --- GPIO27 --- button --- GND
```

Released HIGH, pressed LOW. `PULL_UP=true`, `EXTERNAL_RESISTOR=false`.

External pull-up comparison: `3.3 V --- 10k --- GPIO27 --- button --- GND`; set `EXTERNAL_RESISTOR=true`. External pull-down comparison:

```text
3.3 V --- button --- GPIO27 --- 10k --- GND
```

Set `PULL_UP=false`, `EXTERNAL_RESISTOR=true`. Released LOW, pressed HIGH. TODO: confirm the actual 10k resistor before either external variant. An unconnected input floats and can report noise. A 10k external pull-up is stronger than the typical internal pull-up and may improve long-wire noise immunity.

## Run

1. Inspect parts, disconnect power, wire the default configuration; reconnect only after review.
2. Open `task01_button_debounce.ino`; select ESP32 Dev Module / Core 3.3.12 / COM3; verify and upload when ready.
3. Use Serial Monitor/Plotter at 115200. Press and release slowly several times. Reset with button released for each comparison.
4. The LED toggles on an accepted press; acceptance requires 30 ms of stable input. Change `DEBOUNCE_MS` within 20–50 ms and compare.

## Output and experiment records

`raw`, `stable`, `led`, `raw_edges`, `debounced_edges`, `presses`. Both edge counts include press **and release**; `presses` counts accepted presses only. Polling and serial printing can miss very fast bounce: a low raw count is not proof that contacts never bounce. Document what actually appears; use an analyzer later if needed.

Record manual press count, both edge counts, debounce setting and missed/double actions in the [controlled button follow-up record](../../records/task02-button.md). Draw both pull configurations and explain floating inputs. Optional hardware comparison: with a confirmed external 10k pull-up, add a confirmed 100 nF capacitor from GPIO27 to GND (`RC≈1 ms`); record before/after. A Schmitt-trigger circuit needs separate reviewed wiring and is not implemented here. The published report contains observed counters; none of these optional comparisons is claimed complete.
