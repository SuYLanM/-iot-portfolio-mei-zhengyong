#pragma once
#include "test_protocol.h"
// Only enabled by serial command a, after physical GPIO32->21 and GPIO33->22 wiring.
constexpr int LOCAL_SDA=32,LOCAL_SCL=33;
portMUX_TYPE packetMux=portMUX_INITIALIZER_UNLOCKED;
uint8_t localPacket[PACKET_SIZE];
uint16_t acceptedRequests=0;
bool localEnabled=false;
void receiveLocal(int length) {
  uint8_t bytes[2]={0,0};int consumed=0;
  while(Wire1.available() && consumed<64) {
    int v=Wire1.read();if(consumed<2)bytes[consumed]=uint8_t(v);++consumed;
  }
  if(length!=2 || consumed!=2 || bytes[0]!=COMMAND)return;
  uint8_t next[PACKET_SIZE];encodePacket(next,bytes[1],millis(),++acceptedRequests);
  portENTER_CRITICAL(&packetMux);memcpy(localPacket,next,PACKET_SIZE);portEXIT_CRITICAL(&packetMux);
}
void requestLocal() {
  uint8_t copy[PACKET_SIZE];
  portENTER_CRITICAL(&packetMux);memcpy(copy,localPacket,PACKET_SIZE);portEXIT_CRITICAL(&packetMux);
  Wire1.write(copy,PACKET_SIZE);
}
bool startLocalSlave() {
  if(localEnabled)return true;
  encodePacket(localPacket,0,millis(),0);
  Wire1.onReceive(receiveLocal);Wire1.onRequest(requestLocal);
  localEnabled=Wire1.begin(uint8_t(TEST_ADDRESS),LOCAL_SDA,LOCAL_SCL,100000);
  if(localEnabled)Wire1.slaveWrite(localPacket,PACKET_SIZE); // classic ESP32 preloaded first response
  Serial.printf("LOCAL_SLAVE init_ok=%d candidate_address=0x42 SDA=32 SCL=33 physical_jumpers_required=1\n",localEnabled);
  return localEnabled;
}
