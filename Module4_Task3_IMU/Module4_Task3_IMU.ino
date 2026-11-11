#include <Arduino.h>
#include <M5Unified.h>
#include <cmath>

constexpr uint32_t SAMPLE_PERIOD_MS = 40;
constexpr uint32_t DISPLAY_PERIOD_MS = 200;
constexpr float RAD_TO_DEGREES = 57.2957795f;

bool imuReady = false;
uint32_t lastSampleMs = 0;
uint32_t lastDisplayMs = 0;

void drawReadings(const m5::IMU_Class::imu_data_t& data, float roll, float pitch) {
  M5.Display.setCursor(4, 25);
  M5.Display.printf("A%+6.2f%+6.2f%+6.2f", data.accel.x, data.accel.y, data.accel.z);
  M5.Display.setCursor(4, 45);
  M5.Display.printf("G%+6.0f%+6.0f%+6.0f", data.gyro.x, data.gyro.y, data.gyro.z);
  M5.Display.setCursor(4, 76);
  M5.Display.printf("Roll  %+7.2f deg", roll);
  M5.Display.setCursor(4, 98);
  M5.Display.printf("Pitch %+7.2f deg", pitch);
}

void setup() {
  auto config = M5.config();
  config.internal_imu = true;
  M5.begin(config);
  Serial.begin(115200);

  M5.Display.setRotation(1);
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.setTextSize(2);
  M5.Display.setCursor(4, 4);

  imuReady = M5.Imu.getType() == m5::imu_bmi270;
  if (!imuReady) {
    M5.Display.println("BMI270 unavailable");
    return;
  }

  M5.Display.println("BMI270  X   Y   Z");
  M5.Display.setTextSize(1);
  M5.Display.setCursor(4, 120);
  M5.Display.println("A: g   G: deg/s   Angles: accel tilt");
  M5.Display.setTextSize(2);
  Serial.println("# StickS3 BMI270 ready");
  Serial.println("time_ms,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps,roll_deg,pitch_deg,accel_norm_g");
}

void loop() {
  M5.update();
  if (!imuReady) {
    Serial.println("# ERROR: BMI270 unavailable");
    delay(1000);
    return;
  }

  const uint32_t now = millis();
  if (now - lastSampleMs < SAMPLE_PERIOD_MS) {
    delay(1);
    return;
  }

  const auto fresh = M5.Imu.update();
  constexpr auto required = m5::IMU_Class::sensor_mask_accel | m5::IMU_Class::sensor_mask_gyro;
  if ((fresh & required) != required) {
    delay(1);
    return;
  }
  lastSampleMs = now;

  const auto& data = M5.Imu.getImuData();
  const float yz = std::hypot(data.accel.y, data.accel.z);
  const float roll = std::atan2(data.accel.y, data.accel.z) * RAD_TO_DEGREES;
  const float pitch = std::atan2(-data.accel.x, yz) * RAD_TO_DEGREES;
  const float norm = std::hypot(data.accel.x, yz);

  Serial.printf("%lu,%.5f,%.5f,%.5f,%.4f,%.4f,%.4f,%.3f,%.3f,%.5f\n",
                static_cast<unsigned long>(now), data.accel.x, data.accel.y, data.accel.z,
                data.gyro.x, data.gyro.y, data.gyro.z, roll, pitch, norm);

  if (now - lastDisplayMs >= DISPLAY_PERIOD_MS) {
    lastDisplayMs = now;
    drawReadings(data, roll, pitch);
  }
}
