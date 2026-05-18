#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

class TftpServer {
 public:
  TftpServer();

 private:
  std::vector<uint32_t> active_transfers;
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
  std::cout << "IP address that I will bind to: " << ipstr << '\n';

  int bound = bind(sock, servinfo->ai_addr, servinfo->ai_addrlen);

  if (bound == -1) {
    std::cerr << "Failed to bind to socket\n";
    std::cerr << errno << '\n';
    exit(1);
  }

  struct sockaddr_storage their_addr;
  socklen_t addr_len = sizeof their_addr;

  while (true) {
    void* recv_buffer = malloc(4096);
    std::cout << "Waiting on recvfrom()\n";
    int bytes_received = recvfrom(sock, recv_buffer, 4096, 0,
                                  (struct sockaddr*)&their_addr, &addr_len);
    std::cout << "Bytes received: " << bytes_received << '\n';

    std::cout << "Echoing back.\n";
    int bytes_sent = sendto(sock, recv_buffer, bytes_received, 0,
                            (struct sockaddr*)&their_addr, addr_len);
    std::cout << "Bytes sent: " << bytes_sent << '\n';
    free(recv_buffer);
  }

  freeaddrinfo(servinfo);
}
