#include <zlib.h>
#include <vector>
#include <cstdint>

#include <iostream>
#include <cstring>

#include "CRCManager.h"
#include "HeaderManager.h"
#include "PayloadManager.h"

CRCManager::CRCManager(HeaderManager h) {
  generate(h.getHeader());
}

CRCManager::CRCManager(std::vector<uint8_t> h) {
  generate(h);
}

CRCManager::CRCManager(PayloadManager p) {
  generate(p.get());
}

void CRCManager::generate(std::vector<uint8_t> b) {
  crc = crc32(0L, NULL, 0);
  crc = crc32(crc, reinterpret_cast<const Bytef*>(b.data()), b.size());
}

unsigned long CRCManager::getCRC() const {
  return crc;
}

void CRCManager::setCRCBytes(std::vector<uint8_t> c) {
  // have to mask the first 4 bytes as they get set to 0xFF for some reason
  crc = (c[0] << 24) | (c[1] << 16) | (c[2] << 8) | (c[3]) & 0x00000000FFFFFFFF;
}

std::vector<uint8_t> CRCManager::getCRCBytes() {
  std::vector<uint8_t> crcBytes;
  crcBytes.push_back(static_cast<uint8_t>((crc >> 24)));
  crcBytes.push_back(static_cast<uint8_t>((crc >> 16) & 0xff));
  crcBytes.push_back(static_cast<uint8_t>((crc >> 8)));
  crcBytes.push_back(static_cast<uint8_t>(crc & 0xff));
  return crcBytes;
}

bool CRCManager::compare(CRCManager&c ) {
  return c.getCRC() == crc;
}

bool CRCManager::compare(unsigned long c) {
  return c == crc;
}

bool CRCManager::operator==(const CRCManager& c) const {
  return crc == c.getCRC();
}
