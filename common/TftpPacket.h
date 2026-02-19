#include <string>
#include <vector>

class TftpPacket {
public:
  virtual std::vector<unsigned char> *serialize() = 0;
  unsigned const char TERM = 0x00;
};

class ReadRequest : TftpPacket {
public:
  unsigned const short opcode = 0x0001;
  std::string filename;
  std::string mode = "octet";
  std::vector<unsigned char> *serialize() {
    std::vector<unsigned char> *p = new std::vector<unsigned char>();
    p->push_back(opcode & 0xFF);
    p->push_back(opcode >> 8 & 0xFF);
    for (int i = 0; i < filename.size(); i++) {
      p->push_back(filename[i]);
    }
    p->push_back(TftpPacket::TERM);
    for (int i = 0; i < mode.size(); i++) {
      p->push_back(mode[i]);
    }
    p->push_back(TftpPacket::TERM);
    return p;
  };
};

class WriteRequest : TftpPacket {
public:
  unsigned const short opcode = 0x0002;
  std::string filename;
  std::string mode = "octet";
  std::vector<unsigned char> *serialize() {
    std::vector<unsigned char> *p = new std::vector<unsigned char>();
    p->push_back(opcode & 0xFF);
    p->push_back(opcode >> 8 & 0xFF);
    for (int i = 0; i < filename.size(); i++) {
      p->push_back(filename[i]);
    }
    p->push_back(TftpPacket::TERM);
    for (int i = 0; i < mode.size(); i++) {
      p->push_back(mode[i]);
    }
    p->push_back(TftpPacket::TERM);
    return p;
  };
};

class Data : TftpPacket {
public:
  unsigned const short opcode = 0x0003;
  unsigned short block_number;
  std::vector<unsigned char> data;
  std::vector<unsigned char> *serialize() {
    std::vector<unsigned char> *p = new std::vector<unsigned char>();
    p->push_back(opcode & 0xFF);
    p->push_back(opcode >> 8 & 0xFF);
    p->push_back(block_number & 0xFF);
    p->push_back(block_number >> 8 & 0xFF);
    for (unsigned char c : data) {
      p->push_back(c);
    }
    return p;
  };
};

class Ack : TftpPacket {
public:
  unsigned const short opcode = 0x0004;
  unsigned short block_number;
  std::vector<unsigned char> *serialize() {
    std::vector<unsigned char> *p = new std::vector<unsigned char>();
    p->push_back(opcode & 0xFF);
    p->push_back(opcode >> 8 & 0xFF);
    p->push_back(block_number & 0xFF);
    p->push_back(block_number >> 8 & 0xFF);
    return p;
  };
};

class Error : TftpPacket {
public:
  unsigned const short opcode = 0x0005;
  unsigned short error_code;
  std::string error_msg;
  std::vector<unsigned char> *serialize() {
    std::vector<unsigned char> *p = new std::vector<unsigned char>();
    p->push_back(opcode & 0xFF);
    p->push_back(opcode >> 8 & 0xFF);
    p->push_back(error_code & 0xFF);
    p->push_back(error_code >> 8 & 0xFF);
    for (int i = 0; i < error_msg.size(); i++) {
      p->push_back(error_msg[i]);
    }
    return p;
  };
};
