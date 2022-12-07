#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include <cstdint>

#include "PayloadManager.h"

class FileManager {
 public:
  
  /**
   * @brief Construct a new File Manager with the given PayloadManager to
   * read/write data to/from.
   * 
   * @param p PayloadManager to use for reading data to or writing data from.
   */
  FileManager(PayloadManager p);

  /**
   * @brief Shorthand constructor that calls read or write with the given address.
   * 
   * @param p PayloadManager to use for writing/reading.
   * @param s File address to read from/write to
   * @param write if true, the file at s will be written to.
   */
  FileManager(PayloadManager p, std::string s, bool write = false);

  /**
   * @brief Reads the file located at s, adding the data to the PayloadManager.
   * 
   * @param s location of the file to read.
   */
  void read(std::string s);

  /**
   * @brief Writes to the file located at s, using the data from the PayloadManager.
   * 
   * @param s location of the file to write.
   */
  void write(std::string s);
 private:
  PayloadManager payload;
};

#endif // FILEMANAGER_H