# Software verification

Local Windows 11 environment with Arduino IDE 2.3.10's Arduino CLI. Target `esp32:esp32:esp32` (ESP32 Dev Module), ESP32 Core 3.3.12. The build records below describe successive revisions. Later raw evidence documents successful uploads and acquisition.

| Sketch | Result | Flash bytes | Global RAM bytes |
|---|---|---:|---:|
| [Analog capture](../code/analog_capture/analog_capture.ino) | Compiled | 278964 | 22196 |
| [DS18B20 capture](../code/ds18b20_capture/ds18b20_capture.ino) | Compiled with OneWire 2.3.8 and DallasTemperature 4.0.5 | 276916 | 22164 |

Compile each sketch using `arduino-cli compile --fqbn esp32:esp32:esp32 <sketch-directory>`. Use the installed core and library versions above to reproduce the check.

The updated Task 2 GPIO18 sketch with a serial transfer marker compiled on the same core with OneWire 2.3.8 and DallasTemperature 4.0.6. Flash usage was 277176 bytes and global RAM usage was 22164 bytes. No upload or DS18B20 reading was performed for this build.

The subsequent GPIO18 build with ROM counts, conversion timing and diagnostic statuses compiled offline with the same versions. Arduino CLI reported 277464 bytes of flash and 22164 bytes of global RAM. Windows enumerated a CP210x bridge on COM3 before power was disconnected. No upload or sensor traffic was performed for this build.

`python Module03/test_analysis.py` passed 11 synthetic cases using Python 3.11, NumPy 2.4.0 and Matplotlib 3.10.8. Tests cover the fit and validation paths, hysteresis, repeatability, thermal timing, filter spike and lag behavior, decision output and uncertainty arithmetic. The test builds fixtures in a temporary directory.

Hardware acquisition was subsequently performed. See [Task 2 retest](task02-ds18b20/timing-retest/), [ADC calibration](task03-calibration/adc/), [Task 4 returns](task04b-pot/), [LDR noise](task05-noise/hardware-ldr/) and [pot noise and transition](task05-noise/hardware-pot/) for the build, upload and serial records used for each run. The [final audit](../reports/final-submission-audit.md) states outstanding course requirements.
