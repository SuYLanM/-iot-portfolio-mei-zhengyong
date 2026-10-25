# Task 2 — Button Follow-up Record

Core hardware functionality is documented in the [original report](../evidence/Task02_Button_Debounce.md). Keep its 93/39/19 and 171/79/39 snapshots unchanged; they do not establish the number of physical presses.

## Controlled Repeatability — TODO

Reset with the button released, record initial counters, manually count a known number of slow press/release cycles, and record final counters. Repeat without inventing a missing event history. Both edge counters include press and release; `presses` counts accepted LOW transitions.

| Date / run | Debounce ms | Physical presses manually counted | Initial raw / debounced / presses | Final raw / debounced / presses | Misses / extra accepted counts | Evidence |
|---|---|---|---|---|---|---|
| | | | | | | |
| | | | | | | |
| | | | | | | |

TODO: actual button identification and pin continuity, series resistor/current, independently measured debounce latency, and optional comparable runs with debounce disabled or RC filtering. GPIO27 internal pull-up and 30 ms stable-duration settings are established by the source configuration, not by an oscilloscope timing measurement.

The separate workbook [serial plotting / delay versus millis experiment](task02-serial.md) remains pending.
