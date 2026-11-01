#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <driver/gpio.h>
#include <math.h>

// 教学示例需要额外 BME280；当前 LCD/DS1302 不是该传感器。
constexpr int SDA_PIN=21, SCL_PIN=22;
Adafruit_BME280 sensor;
uint8_t deviceAddress=0;
uint16_t digT1=0;
int32_t digT2=0, digT3=0;
bool ready=false;
uint32_t lastRead=0;

bool busIdle() {
  for(int i=0;i<20;++i) {
    if(!gpio_get_level(GPIO_NUM_21) || !gpio_get_level(GPIO_NUM_22)) return false;
    delay(1);
  }
  return true;
}

bool readBytes(uint8_t address,uint8_t reg,uint8_t *out,size_t count) {
  if(!busIdle()) { Serial.println("BUS_BLOCKED: read skipped"); return false; }
  Wire.beginTransmission(address);
  if(Wire.write(reg)!=1 || Wire.endTransmission(false)!=0) return false;
  size_t received=Wire.requestFrom(address,count,true);
  if(received!=count) { while(Wire.available()) Wire.read(); return false; }
  for(size_t i=0;i<count;++i) { int v=Wire.read(); if(v<0) return false; out[i]=uint8_t(v); }
  return true;
}

bool writeRegister(uint8_t reg,uint8_t value) {
  if(!busIdle()) return false;
  Wire.beginTransmission(deviceAddress);
  Wire.write(reg); Wire.write(value);
  return Wire.endTransmission(true)==0;
}

int32_t signedLE16(const uint8_t *p) {
  uint16_t u=uint16_t(p[0]) | (uint16_t(p[1])<<8);
  return (u&0x8000) ? int32_t(u)-65536 : int32_t(u);
}

bool rawTemperature(uint32_t &raw,uint8_t *bytes) {
  if(!readBytes(deviceAddress,0xFA,bytes,3)) return false;
  raw=(uint32_t(bytes[0])<<12)|(uint32_t(bytes[1])<<4)|(bytes[2]>>4);
  return raw!=0x80000; // 数据手册规定的 skipped-temperature 标记
}

double compensateTemperature(uint32_t raw) {
  double v1=(double(raw)/16384.0-double(digT1)/1024.0)*double(digT2);
  double term=double(raw)/131072.0-double(digT1)/8192.0;
  double v2=term*term*double(digT3);
  return (v1+v2)/5120.0;
}

bool measureOnce() {
  // 温度 x1，湿度/气压关闭；强制模式完成后休眠，避免比较中数据更新。
  if(!writeRegister(0xF4,0x21)) return false;
  delay(10); // 温度 x1 转换留出余量；状态轮询另有 100 ms 上限。
  uint32_t start=millis();
  do {
    uint8_t status;
    if(!readBytes(deviceAddress,0xF3,&status,1)) return false;
    if((status&0x09)==0) return true;
    delay(2);
  } while(millis()-start<100);
  Serial.println("CONVERSION_TIMEOUT"); return false;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Task 6 BME280 temperature: Prepared / Not Hardware Verified");
  pinMode(SDA_PIN,INPUT_PULLUP); pinMode(SCL_PIN,INPUT_PULLUP); delay(50);
  if(!busIdle()) { Serial.println("BUS_BLOCKED: sensor presence NOT_TESTED; no readings"); return; }
  if(!Wire.begin(21,22,100000)) { Serial.println("I2C_INIT_FAILED"); return; }
  Wire.setTimeOut(50);
  gpio_input_enable(GPIO_NUM_21); gpio_input_enable(GPIO_NUM_22);
  if(!busIdle()) { Serial.println("BUS_BLOCKED: sensor presence NOT_TESTED"); return; }
  // 0x76/0x77 是数据手册候选地址，不是本次扫描发现的地址。
  for(uint8_t candidate: {uint8_t(0x76),uint8_t(0x77)}) {
    uint8_t id=0;
    if(readBytes(candidate,0xD0,&id,1) && id==0x60) { deviceAddress=candidate; break; }
  }
  if(!deviceAddress) { Serial.println("SENSOR_NOT_FOUND: no verified BME280 ID at candidate addresses"); return; }
  if(!sensor.begin(deviceAddress,&Wire)) { Serial.println("SENSOR_NOT_FOUND_OR_LIBRARY_INIT_FAILED"); return; }
  sensor.setSampling(Adafruit_BME280::MODE_SLEEP,Adafruit_BME280::SAMPLING_X1,
    Adafruit_BME280::SAMPLING_NONE,Adafruit_BME280::SAMPLING_NONE,
    Adafruit_BME280::FILTER_OFF,Adafruit_BME280::STANDBY_MS_1000);
  uint8_t ctrl=0,hum=0,config=0;
  if(!readBytes(deviceAddress,0xF4,&ctrl,1) || !readBytes(deviceAddress,0xF2,&hum,1) ||
     !readBytes(deviceAddress,0xF5,&config,1) || (ctrl&0xFC)!=0x20 || (hum&7)!=0 || (config&0x1C)!=0) {
    Serial.println("CONFIG_READBACK_FAILED"); return;
  }
  uint8_t cal[6];
  if(!readBytes(deviceAddress,0x88,cal,6)) { Serial.println("CALIBRATION_READ_FAILED"); return; }
  digT1=uint16_t(cal[0])|(uint16_t(cal[1])<<8);
  digT2=signedLE16(cal+2); digT3=signedLE16(cal+4);
  if(digT1==0 || digT1==0xFFFF) { Serial.println("CALIBRATION_INVALID"); return; }
  Serial.printf("BME280_ID_VERIFIED address=0x%02X digT1=%u digT2=%ld digT3=%ld\n",
    deviceAddress,digT1,(long)digT2,(long)digT3);
  ready=true;
}

void loop() {
  if(!ready) { delay(100); return; }
  if(millis()-lastRead<1000) { delay(1); return; }
  lastRead=millis();
  if(!busIdle()) { Serial.println("BUS_BLOCKED: halted; no recovery"); ready=false; return; }
  if(!measureOnce()) { Serial.println("READ_FAILED: no physical values reported"); ready=false; return; }
  uint32_t before,after; uint8_t bytes[3],check[3];
  if(!rawTemperature(before,bytes)) { Serial.println("RAW_READ_FAILED"); return; }
  float libraryC=sensor.readTemperature();
  if(!rawTemperature(after,check) || before!=after || !isfinite(libraryC)) {
    Serial.println("READ_FAILED_OR_CHANGED_SAMPLE: no comparison"); return;
  }
  double directC=compensateTemperature(before);
  Serial.printf("t_ms=%lu raw_bytes=%02X,%02X,%02X adc_T=%lu library_C=%.4f direct_C=%.4f difference_C=%.4f\n",
    (unsigned long)millis(),bytes[0],bytes[1],bytes[2],(unsigned long)before,
    libraryC,directC,directC-libraryC);
}
