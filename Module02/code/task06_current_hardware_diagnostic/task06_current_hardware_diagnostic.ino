#include <Arduino.h>
#include <Wire.h>
#include <driver/gpio.h>

constexpr int SDA_PIN = 21, SCL_PIN = 22;
constexpr uint32_t BUS_HZ = 100000;
bool wireReady = false;

// 只使用输入/弱上拉；不强制输出高电平、不发送恢复脉冲。
bool idleSample(unsigned &sdaHigh, unsigned &sclHigh) {
  sdaHigh = sclHigh = 0;
  for (int i = 0; i < 50; ++i) {
    sdaHigh += gpio_get_level(GPIO_NUM_21) != 0;
    sclHigh += gpio_get_level(GPIO_NUM_22) != 0;
    delay(2);
  }
  return sdaHigh == 50 && sclHigh == 50;
}

void runRound(unsigned round) {
  Serial.printf("ROUND_BEGIN round=%u firmware=task06-current-v1\n", round);
  if (wireReady) { Wire.end(); wireReady = false; }
  pinMode(SDA_PIN, INPUT_PULLUP); pinMode(SCL_PIN, INPUT_PULLUP);
  delay(50);
  unsigned sdaHigh, sclHigh;
  bool idle = idleSample(sdaHigh, sclHigh);
  Serial.printf("IDLE samples=50 SDA_HIGH=%u SDA_LOW=%u SCL_HIGH=%u SCL_LOW=%u input_pullup=1\n",
                sdaHigh,50-sdaHigh,sclHigh,50-sclHigh);
  if (!idle) {
    Serial.println("BUS_BLOCKED: SDA/SCL not stably HIGH; no I2C transaction or recovery attempted");
    Serial.printf("ROUND_END round=%u status=BUS_BLOCKED probed=0 ack=NA nack=NA\n",round);
    return;
  }
  wireReady = Wire.begin(21,22,BUS_HZ);
  Wire.setTimeOut(50);
  if (!wireReady) { Serial.printf("ROUND_END round=%u status=I2C_INIT_FAILED\n",round); return; }
  gpio_input_enable(GPIO_NUM_21); gpio_input_enable(GPIO_NUM_22);
  if (!idleSample(sdaHigh,sclHigh)) {
    Serial.printf("ROUND_END round=%u status=BUS_BLOCKED_AFTER_BEGIN probed=0\n",round); return;
  }
  unsigned ack=0,nack=0,errors=0,probed=0;
  for (uint8_t address=0x08; address<=0x77; ++address) {
    if (!idleSample(sdaHigh,sclHigh)) { Serial.println("BUS_BLOCKED: scan stopped"); break; }
    Wire.beginTransmission(address);
    uint8_t status=Wire.endTransmission(true); ++probed;
    if (status==0) { ++ack; Serial.printf("ACK address_7bit=0x%02X identity=UNVERIFIED\n",address); }
    else if (status==2) ++nack;
    else { ++errors; Serial.printf("BUS_ERROR address=0x%02X status=%u\n",address,status); break; }
  }
  Serial.printf("ROUND_END round=%u probed=%u ack=%u nack=%u errors=%u status=%s\n",
                round,probed,ack,nack,errors,probed==112 && errors==0 ? "SCAN_COMPLETE" : "SCAN_INCOMPLETE");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Task 6 current hardware diagnostic v1; send g for exactly three rounds");
  Serial.println("DS1302 is NOT I2C; PCF8574 is an I/O expander, not a measurement sensor");
}

void loop() {
  if (Serial.available() && Serial.read()=='g') {
    Serial.println("SESSION_BEGIN ESP32 SDA=21 SCL=22 requested_hz=100000 serial=115200");
    for (unsigned round=1;round<=3;++round) { runRound(round); if(round<3) delay(3000); }
    if (wireReady) { Wire.end(); wireReady=false; }
    pinMode(SDA_PIN,INPUT); pinMode(SCL_PIN,INPUT);
    Serial.println("SESSION_END: no automatic retry; GPIO inputs, no recovery pulses; sensor comparison NOT_RUN");
  }
  delay(1);
}
