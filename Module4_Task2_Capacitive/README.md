# Module 4 – Experiment 2: Capacitive Liquid Level Measurement

The ESP32 reads the Capacitive Soil Moisture Sensor V1.2 at 500 ms intervals using a 12-bit ADC. Connect VCC to 3V3, GND to GND, and AOUT to GPIO34. Open the sketch in Arduino IDE as **ESP32 Dev Module** and use a 115200 baud serial monitor.

| Resource | Contents |
|---|---|
| [Arduino sketch](Module4_Task2_Capacitive.ino) | GPIO34 logger with elapsed time and raw ADC counts |
| [Experiment report](experiment-report.md) | Procedure, results, discussion, and calibration status |
| [Synthetic dataset](simulation/synthetic-capacitive-water-level.csv) | Original 147 modeled samples with provenance in each row |
| [ADC vs depth](images/adc_vs_depth.png) | Modeled mean and sample standard deviation by depth |
| [ADC over time](images/adc_over_time.png) | Modeled traces across seven windows |
| [Bench diagnostic archive](data/) | Air and approximately 1 cm immersion captures, including original and deduplicated records |

The bench diagnostic returned ADC 0 in both conditions. A valid hardware depth calibration is pending.

The setup photograph is pending. Its intended path in this experiment is `images/experimental_setup.jpg`.
