#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <chrono>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

#include "../common/TftpPacket.h"

class TftpClient {
 public:
  TftpClient();

 private:
  const int STARTING_LENGTH = 0;
  const int NO_FLAGS = 0;
};

TftpClient::TftpClient() {
  struct addrinfo hints;
  struct addrinfo* server_info;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  int got_address_info = getaddrinfo(NULL, "10000", &hints, &server_info);

  if (got_address_info != 0) {
    std::cerr << "gai error: " << gai_strerror(got_address_info) << std::endl;
    exit(1);
  }

  int server_socket = socket(server_info->ai_family, server_info->ai_socktype,
                    server_info->ai_protocol);

  if (server_socket == -1) {
    std::cerr << "Failed to get socket descriptor" << std::endl;
    exit(1);
  }

  sockaddr_in* ipv4 = (struct sockaddr_in*)server_info->ai_addr;
  void* addr = &(ipv4->sin_addr);
  char ipstr[INET_ADDRSTRLEN];
  inet_ntop(server_info->ai_family, addr, ipstr, sizeof ipstr);

  while (true) {
    std::cout << "Sending data to " << ipstr << ":10000" << std::endl;
    int length = STARTING_LENGTH;
    ReadRequest r = ReadRequest("Test", "binary");
    void* to_send = r.serialize(length);
    int bytes_sent = sendto(server_socket, to_send, length, NO_FLAGS,
                            server_info->ai_addr, server_info->ai_addrlen);
    std::cout << "Bytes sent: " << bytes_sent << std::endl;
    if (bytes_sent == -1) {
      std::cerr << "Failed to send" << std::endl;
      std::cerr << errno << std::endl;
      exit(1);
    }
    free(to_send);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }

  freeaddrinfo(server_info);
};
