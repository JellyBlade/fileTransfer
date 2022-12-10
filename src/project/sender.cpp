#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>

#include "HeaderManager.h"
#include "PayloadManager.h"
#include "CRCManager.h"
#include "FileManager.h"
#include "TimestampManager.h"
#include "PacketManager.h"

// provide hostname and port value as command line arguments
// Mess up with these values and the socket call will likely fail
// argv[0] is the executable name
int main(int argc, char *argv[]) {
  int sock, rval, byte_count, opt;
  struct addrinfo hints, *results, *ptr;
  FileManager f;
  std::vector<PacketManager> packets;
  bool fileFlag = false;
  std::string fileName = "";

  while ((opt = getopt(argc, argv, "f:")) != -1) {
    switch (opt) {
      case 'f':
        fileFlag = true;
        fileName = optarg;
        break;
      case ':':
        std::cout << "Usage: " << argv[0] << " [-f filename] destination_host destination_port" << std::endl;
        return 1;
      case '?':
        // Handle unknown option.
        std::cout << "Usage: " << argv[0] << " [-f filename] destination_host destination_port" << std::endl;
        return 1;
    }
  }

  // Access the required arguments using optind.
  if (!(optind < argc)) {
    std::cout << "Usage: " << argv[0] << " [-f filename] destination_host destination_port" << std::endl;
    return 1;
  } else if (!(optind + 1 < argc)) {
    std::cout << "Usage: " << argv[0] << " [-f filename] destination_host destination_port" << std::endl;
    return 1;
  }
  
  // if we obtain the data from getaddrinfo, we might as well use it to open the socket

  // first, we prepare hints by clearing all of its fields

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;  // we ask for both IPv4 and IPv6
  hints.ai_socktype = SOCK_DGRAM;
  
  if ((rval = getaddrinfo(argv[optind], argv[optind+1], &hints, &results)) != 0) { // error
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

  if (fileFlag) {
    f.read(fileName);
    for (int i = 0; i < f.getContents().size(); i++) {
      HeaderManager h;
      CRCManager crc1;
      CRCManager crc2;
      PacketManager pm;
      TimestampManager t;
      PayloadManager p = f.getContents()[i];

      h.setType(1);
      h.setLength(p.get().size());
      h.setSeqNum(i);
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
      packets.push_back(pm);
    }
  } else { // TODO: split message into multiple payloads
    std::cout << "Enter a message: ";
    std::string input;
    std::getline(std::cin, input);
    for (char c : input) {
      //p.add(static_cast<uint8_t>(c));
      std::cout << "OOGA 1";
    }
    std::cout << "OOGA 2";
  }

for (int i = 0; i < packets.size(); i++) {
  int sent = sendto(sock, packets[i].getPacket().data(), packets[i].getPacket().size(), 0, ptr->ai_addr, ptr->ai_addrlen);
    if (sent == -1) {
      std::cout << std::strerror(errno) << std::endl;
    }
    std::cout << sent << std::endl;
}

  close(sock);
}
