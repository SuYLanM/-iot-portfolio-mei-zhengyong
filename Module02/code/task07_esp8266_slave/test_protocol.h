#pragma once
#include <stdint.h>
#include <stddef.h>
// Test protocol only: no physical sensor quantity is encoded.
constexpr uint8_t TEST_ADDRESS=0x42, COMMAND=0xA5, PACKET_SIZE=12;
inline void encodePacket(uint8_t *p,uint8_t seq,uint32_t uptime,uint16_t requests) {
  p[0]='T';p[1]='7';p[2]=1;p[3]=seq;
  for(int i=0;i<4;++i)p[4+i]=uint8_t(uptime>>(8*i));
  p[8]=uint8_t(requests);p[9]=uint8_t(requests>>8);p[10]=0;
  p[11]=0;for(int i=0;i<11;++i)p[11]^=p[i];
}
inline bool validPacket(const uint8_t *p,size_t n,uint8_t seq) {
  if(n!=PACKET_SIZE || p[0]!='T' || p[1]!='7' || p[2]!=1 || p[3]!=seq || p[10]!=0)return false;
  uint8_t check=0;for(int i=0;i<11;++i)check^=p[i];return check==p[11];
}
inline uint32_t packetUptime(const uint8_t *p) {
  return uint32_t(p[4])|(uint32_t(p[5])<<8)|(uint32_t(p[6])<<16)|(uint32_t(p[7])<<24);
}
