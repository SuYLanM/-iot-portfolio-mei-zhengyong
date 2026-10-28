# Module 02 — Arduino Preparation

Every sketch compiles for classic ESP32 Dev Module with Core 3.3.12. No Module 02 hardware test has occurred. Sensor-dependent sketches deliberately remain disabled until actual hardware configuration and adapter implementation. Open each same-name .ino folder separately; no third-party sensor library has been selected.

| Task | Sketch and English instructions | Default behavior |
|---|---|---|
| 1 | [UART loopback](task01_uart_loopback/README.md) | UART2 RX16/TX17, pending physical jumper test |
| 1 | [Fixed-resistor voltage divider](task01_voltage_divider/README.md) | GPIO35, 32 raw + 32 API-mV samples; Prepared / Awaiting hardware validation |
| 5 | [I2C scanner](task05_i2c_scanner/README.md) | Repeating probes, SDA21/SCL22, 100 kHz |
| 6 | [Library adapter](task06_library_template/README.md) | PENDING_CONFIG; stubs do not invent readings |
| 6 | [Direct register adapter](task06_register_template/README.md) | PENDING_CONFIG; no guessed address/register |
| 7 | [Two-device framework](task07_two_devices/README.md) | PENDING_CONFIG; actual addresses/read adapters needed |
| 8 | [Mux selection template](task08_mux_template/README.md) | PENDING_CONFIG; actual TCA9548A/straps needed |
| 9 | [PWM reference](task09_pwm_reference/README.md) | GPIO25 configured 1 kHz, not measured timing |

[Chinese wiring guide](../wiring-zh.md) · [Validation](../validation.md). Final pins must follow actual board/module documentation. Source compilation does not satisfy Tasks 5–9 hardware evidence.
