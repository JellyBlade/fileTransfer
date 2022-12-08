#include <zlib.h>
#include <vector>
#include <cstdint>

#include "CRCManager.h"
#include "HeaderManager.h"
#include "PayloadManager.h"

CRCManager::CRCManager() {
  crc = crc32(0L, NULL, O);
}

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
  crc = crc32(crc, reinterpret_cast<const Bytef*>(b.data()), b.size());
}

unsigned long CRCManager::getCRC() {
  return crc;
}

bool CRCManager::compare(CRCManager&c ) {
  return c.getCRC == crc;
}

bool CRCManager::compare(unsigned long c) {
  return c == crc;
}

bool CRCManager::operator==(const CRCManager& c) {
  return c.getCRC == crc;
}