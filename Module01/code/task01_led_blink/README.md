# Task 1 — External LED Blink

Status: [imported hardware report](../../evidence/Task01_LED_Blink.md) documents real upload, 500 ms commanded state changes and a green LED illuminated in a photograph. The report's resistor is identified from colour bands as 10 kΩ; this is distinct from the proposed 220–330 Ω circuit below. **TODO: meter-confirmed resistance/current, independent pin checks and full optical blink timing.** The full Task 1 button/debounce work remains TODO.

## Parts and GPIO

- Classic ESP32 Dev Module and USB data cable; breadboard; jumper wires.
- One plain LED; the imported test used green. TODO: confirm actual part, polarity and forward-voltage specification for a repeat test.
- One **220–330 ohm series resistor** for the proposed repeat, chosen after calculation. TODO: identify/verify it with power disconnected; do not relabel the report's 10k resistor as 220–330 ohm.
- GPIO18 is the proposed external LED output; no built-in LED assumption.

## Wiring — review hardware before connecting

Disconnect USB power first. Confirm the board's GPIO18 and GND labels and breadboard connections. Use:

```text
GPIO18 --- confirmed 220–330 ohm resistor --- LED anode (+)
                                               LED cathode (-) --- GND
```

The longer lead is usually the anode; the flat rim usually marks the cathode. Check the actual component. A resistor can be placed on either side of the LED, but must be in series. Do not use 5 V or connect the LED without its resistor.

Calculation: `R = (3.3 V - Vf) / I`. **Illustration only, not a measurement:** assuming a red LED with `Vf=2.0 V`, 220 ohm gives about 5.9 mA and 330 ohm about 3.9 mA. A blue/white LED needs a new calculation. The instructor's approximate target is 5–10 mA; a smaller current may be sufficient visibly. Confirm actual parts before choosing.

## Run after hardware review

1. Open `task01_led_blink.ino` in this folder in Arduino IDE 2.3.10.
2. Select **ESP32 Dev Module**, Espressif Core **3.3.12**, and **COM3** (recheck if reconnected).
3. Verify/compile. Once the inspected circuit is ready, reconnect USB and upload.
4. Open Serial Monitor at **115200 baud**. Expected behaviour: external LED alternates every 500 ms. The report documents the commanded state timing and illumination at one instant; a still photograph does not establish the full optical cycle.

## Output and records

Named fields `t_ms` and `led` report the time and commanded GPIO state. Serial output proves the command, not LED current or illumination.

Record actual resistor value/tolerance, LED colour and assumed/measured `Vf`, calculated current, pin labels, wiring photo, upload result and observed LED behaviour in [Task 1 record](../../records/task01-electronics.md). Do not tick PASS until verified. Source: [official Task 1](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/01-foundations-measurement-and-sensor-chain.md).
