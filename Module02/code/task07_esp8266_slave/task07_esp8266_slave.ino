#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include "test_protocol.h"
#if !defined(ESP8266)
#error "Select ESP8266 LOLIN(WEMOS) D1 R2 & mini, Core 3.1.2."
#endif
uint8_t response[PACKET_SIZE];
uint16_t acceptedRequests=0;
volatile bool newRequest=false;
// size_t selects the Core 3.1.2 overload which enables software-slave interrupts.
void receiveCommand(size_t length) {
  uint8_t bytes[2]={0,0};size_t consumed=0;
  while(Wire.available() && consumed<64) { int v=Wire.read();if(consumed<2)bytes[consumed]=uint8_t(v);++consumed; }
  if(length==2 && consumed==2 && bytes[0]==COMMAND) {
    encodePacket(response,bytes[1],millis(),++acceptedRequests);newRequest=true;
  }
}
void requestData() { Wire.write(response,PACKET_SIZE); }
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);WiFi.forceSleepBegin();delay(1);
  encodePacket(response,0,millis(),0);
  Wire.begin(D2,D1,uint8_t(TEST_ADDRESS));
  Wire.onReceive(receiveCommand);Wire.onRequest(requestData);
  Serial.println("TASK7 ESP8266 SOFTWARE-SLAVE candidate_address=0x42 D2=GPIO4 SDA D1=GPIO5 SCL; no physical-sensor data");
}
void loop() {
  if(newRequest) { newRequest=false;Serial.printf("accepted_requests=%u\n",acceptedRequests); }
  delay(1);
}
