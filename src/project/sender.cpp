#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>

#include "HeaderManager.h"
#include "CRCManager.h"
#include "FileManager.h"
#include "TimestampManager.h"
#include "PayloadManager.h"
#include "PacketManager.h"

// provide hostname and port value as command line arguments
// Mess up with these values and the socket call will likely fail
// argv[0] is the executable name
int main(int argc, char *argv[]) {
  int sock, rval, byte_count;
  struct addrinfo hints, *results, *ptr;

  PacketManager pm;
  HeaderManager h;
  CRCManager crc1;
  CRCManager crc2;
  PayloadManager p = PayloadManager(512);
  TimestampManager t;

  // quick check if we provide the right arguments
  if (argc != 3) {
    std::cout << "Usage " << argv[0] << " destination_host destination_port" << std::endl;
    return 1; // terminate
  }
  
  // if we obtain the data from getaddrinfo, we might as well use it to open the socket

  // first, we prepare hints by clearing all of its fields

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;  // we ask for both IPv4 and IPv6
  hints.ai_socktype = SOCK_DGRAM;
  
  if ((rval = getaddrinfo(argv[1], argv[2], &hints, &results)) != 0) { // error
    std::cerr << "Error getting the destination address: " << gai_strerror(rval) << std::endl;
    return 2;
  }

  // loop through the results from the linked list
  for (ptr = results; ptr != NULL; ptr = ptr->ai_next) {  
    if ((sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) != -1) {
      break;  // socket successful, no need to search further
    }
  }

  // but we might reach the end of the list...
  if (ptr == NULL) {
    std::cerr << "Failed to open socket" << std::endl;
    return 3;
  }

  // if not, we are ready to send here
  // add the code to send "hello world" here
  for (char c : "Hello goobers") {
    p.add(static_cast<std::uint8_t>(c));
  }

  h.setType(1);
  h.setLength(p.get().size());
  h.setSeqNum(1);
  h.setTR(0);

  h.setWindow(1);
  crc1.generate(h.getHeader());
  crc2.generate(p.get());
  
  pm.setCRC(crc1);
  pm.setCRC(crc2, 2);
  pm.setHeader(h);
  pm.setPayload(p);
  pm.setTimestamp(t);
  
  pm.createPacket();

int sent = sendto(sock, pm.getPacket().data(), pm.getPacket().size(), 0, ptr->ai_addr, ptr->ai_addrlen);
  if (sent == -1) {
    std::cout << std::strerror(errno) << std::endl;
  }
  std::cout << sent << std::endl;

  close(sock);
}
