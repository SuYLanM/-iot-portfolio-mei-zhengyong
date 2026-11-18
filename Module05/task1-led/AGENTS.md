# Task 1 — single LED

Follow [Module05 rules](../AGENTS.md). This project uses
`board=mhetesp32minikit`, Arduino, **GPIO13**, one external LED, and a nominal
220 ohm series resistor. GPIO16 in the old source comment is not the configured
pin: `LED_PIN=13` controls the output. Preserve source for this documentation
audit. Serial is 115200 baud; no REPL or third-party LED library is used.
PWM rises and falls over 0..255 with an 8 ms delay per step.
Current is **Not measured** and the dedicated fade photo/video is pending.
The module `.gitignore` excludes build products for this project.
