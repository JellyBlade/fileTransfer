#include <vector>
#include <cstdint>

#include "HeaderManager.h"
#include "TimestampManager.h"
#include "CRCManager.h"
#include "PayloadManager.h"
#include "PacketManager.h"

void PacketManager::setHeader(HeaderManager h) {
  header = h;
}

HeaderManager PacketManager::getHeader() {
  return header;
}

void PacketManager::setTimestamp(TimestampManager t) {
  timestamp = t;
}

TimestampManager PacketManager::getTimestamp() {
  return timestamp;
}

void PacketManager::setCRC(CRCManager c, int crcNum) {
  if (crcNum == 2) {
    crc2 = c;
    return;
  }
  crc1 = c;
}

CRCManager PacketManager::getCRC(int crcNum) {
  if (crcNum == 2) {
    return crc2;
  }
  return crc1;
}

void PacketManager::setPayload(PayloadManager p) {
  payload = p;
}

PayloadManager PacketManager::getPayload() {
  return payload;
}

void PacketManager::createPacket() {
  packet.clear();

  std::vector<uint8_t> headerData = header.getHeader();
  std::vector<uint8_t> timestampData = timestamp.getBytes();
  std::vector<uint8_t> crc1Data = crc1.getCRCBytes();
  std::vector<uint8_t> crc2Data = crc2.getCRCBytes();
  std::vector<uint8_t> payloadData = payload.get();

  packet.insert(packet.end(), headerData.begin(), headerData.end());
  packet.insert(packet.end(), timestampData.begin(), timestampData.end());
  packet.insert(packet.end(), crc1Data.begin(), crc1Data.end());
  packet.insert(packet.end(), payloadData.begin(), payloadData.end());
  packet.insert(packet.end(), crc2Data.begin(), crc2Data.end());
}

std::vector<uint8_t> PacketManager::getPacket() {
  return packet;
}

void PacketManager::setPacket(std::vector<uint8_t> p) {
  packet.clear();
  packet = p;
}

void PacketManager::rebuildPacket() {
  std::vector<uint8_t> headerData;
  std::vector<uint8_t> timestampData;
  std::vector<uint8_t> crc1Data;
  std::vector<uint8_t> crc2Data; 
  std::vector<uint8_t> payloadData;

  headerData.insert(headerData.begin(), packet.begin(), packet.begin()+4);
  timestampData.insert(timestampData.begin(), packet.begin()+4, packet.begin()+8);
  crc1Data.insert(crc1Data.begin(), packet.begin()+8, packet.begin()+12);
  crc2Data.insert(crc2Data.end(), std::prev(packet.end(), 4), packet.end());
  payloadData.insert(payloadData.begin(), packet.begin()+12, packet.end()-4);

  HeaderManager h(headerData);
  TimestampManager t(timestampData);
  CRCManager c1;
  CRCManager c2;
  PayloadManager p(payloadData);
  c1.setCRCBytes(crc1Data);
  c2.setCRCBytes(crc2Data);
  header = h;
  timestamp = t;
  crc1 = c1;
  crc2 = c2;
  payload = p;
}

int PacketManager::getPacketType() {
  return header.getType();
}

bool PacketManager::isTrunc() {
  return header.isTrunc();
}

int PacketManager::getSeq() {
  return header.getSeqNum();
}

int PacketManager::getWindow() {
  return header.getWindow();
}

int PacketManager::getLength() {
  return header.getLength();
}
