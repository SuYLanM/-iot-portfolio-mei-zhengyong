#include <M5Unified.h>
#include <math.h>

constexpr int SAMPLE_COUNT = 20;
constexpr int SAMPLE_INTERVAL_MS = 50;

// Experimental thresholds (in g)
// These values must be validated with our measurements.
constexpr float HARBOR_THRESHOLD = 0.025f;
constexpr float ROUGH_THRESHOLD = 0.25f;

float sumDeviation = 0;
float maxDeviation = 0;
int sampleCount = 0;

void setup() {
  Serial.begin(115200);
  M5.begin();

  Serial.println("IMU Sea State Classification");
  Serial.println("MeanDev_g,MaxDev_g,State");
}

void loop() {
  M5.update();

  if (M5.Imu.update()) {
    auto data = M5.Imu.getImuData();

    float ax = data.accel.x;
    float ay = data.accel.y;
    float az = data.accel.z;

    float magnitude = sqrtf(
      ax * ax + ay * ay + az * az
    );

    float deviation = fabsf(magnitude - 1.0f);

    sumDeviation += deviation;

    if (deviation > maxDeviation) {
      maxDeviation = deviation;
    }

    sampleCount++;
  }

  if (sampleCount >= SAMPLE_COUNT) {
    float meanDeviation = sumDeviation / sampleCount;

    const char* state;

    if (meanDeviation < HARBOR_THRESHOLD) {
      state = "IN HARBOR";
    }
    else if (meanDeviation >= ROUGH_THRESHOLD) {
      state = "ROUGH";
    }
    else {
      state = "MOVING";
    }

    Serial.printf("%.3f,%.3f,%s\n",
                  meanDeviation,
                  maxDeviation,
                  state);

    sumDeviation = 0;
    maxDeviation = 0;
    sampleCount = 0;
  }

  delay(SAMPLE_INTERVAL_MS);
}
