# Task 6 — Current Hardware Diagnostic

Built and uploaded on the real ESP32 / COM3 on 2026-11-02. Three actual rounds reported BUS_BLOCKED, SDA and SCL LOW for 50/50 samples each; no address probes or recovery pulses were sent. [Original serial log](../../evidence/task06-library-vs-register/current-hardware-serial-20261102.txt).

Open the same-name .ino here in Arduino IDE 2.3.10, ESP32 Dev Module / Core 3.3.12. Serial 115200; send `g` for exactly three rounds. Intended pins: SDA21, SCL22, compatible 3.3 V/GND. Circuit unchanged; no physical adjustment is requested. The code uses input weak pull-ups only for idle diagnostics, never push-pull HIGH. Blocked rounds skip Wire initialization. An idle bus can proceed to an address-only scan of 0x08–0x77, with 50 ms operation timeout and stop-on-error; ACK identity is unverified. No LCD address is presumed. DS1302 is excluded. After a session the pins return to ordinary input and the program waits for commands.

Compile PASS: flash 295276 bytes, RAM 23500 bytes. Upload Hash verified. This replaces only the selected ESP32 firmware; Task 5 source/logs and all Task 9 files remain preserved.

[Sketch](task06_current_hardware_diagnostic.ino) · [English report](../../register-reading.md) · [Chinese guide](../../wiring-zh.md#task-6库读取与直接寄存器读取).
