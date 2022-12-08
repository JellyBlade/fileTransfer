#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <cstdint>
#include <vector>

#include "PacketManager.h"

// provide hostname and port value as command line arguments
// Mess up with these values and the socket call will likely fail
// argv[0] is the executable name
int main(int argc, char *argv[]) {
  int sock, rval, byte_count;
  struct addrinfo hints, *results, *ptr;
  socklen_t fromlen;
  struct sockaddr_storage fromaddr;
  std::vector<uint8_t> packet(512);

  // quick check if we preovide the right arguments
  if (argc != 2) {
    std::cout << "Usage " << argv[0] << " receive_port" << std::endl;
    return 1; // terminate
  }
  
  // if we obtain the data from getaddrinfo, we might as well use it to open the socket

  // first, we prepare hints by clearing all of its fields

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;  // we ask for both IPv4 and IPv6
  hints.ai_socktype = SOCK_DGRAM;
  
  if ((rval = getaddrinfo("localhost", argv[1], &hints, &results)) != 0) { // error
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

  if (bind(sock, ptr->ai_addr, ptr->ai_addrlen) == -1) {
    std::cerr << "Cannot bind the socket" << std::endl;
    return 4;
  }
  // initialize fromlen to prevent EINVAL from being thrown by sendto()
  fromlen = sizeof(fromaddr);

  byte_count = recvfrom(sock, packet.data(), packet.size(), 0, (sockaddr*)(&fromaddr), &fromlen);
  if (byte_count == -1) {
    std::cout << "Error occurred" << std::endl;
    std::cout << std::strerror(errno) << std::endl;
  }
  std::cout << "Received " << byte_count << " bytes" << std::endl;
  packet.resize(byte_count);
  
  PacketManager pm;
  pm.setPacket(packet);
  for (auto c : packet) {
    std::cout << c;
  }
  std::cout << std::endl;
  close(sock);
}
