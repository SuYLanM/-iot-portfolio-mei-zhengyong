#include <Arduino.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <driver/gpio.h>
#if !defined(CONFIG_IDF_TARGET_ESP32)
#error "This master/local-slave sketch targets classic ESP32 only."
#endif
#include "test_protocol.h"
#include "local_slave.h"

constexpr int SDA_PIN=21,SCL_PIN=22;
constexpr uint8_t LCD_ADDRESS=0x27; // actual Task 5 evidence, re-probed in each run
hd44780_I2Cexp lcd(LCD_ADDRESS);
bool masterReady=false,lcdReady=false;
uint8_t sequence=0;

bool idleBus() {
  for(int i=0;i<5;++i) {
    if(!gpio_get_level(GPIO_NUM_21) || !gpio_get_level(GPIO_NUM_22))return false;
    delay(1);
  }
  return true;
}
bool startMaster() {
  if(masterReady)return idleBus();
  pinMode(21,INPUT_PULLUP);pinMode(22,INPUT_PULLUP);delay(30);
  if(!idleBus()) { Serial.println("BUS_BLOCKED: no initialization/recovery/probes");return false; }
  masterReady=Wire.begin(21,22,100000);Wire.setTimeOut(50);
  if(masterReady) { gpio_input_enable(GPIO_NUM_21);gpio_input_enable(GPIO_NUM_22); }
  Serial.printf("MASTER init_ok=%d SDA=21 SCL=22 requested_hz=100000 timeout_ms=50\n",masterReady);
  return masterReady && idleBus();
}

bool testTarget() {
  if(!idleBus())return false;
  ++sequence;
  Wire.beginTransmission(TEST_ADDRESS);Wire.write(COMMAND);Wire.write(sequence);
  uint8_t status=Wire.endTransmission(true); // STOP allows asynchronous slave processing
  Serial.printf("TEST_COMMAND address=0x42 seq=%u wire_status=%u\n",sequence,status);
  if(status!=0)return false;
  delay(20);
  for(int attempt=1;attempt<=3;++attempt) {
    if(!idleBus())return false;
    uint8_t packet[PACKET_SIZE]={0};
    size_t got=Wire.requestFrom(TEST_ADDRESS,size_t(PACKET_SIZE),true);
    size_t count=0;while(Wire.available() && count<PACKET_SIZE)packet[count++]=uint8_t(Wire.read());
    bool valid=got==PACKET_SIZE && count==PACKET_SIZE && validPacket(packet,count,sequence);
    Serial.printf("TEST_RESPONSE attempt=%d requested=12 received=%u consumed=%u valid=%d raw=",attempt,unsigned(got),unsigned(count),valid);
    for(size_t i=0;i<count;++i)Serial.printf("%02X%s",packet[i],i+1==count ? "\n" : ",");
    if(count==0)Serial.println("NONE");
    if(valid) {
      unsigned requests=unsigned(packet[8])|(unsigned(packet[9])<<8);
      Serial.printf("TEST_COMM_PASS seq=%u target_uptime_ms=%lu accepted_requests=%u data_kind=MCU_TEST_NOT_SENSOR\n",
        sequence,(unsigned long)packetUptime(packet),requests);
      return true;
    }
    delay(10); // bounded retry also rejects an ESP32 preloaded stale response
  }
  Serial.println("TEST_COMM_FAILED: no matching protocol packet; ACK alone is not success");return false;
}

