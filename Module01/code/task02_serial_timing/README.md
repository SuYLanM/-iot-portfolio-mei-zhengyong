# Task 2 — Serial Plotter and delay()/millis() Comparison

Status: existing basic serial communication passed; **TODO: this sketch's upload, plot and timing measurements**.

## Parts, GPIO and wiring

ESP32, USB data cable, breadboard, jumpers, inspected external LED and confirmed series resistor on GPIO18, confirmed button GPIO27 → GND with internal pull-up. Use the Task 1 wiring instructions. No potentiometer is needed. TODO: confirm resistor and button before wiring; disconnect power to rewire.

## Run

1. Open `task02_serial_timing.ino`; ESP32 Dev Module / Core 3.3.12 / COM3; verify then upload after circuit review.
2. Open Serial Plotter at 115200. Named traces use `name:value`, without a space after the colon. Hide timing/count traces to see the 0/1 button and LED clearly; save the required signal plot.
3. Run all four combinations of `BLOCKING_DELAY` (false/true) and `PRINT_OUTPUT` (false/true), recompile/upload/reset each time. Keep wiring and actions comparable. When printing is false, use Serial Monitor and send `r` after at least two seconds for a single snapshot of the last completed timing window. Discard the following window, which contains query/printing overhead.
4. Try short presses and longer holds. A 200 ms blocking delay can miss short events. The non-blocking loop keeps polling while waiting for the next LED/report update. This sketch counts raw transitions, not debounced presses.

## Output and timing meaning

`button`, `led`, `mean_loop_us`, `max_loop_us`, `raw_edges`, `blocking`. Timing is measured from loop entry to next loop entry with `micros()`; unsigned subtraction handles rollover. Means/maxima cover each approximately 200 ms reporting window. The first window contains startup effects; discard it. Printing overhead appears in later loop intervals, not only the print statement's window.

The LED toggles and plots update on a 200 ms schedule. The fast polling rate and the 5 Hz reporting rate are different. Approximate polling rate is `1e6 / mean_loop_us`, using the measured interval. Serial output may still block if its buffer fills; `millis()` scheduling does not make every operation non-blocking.

Record four timing combinations, raw-edge counts, observed responsiveness, tool and actual plot/log links in [Task 2 record](../../records/task02-serial.md). Reference [existing serial PASS evidence](../../evidence/serial-test.md); it does not complete this task's plot or timing comparison.
