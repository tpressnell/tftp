#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>

#include "../common/TftpPacket.h"

class TftpServer {
 public:
  TftpServer();

 private:
  TftpPacket::Opcode get_opcode(void* received_message);
  const int BLOCK_SIZE = 4096;
};

TftpServer::TftpServer() {
  int status;
  struct addrinfo hints;
  struct addrinfo* servinfo;  // will point to the results

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  status = getaddrinfo(NULL, "10000", &hints, &servinfo);

  if (status != 0) {
    std::cerr << "gai error: %s\n" << gai_strerror(status) << '\n';
    exit(1);
  }

  int sock =
      socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

  if (sock == -1) {
    std::cerr << "Failed to get socket descriptor\n";
    exit(1);
  }

  sockaddr_in* ipv4 = (struct sockaddr_in*)servinfo->ai_addr;
  void* addr = &(ipv4->sin_addr);
  char ipstr[INET_ADDRSTRLEN];
  inet_ntop(servinfo->ai_family, addr, ipstr, sizeof ipstr);

  int bound = bind(sock, servinfo->ai_addr, servinfo->ai_addrlen);
  if (bound == -1) {
    std::cerr << "Failed to bind to socket\n";
    std::cerr << errno << '\n';
    exit(1);
  }

  struct sockaddr_in their_addr;
  socklen_t addr_len = sizeof their_addr;

  std::cout << "Listening on " << ipstr << ":10000" << std::endl;
  while (true) {
    void* recv_buffer = malloc(BLOCK_SIZE);
    int bytes_received = recvfrom(sock, recv_buffer, BLOCK_SIZE, 0,
                                  (struct sockaddr*)&their_addr, &addr_len);
    std::cout << "Bytes received: " << bytes_received << std::endl;
    std::cout << "Opcode received: "
              << TftpPacket::opcode_to_string(get_opcode(recv_buffer))
              << std::endl;

    free(recv_buffer);
  }

  freeaddrinfo(servinfo);
}

TftpPacket::Opcode TftpServer::get_opcode(void* received_message) {
  char* tmp = static_cast<char*>(received_message);
  short result = 0;
  result = result + *tmp;
  ++tmp;
  result = result + *tmp;
  std::cout << "Short returned: " << result << std::endl;
  return static_cast<TftpPacket::Opcode>(result);
}
