#ifndef PACKETMANAGER_H
#define PACKETMANAGER_H

#include <vector>
#include <cstdint>

#include "HeaderManager.h"
#include "TimestampManager.h"
#include "CRCManager.h"
#include "PayloadManager.h"

/**
 * @brief Manages the packet structure and data.
 * 
 */
class PacketManager {
 public:
  
  /**
   * @brief Construct a new Packet Manager object
   * 
   */
  PacketManager() {};

  /**
   * @brief Set the header of the packet
   * 
   * @param h HeaderManager to use for the packet header.
   */
  void setHeader(HeaderManager h);

  /**
   * @brief Returns the HeaderManager of the packet
   * 
   * @return HeaderManager 
   */
  HeaderManager getHeader();

  /**
   * @brief Set the Timestamp of the packet
   * 
   * @param t TimestampManager to use for the packet timestamp
   */
  void setTimestamp(TimestampManager t);

  /**
   * @brief Get the TimestampManager of the packet.
   * 
   * @return TimestampManager 
   */
  TimestampManager getTimestamp();

  /**
   * @brief Sets either the CRC1 or CRC2 of the packet to the given CRCManager
   * 
   * @param c CRCManager to use for one the packet CRC fields
   * @param crcNum 1 for CRC1, 2 for CRC2. Other numbers will default to CRC1.
   */
  void setCRC(CRCManager c, int crcNum = 1);

  /**
   * @brief Returns the CRCManager for either the CRC1 or CRC2 fields of the packet.
   * 
   * @param crcNum 1 for CRC1, 2 for CRC2. Other numbers will default to CRC1.
   * @return CRCManager 
   */
  CRCManager getCRC(int crcNum = 1);

  /**
   * @brief Set the PayloadManager for the packet's data.
   * 
   * @param p PayloadManager to use for this packet's data.
   */
  void setPayload(PayloadManager p);

  /**
   * @brief Get the PayloadManager for this packet's data.
   * 
   * @return PayloadManager 
   */
  PayloadManager getPayload();

  /**
   * @brief Collate the raw data in a uint8_t vector from each manager of this 
   * packet for transmission. 
   * 
   */
  void createPacket();

  /**
   * @brief Returns the constructed packet, ready for transmission.
   * 
   * @return std::vector<uint8_t> 
   */
  std::vector<uint8_t> getPacket();

  /**
   * @brief Recreate the packet from the given vector of uint8_t's, creating
   * the appropriate managers for each section of the packet's data. For use
   * by the receiver.
   * 
   */
  void setPacket(std::vector<uint8_t>);

  /**
   * @brief Gets the type of this packet. 
   * 1 = PTYPE_DATA
   * 2 = PTYPE_ACK
   * 3 = PTYPE_NACK
   * Any other number and this packet should be ignored.
   * 
   * @return int 
   */
  int getPacketType();

  /**
   * @brief Has this packet's data been truncated by the network?
   * 
   * @return true if this packet's header TR field = 1, and thus has no payload.
   * @return false otherwise
   */
  bool isTrunc();

  /**
   * @brief Returns the sequence number of this packet.
   * 
   * @return int sequence number
   */
  int getSeq();

  /**
   * @brief Get the value of this packet header's window field that denotes the
   * receiver's available buffer size in terms of segments.
   * 
   * @return int 
   */
  int getWindow();

  /**
   * @brief Get the value of this packet header's length field, between 0-512,
   * that denotes the number of bytes in the untruncated payload.
   * 
   * @return int 
   */
  int getLength();

 private:
  const int PTYPE_DATA = 1;
  const int PTYPE_ACK = 2;
  const int PTYPE_NACK = 3;
  HeaderManager header;
  TimestampManager timestamp;
  CRCManager crc1;
  PayloadManager payload;
  CRCManager crc2;
  std::vector<uint8_t> packet;
};

#endif // PACKETMANAGER_H