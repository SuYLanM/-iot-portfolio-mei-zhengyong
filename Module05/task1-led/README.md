# Task 1 — PWM LED Fade

## Objective and wiring

Demonstrate PWM dimming of one external LED on the MH-ET LIVE ESP32 MiniKit.

| From | Connection | To |
|---|---|---|
| GPIO13 | Nominal 220 ohm series resistor | LED anode |
| LED cathode | Direct wire | GND |

The configured pin is **13**, as defined by `LED_PIN` in
[src/main.cpp](src/main.cpp). An old comment mentions GPIO16; it does not change
the executable pin selection. This report preserves the original source.

## Implementation and operation

[platformio.ini](platformio.ini) selects `mhetesp32minikit`, the Arduino framework,
and 115200 baud. `analogWrite()` steps duty from 0 to 255 and back, with 8 ms per
step. No third-party library or serial control command is required. Duty controls
the on-time fraction; it is not an analog output voltage or measured brightness.

```sh
cd Module05/task1-led
pio run
pio run -t compiledb
# Upload only when intentionally replacing the board's current firmware:
pio run -t upload
pio device monitor -b 115200
```

## Results and remaining evidence

The existing [module record](../README.md#verified-serial-output--task-1)
contains the GPIO13 startup serial excerpt and reports a prior build and upload.
No new hardware experiment or compilation was performed for the Task 4 audit.
The dedicated fade photograph/video is pending. LED current is **Not measured**;
it cannot be inferred from a startup banner. When available, voltage across
the nominal 220 ohm resistor can support an estimate using `I=UR/R`, with the
PWM setting and measurement method recorded.

See [development rules](AGENTS.md) and the shared [Study log](../README.md#study-log).
