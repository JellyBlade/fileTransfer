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
  std::vector<uint8_t> crc1Data = crc1.getCRC();
  std::vector<uint8_t> crc2Data = crc2.getCRC();
  std::vector<uint8_t> payloadData = payload.get();

  packet.push_back(headerData.begin(), headerData.end());
  packet.push_back(timestampData.begin(), timestampData.end());
  packet.push_back(crc1Data.begin(), crc1Data.end());
  packet.push_back(payloadData.begin(), payloadData.end());
  packet.push_back(crc2Data.begin(), crc2Data.end());
}

std::vector<uint8_t> PacketManager::getPacket() {
  return packet;
}

void PacketManager::setPacket(std::vector<uint8_t> p) {
  packet.clear();
  packet = p;
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