bool testLcd(unsigned round,bool targetValid) {
  if(!idleBus())return false;
  if(!lcdReady) {
    int status=lcd.begin(16,2);lcdReady=status==0;
    Serial.printf("LCD_INIT address=0x27 status=%d\n",status);
    if(!lcdReady)return false;
  }
  char row1[17]="Task7 LCD 0x27  ",text[17],row2[17],read1[17],read2[17];
  snprintf(text,sizeof(text),"round=%u B:%s",round,targetValid ? "PASS" : "missing");
  snprintf(row2,sizeof(row2),"%-16s",text);
  bool ok=lcd.setCursor(0,0)==0;ok=(lcd.print(row1)==16)&&ok;
  ok=(lcd.setCursor(0,1)==0)&&ok;ok=(lcd.print(row2)==16)&&ok;
  ok=(lcd.setCursor(0,0)==0)&&ok;
  for(int i=0;i<16;++i) { int c=lcd.read();if(c<0)ok=false;read1[i]=c<0 ? '?' : char(c); }read1[16]=0;
  ok=(lcd.setCursor(0,1)==0)&&ok;
  for(int i=0;i<16;++i) { int c=lcd.read();if(c<0)ok=false;read2[i]=c<0 ? '?' : char(c); }read2[16]=0;
  ok=ok && strcmp(row1,read1)==0 && strcmp(row2,read2)==0;
  Serial.printf("LCD_COMM match=%d row1=\"%s\" row2=\"%s\" data_kind=LCD_DDRAM_NOT_SENSOR\n",ok,read1,read2);
  return ok;
}

void runSession(bool allowTestProtocol) {
  Serial.printf("SESSION_BEGIN TASK7_HARDWARE local_slave_enabled=%d protocol_armed=%d\n",localEnabled,allowTestProtocol);
  if(!startMaster()) { Serial.println("SESSION_END status=BUS_BLOCKED_OR_INIT_FAILED");return; }
  for(unsigned round=1;round<=3;++round) {
    Serial.printf("SCAN_BEGIN round=%u SDA=%d SCL=%d\n",round,gpio_get_level(GPIO_NUM_21),gpio_get_level(GPIO_NUM_22));
    if(!idleBus()) { Serial.println("BUS_BLOCKED: session stopped");break; }
    unsigned ack=0,nack=0,errors=0,probed=0;bool foundLcd=false,foundTest=false;
    for(uint8_t address=0x08;address<=0x77;++address) {
      if(!gpio_get_level(GPIO_NUM_21) || !gpio_get_level(GPIO_NUM_22)) { Serial.println("BUS_BLOCKED: scan stopped");break; }
      Wire.beginTransmission(address);uint8_t status=Wire.endTransmission(true);++probed;
      if(status==0) {
        ++ack;Serial.printf("ACK address_7bit=0x%02X identity=UNVERIFIED_BY_ACK\n",address);
        if(address==LCD_ADDRESS)foundLcd=true;if(address==TEST_ADDRESS)foundTest=true;
      } else if(status==2)++nack;
      else { ++errors;Serial.printf("BUS_ERROR address=0x%02X status=%u\n",address,status);break; }
      delay(1);
    }
    Serial.printf("SCAN_END round=%u probed=%u ack=%u nack=%u errors=%u\n",round,probed,ack,nack,errors);
    if(probed!=112 || errors!=0) { Serial.println("COMM_SKIPPED: incomplete/error scan");break; }
    bool bOk=false,aOk=false;
    if(foundTest && allowTestProtocol)bOk=testTarget();
    else Serial.printf("TEST_TARGET status=%s address=0x42; no fabricated response\n",foundTest ? "PROTOCOL_NOT_ARMED" : "NOT_ACKNOWLEDGED");
    if(foundLcd)aOk=testLcd(round,bOk);
    else {lcdReady=false;Serial.println("LCD_NOT_ACKNOWLEDGED address=0x27");}
    Serial.printf("ROUND_RESULT round=%u lcd_ack=%d lcd_comm=%d test_ack=%d test_comm=%d both_address_and_comm_pass=%d\n",
      round,foundLcd,aOk,foundTest,bOk,foundLcd && foundTest && aOk && bOk);
    if(round<3)delay(3000);
  }
  Serial.println("SESSION_END no automatic retries; no claim of two physical sensors");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Task7 ESP32: g=baseline three rounds; a=local Wire1+master AFTER two jumpers; b=external test target AFTER wiring");
  Serial.println("0x27 historical LCD address; 0x42 configured candidate only until actual ACK+packet; DS1302 is NOT I2C");
}
void loop() {
  if(Serial.available()) {
    char c=Serial.read();
    if(c=='g')runSession(false);
    if(c=='a' && startLocalSlave())runSession(true);
    if(c=='b')runSession(true);
  }
  delay(1);
}
