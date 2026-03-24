#include <cstring>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

class TftpServer {
public:
  TftpServer();

private:
  void handle();
  bool ready = false;
  std::string hash(std::string ip_address, std::string filename);
  std::vector<std::string> active_transfers;
};

TftpServer::TftpServer() {
  int status;
  struct addrinfo hints;
  struct addrinfo *servinfo; // will point to the results

  memset(&hints, 0, sizeof hints); // make sure the struct is empty
  hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE; // fill in my IP for me
  status = getaddrinfo(NULL, "69", &hints, &servinfo);

  if (status != 0) {
    std::cerr << "gai error: %s\n" << gai_strerror(status) << '\n';
    exit(1);
  }

  int sock =
      socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

  if (sock == -1) {
    std::cerr << "Failed to get socket descriptor";
    exit(1);
  }

  int bound = bind(sock, servinfo->ai_addr, servinfo->ai_addrlen);

  if (bound == -1) {
    std::cerr << "Failed to bind to socket";
    exit(1);
  }

  int listening = listen(sock, 10);

  if (listening == -1) {
    std::cerr << "Failed to start listening to socket";
    exit(1);
  }

  struct sockaddr_storage their_addr;
  socklen_t addr_size = sizeof their_addr;

  int their_sock = accept(sock, (struct sockaddr *)&their_addr, &addr_size);

  if (their_sock == -1) {
    std::cerr << "Failed to accept connection";
  }

  freeaddrinfo(servinfo);
}

void TftpServer::handle(){};

std::string TftpServer::hash(std::string ip_address, std::string filename) {
  return ip_address + filename;
};
