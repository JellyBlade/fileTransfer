#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <cstdint>
#include <vector>

#include "FileManager.h"
#include "PacketManager.h"

// provide hostname and port value as command line arguments
// Mess up with these values and the socket call will likely fail
// argv[0] is the executable name
int main(int argc, char *argv[]) {
  int sock, rval, byte_count, opt;
  struct addrinfo hints, *results, *ptr;
  socklen_t fromlen;
  struct sockaddr_storage fromaddr;
  std::vector<uint8_t> packet(528);
  FileManager f;
  bool fileFlag = false;
  std::string fileName = "";

 while ((opt = getopt(argc, argv, "f:")) != -1) {
    switch (opt) {
      case 'f':
        fileFlag = true;
        fileName = optarg;
        break;
      case ':':
        std::cout << "Usage: " << argv[0] << " [-f filename] receive_port" << std::endl;
        return 1;
      case '?':
        // Handle unknown option.
        std::cout << "Usage: " << argv[0] << " [-f filename] receive_port" << std::endl;
        return 1;
    }
  }

  // Access the required arguments using optind.
  if (!(optind < argc)) {
    std::cout << "Usage: " << argv[0] << " [-f filename] receive_port" << std::endl;
    return 1;
  }
  
  // if we obtain the data from getaddrinfo, we might as well use it to open the socket

  // first, we prepare hints by clearing all of its fields

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;  // we ask for both IPv4 and IPv6
  hints.ai_socktype = SOCK_DGRAM;
  
  if ((rval = getaddrinfo("localhost", argv[optind], &hints, &results)) != 0) { // error
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
  pm.rebuildPacket();
  
  if (pm.isTrunc()) {
    std::cout << "NOTACK" << std::endl;
  }
  if (pm.getHeader().getType() > 3 || pm.getHeader().getType() < 1) {
    std::cout << "Packet ignored" << std::endl;
  }
  CRCManager ch(pm.getHeader());
  if (!(pm.getCRC(1) == ch)) {
    std::cout << "Header Checksum failed!" << std::endl;
    std::cout << pm.getCRC(1).getCRC() << std::endl;
    std::cout << ch.getCRC() << std::endl;
  }
  CRCManager cp(pm.getPayload());
  if (!(pm.getCRC(2) == cp)) {
    std::cout << "Payload checksum failed!" << std::endl;
    std::cout << pm.getCRC(2).getCRC() << std::endl;
    std::cout << cp.getCRC() << std::endl;
  }
  if (fileFlag) {
    std::vector<PayloadManager> payloads;
    payloads.push_back(pm.getPayload());
    f.setContents(payloads);
    f.write(fileName);
  } else {
    for (auto c : pm.getPayload().get()) {
      std::cout << c;
    }
    std::cout << std::endl;
  }
  
  close(sock);
}
