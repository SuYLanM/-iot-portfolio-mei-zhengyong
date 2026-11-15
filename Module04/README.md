# Module 4 — Liquid Measurement & Sensor Selection

## Module Overview

This portfolio compares water-presence detection, continuous liquid-level estimation, and motion measurement in a tabletop setup. Tasks 1–3 are Mei Zhengyong’s experiments. Sections 4–6 present LudwigMo’s reference experiments and tank-selection proposal. The numbering is for portfolio navigation.

## Learning Objectives

- Distinguish a binary water alarm from a continuous liquid-level measurement.
- Evaluate calibration, repeatability, and the effects of conductivity, probe placement, and motion on sensor readings.
- Explain how acceleration and angular rate can identify conditions that may reduce confidence in a level reading.
- Select a primary and backup sensing principle for each tank based on the measurement decision and likely failure modes.

## Equipment

| Work | Equipment |
|---|---|
| Task 1 | ESP32 DevKit, LM393 module, two conductive probes, water |
| Task 2 | ESP32 DevKit, V1.2 capacitive soil-moisture probe, breadboard, jumper wires, clear plastic cup, water |
| Task 3 | M5Stack StickS3 with BMI270 and USB serial connection |
| Partner reference | ESP32 with an HD-38 conductive module, StickS3 with BMI270, and an HC-SR04 ultrasonic setup referenced in the tank decisions |

## Task 1 – Conductive Water Level Detection

The probe was tested in air, at first water contact, at roughly 2 and 5 cm immersion, and after removal. The five windows contain 105 samples. DO was high in air and low at all three water-contact conditions. The ADC reached full scale in air, and mean ADC counts across the wet conditions did not change monotonically with depth. The observations support a point-level alarm for the tested setup. The report’s AO figure provides the visual record.

[Report](Task01_WaterLevel/experiment-report.md) · [Arduino sketch](Task01_WaterLevel/Task01_WaterLevel.ino) · [CSV records](Task01_WaterLevel/data/) · [AO time series](Task01_WaterLevel/ao-time-series.png) · [Setup guide](Task01_WaterLevel/README.md)

## Task 2 – Capacitive Liquid Level Measurement

The initial serial diagnostics returned zero counts in air and at approximately 1 cm. A separate 147-point synthetic dataset illustrates a depth-response analysis. The two operator-supplied datasets support preliminary endpoint fits and a held-out 2 cm check. Their source relationship remains unconfirmed because the values differ and the timestamped submission is spaced at 200 ms, compared with the archived sketch’s 500 ms sampling period. Hardware validation of the acquisition and tank-volume calibration remains open.

[Report](Task02_Capacitive/experiment-report.md) · [Arduino sketch](Task02_Capacitive/Task02_Capacitive.ino) · [Diagnostic and supplied CSVs](Task02_Capacitive/data/) · [Synthetic CSV](Task02_Capacitive/simulation/synthetic-capacitive-water-level.csv) · [Setup photo](Task02_Capacitive/images/experimental_setup.jpg) · [Simulated depth plot](Task02_Capacitive/images/adc_vs_depth.png) · [Simulated time plot](Task02_Capacitive/images/adc_over_time.png)

## Task 3 – Six-Axis IMU Measurement

The StickS3 recorded 1,749 frames over seven static, tilted, shaking, and recovered conditions. Three-axis acceleration was used to estimate roll and pitch, and angular rate was logged separately. During shaking, the acceleration magnitude ranged from 0.553 to 1.619 g. Linear acceleration and actual rotation both affect the gravity-based tilt estimate during movement. The plots show the recorded signals, and dynamic angle accuracy remains unquantified without a reference.

[Report](Task03_IMU/experiment-report.md) · [Arduino sketch](Task03_IMU/Task03_IMU.ino) · [Seven CSVs](Task03_IMU/data/) · [Test overview](Task03_IMU/figures/imu-test-overview.png) · [Shaking detail](Task03_IMU/figures/imu-shaking-detail.png)

## Section 4 – Binary Water Detection

LudwigMo used an HD-38 on GPIO27 and recorded a switching depth of approximately 0.3 cm and a 300 ms software debounce. The switching depth and debounce are specific to that reference setup. Task 1 provides separate wet/dry observations using the local wiring. The partner archive supplies the result table and screen capture.

[Partner report and recorded results](partner_reference/README.md#4-binary-water-detection) · [Arduino sketch](partner_reference/code/water_sensor_binary.ino) · [Setup photo](partner_reference/pictures/binary_sensor_setup.jpg) · [Serial output](partner_reference/pictures/binary_sensor_output.png)

## Section 5 – IMU Motion Detection and Sea-State Classification

LudwigMo classified laboratory motion using the mean absolute deviation of acceleration magnitude from 1 g over a short window. The reported thresholds are 0.025 g and 0.25 g. A gentle-motion window near the lower threshold was classified as stationary. These thresholds complement Task 3’s direct tilt and angular-rate measurements. The partner archive supplies summary values and screenshots for the three motion classes.

[Partner report and classification results](partner_reference/README.md#5-imu-motion-detection-and-sea-state-classification) · [IMU measurement sketch](partner_reference/code/imu_motion_test.ino) · [Classifier sketch](partner_reference/code/imu_sea_state.ino) · [Harbor capture](partner_reference/pictures/imu_harbor.png) · [Moving capture](partner_reference/pictures/imu_moving.png) · [Rough capture](partner_reference/pictures/imu_rough.png)

## Section 6 – Decision Sheet for Four Tanks

LudwigMo proposes continuous sensing with independent low-level alarms for diesel and fresh water, independent high-level detection for waste water, and staggered binary alarms for the bilge. The decision sheet links bench observations with expected failure modes and installation choices. The proposed vessel installations still require application-specific testing. The section uses the code, figures, and measurement summaries of the supporting experiments.

[Partner decision sheet and recommendations](partner_reference/README.md#6-decision-sheet--sensor-selection-for-four-tanks) · [Binary and IMU evidence](partner_reference/README.md#related-arduino-sketches) · [Partner ultrasonic calibration and setup](https://github.com/LudwigMo/sensorik-aktorik-portfolio/blob/a91cfa6491b854ff9ea7858242c6c1142b48ca65/Module04/README.md#2-volume-calibration-and-validation)

## Overall Discussion

The two conductive demonstrations support wet/dry detection in their respective setups. Probe depth, conductivity, and comparator adjustment affect the switching point, so LudwigMo’s trigger depth should be checked before using it with the local circuit. A continuous estimate also needs a validated relation between the sensor reading, depth, and tank volume. The capacitive analysis illustrates that relation at a few depth points, while LudwigMo’s ultrasonic reference uses several known volumes and documents a separate foam and tilt test. Neither set supplies synchronized liquid-level and motion readings. That test would show when a motion flag or extra settling time improves the tank display.

## Conclusion

The portfolio demonstrates binary water detection and six-axis motion logging. A calibrated continuous volume display still needs measurements at known fill volumes and independent validation. The tank-selection proposal uses distinct primary and backup principles because the four tanks have different decisions and failure modes.

## References and Contributions

- Mei Zhengyong contributed the Task 1–3 sketches, acquisition records, analyses, and reports.
- Sections 4–6 retain LudwigMo’s original text, reported results, sketches, and images in the partner archive, including the source license. Measurements and recommendations there are credited to LudwigMo. No standalone partner CSV was supplied for these sections. [Partner reference and license](partner_reference/README.md)
- The course workbook defines the learning goals and the joint decision sheet and demonstration deliverables. [HSBI Module 4 workbook](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/04-liquid-measurement-and-sensor-selection.md)
