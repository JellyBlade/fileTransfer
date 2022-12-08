#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include <cstdint>

#include "PayloadManager.h"

class FileManager {
 public:
  
  /**
   * @brief Construct a new File Manager to read/write data to/from a file.
   * 
   */
  FileManager() {};

  /**
   * @brief Shorthand constructor that calls read or write with the given address.
   * 
   * @param s File address to read from/write to
   * @param write if true, the file at s will be written to.
   */
  FileManager(std::string s, bool doWrite = false);

  /**
   * @brief Reads the file located at s, creating a number of PayloadManagers
   * to store the contents of the file.
   * 
   * @param s location of the file to read.
   */
  void read(std::string s);

  /**
   * @brief Writes to the file located at s, using the data all of the
   * PayloadManagers provided by setContents().
   * 
   * @param s location of the file to write.
   */
  void write(std::string s);

  /**
   * @brief Get the contents of the file read.
   * 
   * @return std::vector<PayloadManager> 
   */
  std::vector<PayloadManager> getContents();

  /**
   * @brief Set the contents of the file to be written.
   * 
   */
  void setContents(std::vector<PayloadManager> payloads);
 private:
  std::vector<PayloadManager> contents;
};

#endif // FILEMANAGER_H