# Task 9 A — ESP8266 I2C Capture Record

**Software Prepared / Hardware capture Pending.** [Source and wiring](../code/task09_esp8266_i2c_source/README.md) · [Task 9 report](../scope-decision.md).

## Planned setup — not measured results

Blue WeMos D1 mini ESP8266 generates I2C at requested 100 kHz. D2/GPIO4 SDA connects to black ESP32 GPIO21/CH0; D1/GPIO5 SCL to black GPIO19/CH1. Dedicated common ground and black GPIO22→GPIO23 clock jumper are required. The black analyzer retains its working official v0.31 firmware, COM3 / 921600. Recheck blue COM port and suitable 3.3 V pull-ups before upload. No sensor model or device address is assumed.

ESP8266 Core 3.1.2 compilation passed; upload and real scanning have not yet occurred. With no responding sensor, a verified NACK-only capture can demonstrate address transmission but not sensor ACK or data acquisition. The requested 1 MHz analyzer rate remains uncalibrated.

## Actual setup and observations — fill after experiment

| Item | Actual observation |
|---|---|
| Date / operator | Pending |
| Blue board port / upload result | Pending |
| Wiring photograph / common ground | Pending |
| SDA/SCL pull-up values / supply | Pending |
| Sensor model / datasheet / or explicitly no sensor | Pending |
| Source serial ACK / NACK / error totals | Pending |
| Analyzer requested rate / sample count / trigger | Pending |
| Actual sample-derived SCL timing | Pending |
| Decoder SDA / SCL assignments | Pending |
| START / 7-bit address / R/W | Pending |
| Ninth-clock ACK or NACK / STOP | Pending |
| Original .sr / hash / integrity / sample count | Pending |
| Real decoder and settings screenshots | Pending |
| Limitations / repeatability | Pending |

No scan addresses, screenshots, decoded frames or successful hardware outcomes have been fabricated. This file is a preparation template, not a completed I2C report.
