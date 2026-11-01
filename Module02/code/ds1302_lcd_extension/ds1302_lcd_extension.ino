#include <Arduino.h>
#include <Wire.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <driver/gpio.h>

// 扩展实验，不是 Task 6 的 I2C 传感器库/寄存器比较。
constexpr int RTC_DAT=25, RTC_CLK=26, RTC_CE=27;
constexpr int VERIFIED_LCD_ADDRESS=0x27; // 2026-11-02 实际 Scanner 五轮 ACK，不是预设猜测。
ThreeWire rtcWire(RTC_DAT,RTC_CLK,RTC_CE);
RtcDS1302<ThreeWire> rtc(rtcWire);
hd44780_I2Cexp lcd(static_cast<uint8_t>(VERIFIED_LCD_ADDRESS));
bool lcdReady=false,havePrevious=false,started=false,readbackDone=false;
uint64_t previousSeconds=0;
unsigned sample=0;
uint32_t lastRead=0;

bool idleBus() {
  unsigned a=0,b=0;
  for(int i=0;i<20;++i) { a+=gpio_get_level(GPIO_NUM_21)!=0; b+=gpio_get_level(GPIO_NUM_22)!=0; delay(2); }
  Serial.printf("I2C_IDLE samples=20 SDA_HIGH=%u SCL_HIGH=%u\n",a,b);
  return a==20 && b==20;
}

bool bcdValid(uint8_t b,uint8_t max) {
  return (b&15)<=9 && (b>>4)<=9 && (b>>4)*10+(b&15)<=max;
}

void startSession() {
  Serial.println("SESSION_BEGIN DS1302_EXTENSION v2 DAT=25 CLK=26 CE=27 serial=115200");
  Serial.println("READ_ONLY_RTC: no date/time, halt, write-protect or charger setting is changed");
  rtc.Begin(); // 只初始化三线 GPIO，不调用任何 Set 方法。
  pinMode(21,INPUT_PULLUP); pinMode(22,INPUT_PULLUP); delay(50);
  bool idle=idleBus();
  if(!idle) Serial.println("LCD_DISABLED_BUS_BLOCKED: continue DS1302 three-wire reads only");
  else if(VERIFIED_LCD_ADDRESS<0x08 || VERIFIED_LCD_ADDRESS>0x77)
    Serial.println("LCD_DISABLED_NO_VERIFIED_ADDRESS: no guessed LCD address used");
  else if(Wire.begin(21,22,100000)) {
    Wire.setTimeOut(50);
    gpio_input_enable(GPIO_NUM_21);gpio_input_enable(GPIO_NUM_22);
    if(idleBus()) {
      Wire.beginTransmission(uint8_t(VERIFIED_LCD_ADDRESS));
      uint8_t ack=Wire.endTransmission(true);
      Serial.printf("LCD_ADDRESS_CHECK address=0x%02X status=%u\n",VERIFIED_LCD_ADDRESS,ack);
      if(ack==0) {
        int status=lcd.begin(16,2);lcdReady=status==0;
        Serial.printf("LCD_INIT status=%d ready=%d visible_text=NOT_VISUALLY_VERIFIED\n",status,lcdReady);
      }
    }
  } else Serial.println("LCD_DISABLED_I2C_INIT_FAILED");
  started=true;lastRead=millis()-1000;
}

