# Module 02 — Arduino Preparation

Existing ESP32 sketches compile for classic ESP32 Dev Module with Core 3.3.12; the separate WeMos PWM sketch compiles for **ESP8266 / d1_mini / Core 3.1.2**. The external analyzer has a [verified static CH0 LOW/HIGH stage](../evidence/task09-logic-analyzer/README.md); the later [ESP8266 PWM source and capture](../evidence/task09-esp8266-pwm.md) are also verified. The ESP32 PWM/I2C target sketches and other tasks retain their existing status. The earlier generic sensor scaffolds remain disabled until actual hardware configuration and adapter implementation; the current Task 7 LCD/MCU demonstrator below instead has functional guarded code. Tasks 5–7 were updated in place for both ESP32 and WeMos ESP8266; [eight compile checks](../validation.md#follow-up-preparation-checks--2026-11-02) passed. Those earlier eight checks were preparation only; the later Task 6 diagnostic below records an actual blocked-bus run. Open each same-name .ino folder separately; the earlier generic scaffolds selected no third-party sensor library. The later [Task 6 BME280 example](task06_library_vs_register/README.md) now has a concrete library and a successful build, while the [current-hardware diagnostic](task06_current_hardware_diagnostic/README.md) has three real blocked-bus rounds. These do not establish sensor readings.

| Task | Sketch and English instructions | Default behavior |
|---|---|---|
| 1 | [UART loopback](task01_uart_loopback/README.md) | UART2 RX16/TX17, pending physical jumper test |
| 1 | [Fixed-resistor voltage divider](task01_voltage_divider/README.md) | GPIO35, 32 raw + 32 API-mV samples; Prepared / Awaiting hardware validation |
| 2 Pod 4 | [CAN A — local self-test](task02_can_self_test/README.md) | 500 kbit/s, same unconnected GPIO5 TX/RX feedback; no physical CAN validation |
| 2 Pod 4 | [CAN B — external communication](task02_can_external/README.md) | 500 kbit/s, GPIO5 TX / GPIO4 RX, SN65HVD230 and active peer required; hardware Pending |
| 5 | [I2C scanner](task05_i2c_scanner/README.md) | ESP32 SDA21/SCL22 or ESP8266 D2/D1; requested 100 kHz; ACK/NACK/errors; sensor test Pending |
| 6 | [Current-hardware diagnostic](task06_current_hardware_diagnostic/README.md) | Compile/upload PASS; three real BUS_BLOCKED rounds; no probes |
| 6 | [BME280 library versus direct reading](task06_library_vs_register/README.md) | Compile PASS; extra sensor required, not uploaded/hardware verified |
| 6 | [Library adapter](task06_library_template/README.md) | PENDING_CONFIG; stubs do not invent readings |
| 6 | [Direct register adapter](task06_register_template/README.md) | PENDING_CONFIG; no guessed address/register |
| 7 | [ESP32 master / Wire1 test target](task07_two_devices/README.md) | Fresh compile/upload PASS; three real 0x27/0x42 dual-address/data passes; MCU emulator, not two sensors |
| 7 | [ESP8266 built-in Wire slave candidate](task07_esp8266_slave/README.md) | Core 3.1.2 compile PASS; not uploaded, physical testing Pending |
| 7 | [Simulation only](task07_simulation/README.md) | Eight real host-model tests PASS; synthetic address responses, not hardware evidence |
| 8 | [Mux selection template](task08_mux_template/README.md) | PENDING_CONFIG; actual TCA9548A/straps needed |
| 9 | [PWM reference](task09_pwm_reference/README.md) | ESP32 GPIO25 alternative; not used in the actual WeMos experiment |
| 9 | [ESP8266 WeMos PWM generator](task09_esp8266_pwm_generator/README.md) | D1/GPIO5, 1000 Hz requested, approximately 50%; COM4 upload and real capture verified |
| 9 | [I2C capture source](task09_i2c_capture_source/README.md) | Separate target: SDA21/SCL22, 100 kHz, ACK/NACK logging; Task 5 source unchanged |
| 9 | [ESP8266 WeMos I2C source](task09_esp8266_i2c_source/README.md) | Compile PASS; D2/GPIO4 SDA, D1/GPIO5 SCL, 100 kHz requested; initially paused; COM4 upload and NACK-only capture verified |

Task 9 uses a **separate analyzer board**. Actual static and WeMos PWM captures used its unchanged official SUMP firmware; the [Core 3 retune patch](../patches/task09-core3-clock-retune.patch) remains prepared and was not flashed for those results. Do not combine firmware. [Chinese operation manual](../task09-logic-analyzer-guide-zh.md) · [Actual software build checks](../task09-analyzer-build.md). The [ESP8266 PWM capture](../evidence/task09-esp8266-pwm.md) is now documented; [I2C NACK-only capture](../evidence/task09-esp8266-i2c.md) is verified; sensor ACK/data and independently calibrated timing remain Pending.

[Chinese wiring guide](../wiring-zh.md) · [Validation](../validation.md). Final pins must follow actual board/module documentation. Source compilation does not satisfy Tasks 5–9 hardware evidence.

## Separate Hardware Extension

[DS1302 + LCD1602](ds1302_lcd_extension/README.md): COM3 upload verified, real read-only RTC samples advance by one second, LCD 0x27 initializes and DDRAM readback matches. Not an I2C sensor comparison and not a replacement for Task 6.
