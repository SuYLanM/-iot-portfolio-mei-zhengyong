#include <M5Unified.h>
#include <math.h>

void setup() {
  Serial.begin(115200);
  M5.begin();

  Serial.println("M5StickS3 IMU Motion Test");
  Serial.println("ax,ay,az,accel_magnitude");
}

void loop() {
  M5.update();

  if (M5.Imu.update()) {
    auto data = M5.Imu.getImuData();

    float ax = data.accel.x;
    float ay = data.accel.y;
    float az = data.accel.z;

    float magnitude = sqrt(ax * ax + ay * ay + az * az);

    Serial.printf("%.3f,%.3f,%.3f,%.3f\n",
                  ax, ay, az, magnitude);
  }

  delay(100);
}
