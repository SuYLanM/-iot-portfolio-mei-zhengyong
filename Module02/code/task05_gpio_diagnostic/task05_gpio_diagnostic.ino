#include <Arduino.h>
#include <Wire.h>
#include <driver/gpio.h>

// Passive input/weak-pull-up diagnostics only. No GPIO OUTPUT or bus-clear pulses.
constexpr int SDA_PIN = 21, SCL_PIN = 22;
constexpr int SAMPLES = 100;
bool wireReady = false;

bool observe(const char *phase) {
  int sdaHigh = 0, sclHigh = 0;
  for (int i = 0; i < SAMPLES; ++i) {
    sdaHigh += gpio_get_level(GPIO_NUM_21) != 0;
    sclHigh += gpio_get_level(GPIO_NUM_22) != 0;
    delay(5);
  }
  Serial.printf("LEVEL phase=%s samples=%d SDA_HIGH=%d SDA_LOW=%d SCL_HIGH=%d SCL_LOW=%d\n",
                phase, SAMPLES, sdaHigh, SAMPLES-sdaHigh, sclHigh, SAMPLES-sclHigh);
  gpio_dump_io_configuration(stdout, (1ULL << SDA_PIN) | (1ULL << SCL_PIN));
  return sdaHigh == SAMPLES && sclHigh == SAMPLES;
}

void diagnose() {
  if (wireReady) { Wire.end(); wireReady = false; }
  Serial.println("DIAG_BEGIN task05-passive-v1; no voltage measurement");
  pinMode(SDA_PIN, INPUT); pinMode(SCL_PIN, INPUT); delay(100);
  observe("INPUT_NO_PULL");
  pinMode(SDA_PIN, INPUT_PULLUP); delay(100);
  observe("SDA_PULLUP_ONLY");
  pinMode(SCL_PIN, INPUT_PULLUP); delay(100);
  const bool weakHigh = observe("BOTH_INTERNAL_PULLUP");
  wireReady = Wire.begin(21, 22, 100000);
  Wire.setTimeOut(50);
  Serial.printf("WIRE_BEGIN ok=%d clock_hz=%lu\n", wireReady, (unsigned long)Wire.getClock());
  if (wireReady) {
    delay(100); observe("AFTER_WIRE_BEGIN");
    const esp_err_t a = gpio_input_enable(GPIO_NUM_21);
    const esp_err_t b = gpio_input_enable(GPIO_NUM_22);
    Serial.printf("INPUT_ENABLE sda_status=%d scl_status=%d\n", a, b);
    delay(100); observe("AFTER_INPUT_ENABLE");
  }
  Serial.printf("DIAG_END weak_pullup_both_high=%d; scan not performed; internal pullups are not a qualified external I2C pullup solution\n", weakHigh);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Task 5 GPIO diagnostic v1; send d for passive test; send s for one gated scan");
}

void loop() {
  if (Serial.available()) {
    const char command = Serial.read();
    if (command == 'd') diagnose();
    if (command == 's') {
      if (!wireReady || !observe("PRE_SCAN")) {
        Serial.println("SCAN_NOT_RUN bus not ready/high; no recovery pulses");
      } else {
        unsigned ack=0, nack=0, errors=0;
        Serial.println("SCAN_BEGIN range=0x08-0x77 requested_hz=100000");
        for (uint8_t address=0x08; address<=0x77; ++address) {
          Wire.beginTransmission(address);
          uint8_t status=Wire.endTransmission(true);
          Serial.printf("PROBE address=0x%02X status=%u\n",address,status);
          if (status==0) ++ack; else if (status==2) ++nack; else ++errors;
          if (gpio_get_level(GPIO_NUM_21)==0 || gpio_get_level(GPIO_NUM_22)==0) {
            Serial.println("SCAN_ABORT line LOW after probe; no further probes");break;
          }
          delay(1);
        }
        Serial.printf("SCAN_END ack=%u nack=%u errors=%u probed=%u\n",ack,nack,errors,ack+nack+errors);
      }
    }
  }
  delay(1);
}
