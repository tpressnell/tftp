#include <cstring>
#include <iostream>
#include <string>

class TftpPacket {
 public:
  enum Opcode : short {
    ReadRequest = 0x0001,
    WriteRequest = 0x0002,
    Data = 0x0003,
    Ack = 0x0004,
    Error = 0x0005
  };
  static std::string opcode_to_string(Opcode o) {
    switch (o) {
      case ReadRequest:
        return "ReadRequest";
      case WriteRequest:
        return "WriteRequest";
      case Data:
        return "Data";
      case Ack:
        return "Ack";
      case Error:
        return "Error";
    }
    return "Unknown";
  };

  static char* serialize_opcode(Opcode opcode, char* buffer) {
    *buffer = opcode >> 8 & 0xFF;
    ++buffer;
    *buffer = opcode & 0xFF;
    ++buffer;

    return buffer;
  }
};

class ReadRequest : TftpPacket {
 public:
  ReadRequest(std::string _filename, std::string _mode) {
    filename = _filename;
    mode = _mode;
  };
  const Opcode opcode = Opcode::ReadRequest;
  std::string filename;
  std::string mode;
  void* serialize(int& data_length) {
    data_length = sizeof(Opcode) + filename.length() + 1 + mode.length() + 1;
    void* data = malloc(data_length);
    char* tmp = reinterpret_cast<char*>(data);
    tmp = serialize_opcode(opcode, tmp);
    std::memcpy(tmp, filename.data(), filename.length() + 1);
    tmp += filename.length() + 1;
    std::memcpy(tmp, mode.data(), mode.length() + 1);

    return data;
  };
};

class WriteRequest : TftpPacket {
 public:
  WriteRequest(std::string _filename, std::string _mode) {
    filename = _filename;
    mode = _mode;
  };
  const Opcode opcode = Opcode::WriteRequest;
  std::string filename;
  std::string mode;
};

class Data : TftpPacket {
 public:
  Data(short _blockNumber, char* _data, int _dataLength) {
    blockNumber = _blockNumber;
    data = _data;
    dataLength = _dataLength;
  }
  const Opcode opcode = Opcode::Data;
  short blockNumber;
  char* data;
  int dataLength;
};

class Ack : TftpPacket {
 public:
  Ack(short _blockNumber) { blockNumber = _blockNumber; }
  const Opcode opcode = Opcode::Ack;
  short blockNumber;
};

class Error : TftpPacket {
 public:
  Error(short _errorCode, std::string _errorMsg) {
    errorCode = _errorCode;
    errorMsg = _errorMsg;
  }
  const Opcode opcode = Opcode::Error;
  short errorCode;
  std::string errorMsg;
};
