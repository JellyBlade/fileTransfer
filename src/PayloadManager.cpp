#include <vector>
#include <cstdint>

#include "PayloadManager.h"

PayloadManager::PayloadManager() {
  size = 0;
}

PayloadManager::PayloadManager(int s) {
  size = s;
}

PayloadManager::PayloadManager(std::vector<uint8_t> p) {
  payload = p;
}

bool PayloadManager::add(uint8_t p) {
  if (payload.size() < size) {
    payload.push_back(p);
    return true;
  }
  return false;
}

std::vector<uint8_t> PayloadManager::get() {
  return payload;
}