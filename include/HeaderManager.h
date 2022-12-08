#ifndef HEADERMANAGER_H
#define HEADERMANAGER_H

#include <vector>
#include <cstdint>

// Manages the header's first 4 bytes according to the following map:
//  01 2 3 7 8     15 16      31
// +--+-+----+-------+----------+
// |Ty|T|Win|  Seq   |  Length  |
// |pe|R|Dow|  Num   |          |
// +---+-----+-----+------------+
// Each of these sections of bits have accompanying getters and setters.

class HeaderManager {
 private:
  const int HEADER_SZ = 4;
  const int TYPE_START = 0;
  const int SEQ_START = 1;
  const int LENGTH_START = 2;
  const int LENGTH_END = LENGTH_START + 1;
  std::vector<uint8_t> header = std::vector<uint8_t>(HEADER_SZ);
 public:

  // default constructor, initializes all header values to zero.
  HeaderManager();

  // Returns the entire header vector.
  std::vector<uint8_t> getHeader();

  /**
   * @brief Get this header's type.
   * 1 = PTYPE_DATA
   * 2 = PTYPE_ACK
   * 3 = PTYPE_NACK
   * 0 or any other number than the above means that this packet will be
   * ignored.
   * 
   * @return int 
   */
  int getType();

  // Sets the header's type field to the specified type.
  // if the provided type is not between 0..3, the value is set to 0
  void setType(int type);

  // Returns the value of the header's TR.
  int getTR();

  /**
   * @brief Has this packet been truncated?
   * 
   * @return true if the packet has been truncated (TR = 1)
   * @return false otherwise;
   */
  bool isTrunc()

  // Sets the header's TR field to the specified value.
  // If the provided TR value is not 0 or 1, the value is set to 0.
  void setTR(int tr);

  // Returns the value of the header's window
  int getWindow();

  // Sets the header's window field to the specified value.
  // If the provided window value is not between 0..31, the value is set to 0.
  void setWindow(int window);

  // Returns the value of the header's Seq num.
  int getSeqNum();

  // Sets the header's Seq num field to the specified value.
  // If the provided Seq num value is not between 0..255, the value is set to 0.
  void setSeqNum(int seqNum);

  // Returns the value of the header's length.
  int getLength();

  // Sets the header's length field to the specified value.
  // If the provided length value is not between 0..512, the value is set to 0.
  void setLength(int length);
};

#endif // HEADERMANAGER_H