void readAndDisplay() {
  ++sample;
  uint8_t raw[8];
  rtcWire.beginTransmission(0xBF); // DS1302 三线时钟 burst READ，不是 I2C。
  for(int i=0;i<8;++i) raw[i]=rtcWire.read();
  rtcWire.endTransmission();
  Serial.printf("RTC_RAW sample=%u bytes=",sample);
  for(int i=0;i<8;++i) Serial.printf("%02X%s",raw[i],i==7 ? "\n" : ",");
  bool hours=(raw[2]&0x80) ? (bcdValid(raw[2]&0x1F,12) && (raw[2]&0x1F)!=0) : bcdValid(raw[2]&0x3F,23);
  bool validRaw=bcdValid(raw[0]&0x7F,59) && bcdValid(raw[1],59) && hours &&
    bcdValid(raw[3],31) && raw[3]!=0 && bcdValid(raw[4],12) && raw[4]!=0 &&
    raw[5]>=1 && raw[5]<=7 && bcdValid(raw[6],99);
  RtcDateTime dt=rtc.GetDateTime();
  bool valid=validRaw && dt.IsValid();
  bool running=(raw[0]&0x80)==0;
  bool writeProtected=(raw[7]&0x80)!=0;
  if(!valid) {
    Serial.printf("RTC_INVALID sample=%u raw_bcd_valid=%d calendar_valid=%d running_flag=%d; no valid timestamp\n",
      sample,validRaw,dt.IsValid(),running);
    havePrevious=false;
    if(lcdReady && idleBus()) { lcd.setCursor(0,0);lcd.print("RTC invalid     ");lcd.setCursor(0,1);lcd.print("No auto setting "); }
    return;
  }
  char date[17],time[17];
  snprintf(date,sizeof(date),"%04u-%02u-%02u",dt.Year(),dt.Month(),dt.Day());
  snprintf(time,sizeof(time),"%02u:%02u:%02u",dt.Hour(),dt.Minute(),dt.Second());
  uint64_t seconds=dt.TotalSeconds64();
  Serial.printf("RTC_READ sample=%u t_ms=%lu date=%s time=%s valid=1 running=%d write_protected=%d",
    sample,(unsigned long)millis(),date,time,running,writeProtected);
  if(havePrevious) {
    long long delta=(long long)seconds-(long long)previousSeconds;
    Serial.printf(" delta_seconds=%lld progression=%s\n",delta,delta>0 ? "ADVANCING" : delta==0 ? "UNCHANGED" : "BACKWARDS");
  } else Serial.println(" delta_seconds=NA progression=FIRST_VALID_SAMPLE");
  previousSeconds=seconds;havePrevious=true;
  if(lcdReady) {
    if(!idleBus()) { lcdReady=false;Serial.println("LCD_DISABLED_BUS_BLOCKED; RTC continues"); }
    else {
      lcd.setCursor(0,0);lcd.print(date);lcd.print("      ");
      lcd.setCursor(0,1);lcd.print(time);lcd.print("        ");
      Serial.println("LCD_WRITE_ATTEMPTED: visible content requires real observation");
      if(!readbackDone) {
        char row1[17],row2[17],expected1[17],expected2[17];
        snprintf(expected1,sizeof(expected1),"%-16s",date);
        snprintf(expected2,sizeof(expected2),"%-16s",time);
        bool readOk=lcd.setCursor(0,0)==0;
        for(int i=0;i<16;++i) { int c=lcd.read();if(c<0)readOk=false;row1[i]=c<0 ? '?' : char(c); }row1[16]=0;
        if(lcd.setCursor(0,1)!=0)readOk=false;
        for(int i=0;i<16;++i) { int c=lcd.read();if(c<0)readOk=false;row2[i]=c<0 ? '?' : char(c); }row2[16]=0;
        bool match=readOk && strcmp(row1,expected1)==0 && strcmp(row2,expected2)==0;
        Serial.printf("LCD_DDRAM_READBACK read_ok=%d match=%d row1=\"%s\" row2=\"%s\" visible_text=NOT_VISUALLY_VERIFIED\n",readOk,match,row1,row2);
        readbackDone=true;
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("DS1302 + LCD extension ready; send g to start read-only RTC test");
}
void loop() {
  if(Serial.available() && Serial.read()=='g' && !started) startSession();
  if(started && millis()-lastRead>=1000) { lastRead=millis();readAndDisplay(); }
  delay(1);
}
