# DS1302 + LCD1602 Extension — Real Hardware Run

Open [ds1302_lcd_extension.ino](ds1302_lcd_extension.ino) as its own same-name Arduino sketch. This is an **extension**, not Task 6 sensor comparison. [Actual results / all logs](../../evidence/ds1302-lcd-extension/README.md).

- ESP32 Dev Module / Core 3.3.12 / COM3 CP210x; Serial **115200**. Rtc by Makuna **2.5.0**, hd44780 by Bill Perry **1.3.2**, installed via Arduino Library Manager/CLI.
- LCD: VCC→3V3, GND→GND, SDA→GPIO21, SCL→GPIO22. Actual preceding Scanner found **0x27** in five complete sweeps; this is the address used by the final program, not a default guess.
- DS1302: VCC→3V3, GND→GND, DAT→GPIO25, CLK→GPIO26, RST/CE→GPIO27. DS1302 uses ThreeWire, not I2C.
- Final compile PASS: **300760 flash bytes / 23580 global RAM bytes**. Uploaded on COM3, **Hash of data verified**. Final capture contains ten valid RTC reads, 2026-11-01 23:48:59 through 23:49:08, +1 s each; the earlier fifteen-read capture is also preserved.
- Send **g** once to start. RTC readings and LCD output continue about once per second; reboot returns to command-wait mode. No RTC date/time, halt, write-protect or charger setting is changed. Missing/invalid/stopped RTC data is never replaced with compile time.
- LCD ACK and initialization succeeded; one real two-row DDRAM readback matched the written date/time. Physical text visibility still needs actual observation. Current civil-time accuracy/timezone was not established.
- If the I2C bus is LOW or LCD fails, the program continues RTC-only operation. The actual existing 3.3 V wiring is user-reported; GPIO HIGH/ACK is not a voltage measurement. Never attach 5 V pull-ups.

No new wiring is requested by these instructions. Task 6's BME280 example remains compiled-only and its real sensor comparison Pending.
