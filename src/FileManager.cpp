#include <string>
#include <vector>
#include <cstdint>
#include <fstream>

#include "PayloadManager.h"
#include "FileManager.h"


FileManager::FileManager(std::string s, bool doWrite) {
  if (doWrite) {
    write(s);
  } else {
    read(s);
  }
}

void FileManager::read(std::string s) {
  std::ifstream infile(s);
  PayloadManager pm;
  uint8_t b;

  while (infile.read((char*)&b, sizeof(uint8_t))) {
    if (pm.get().size() == 512) {
      contents.push_back(pm);
      pm = PayloadManager();
    }
    pm.add(b);
  }
  contents.push_back(pm);
  infile.close();
}

void FileManager::write(std::string s) {
  std::ofstream outfile(s);

  for (auto pm : contents) {
    for (auto b : pm.get()) {
      outfile << b;
    }
  }
  outfile.close();
}

std::vector<PayloadManager> FileManager::getContents() {
  return contents;
}

void FileManager::setContents(std::vector<PayloadManager> payloads) {
  contents = payloads;
}