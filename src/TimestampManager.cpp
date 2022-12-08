#include <ctime>
#include <vector>
#include <cstdint>

#include "TimestampManager.h"

TimestampManager::TimestampManager() {
  set();
}

TimestampManager::TimestampManager(std::vector<uint8_t> t) {
  timestamp = t;
}

void TimestampManager::set(int unix) {
  timestamp.push_back(static_cast<uint8_t>((unix >> 24)));
  timestamp.push_back(static_cast<uint8_t>((unix >> 16) & 0xff));
  timestamp.push_back(static_cast<uint8_t>((unix >> 8)));
  timestamp.push_back(static_cast<uint8_t>(unix & 0xff));
}

void TimestampManager::set() {
  time_t current_time = std::time(NULL);
  set((int)current_time);
}

std::vector<uint8_t> TimestampManager::getBytes() {
  return timestamp;
}

int TimestampManager::getUNIX() {
  int timeCom = (timestamp[0] << 24) | (timestamp[1] << 16) | (timestamp[2] << 8) | (timestamp[3]);
  return timeCom;
}