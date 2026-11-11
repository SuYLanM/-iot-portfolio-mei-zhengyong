# Module 4 Task 3 — Six-axis attitude measurement

Open `Module4_Task3_IMU.ino` in Arduino IDE. Select **M5StickS3** from the M5Stack board package. The sketch uses M5Unified to initialize the built-in BMI270 and M5GFX display. Open the native USB serial port at 115200 baud. The sketch targets 25 serial samples per second and refreshes the screen about five times per second.

The local build uses M5Stack ESP32 3.3.10, M5Unified 0.3.0, M5GFX 0.3.0, and M5PM1 1.0.7. Select OPI PSRAM, Hardware CDC and JTAG, and USB CDC on boot. To enter download mode manually, hold the side Reset button until the green LED flashes, then release it. Windows can assign a different COM port in download mode.

## Angle convention

Acceleration is reported in g and angular rate in degrees per second, using the axes returned by M5Unified. Roll is `atan2(ay, az)` and pitch is `atan2(-ax, hypot(ay, az))`, both converted to degrees. These are gravity-based tilt estimates. The zero reference is the orientation with the static acceleration aligned with positive Z. Roll is ill-conditioned near pitch of ±90°.

During shaking, linear acceleration changes the apparent gravity vector and can bias the angles. The acceleration magnitude column helps flag some dynamic samples, although a value near 1 g does not guarantee static conditions. The gyroscope is logged separately. This sketch does not fuse angular rate into the tilt estimate or estimate absolute heading.

The CSV fields are `time_ms,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps,roll_deg,pitch_deg,accel_norm_g`. Time is elapsed device time in milliseconds.

The seven recorded test windows are listed with their CSVs, plots, and analysis in the [English experiment report](experiment-report.md).

[StickS3 hardware documentation](https://docs.m5stack.com/en/core/StickS3) · [Arduino setup and upload](https://docs.m5stack.com/en/arduino/m5sticks3/program) · [M5Unified IMU example](https://github.com/m5stack/M5Unified/tree/master/examples/Basic/Imu)
