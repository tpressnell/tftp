#include <string>
#include <vector>

class TftpServer {
public:
  void start();

private:
  void listen();
  void send();
  std::string hash(std::string ip_address, std::string filename);
  std::vector<std::string> active_transfers;
};

void TftpServer::listen(){};

void TftpServer::send(){};

std::string TftpServer::hash(std::string ip_address, std::string filename) {
  return ip_address + filename;
};
