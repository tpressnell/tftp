#include <iostream>
#include <string>
#include <vector>

class TftpPacket {
 public:
  virtual std::ostream& asStream(std::ostream& stream) = 0;
  static std::ostream& term(std::ostream& stream) {
    stream << 0x00;
    return stream;
  }
  enum Opcode : short { ReadRequest = 1, WriteRequest, Data, Ack, Error };
};

class TftpPacketFactory {
 public:
  static TftpPacket decodePacket(void* data, int data_length);
  static int encodePacket(void* data, int data_length);
};

class ReadRequest : TftpPacket {
 public:
  ReadRequest(std::string _filename, std::string _mode) {
    filename = _filename;
    mode = _mode;
  };
  const short opcode = Opcode::ReadRequest;
  std::string filename;
  std::string mode;
  std::ostream& asStream(std::ostream& stream) {
    stream << opcode << filename << term << mode << term;
    return stream;
  };
};

class WriteRequest : TftpPacket {
 public:
  WriteRequest(std::string _filename, std::string _mode) {
    filename = _filename;
    mode = _mode;
  };
  const short opcode = Opcode::WriteRequest;
  std::string filename;
  std::string mode;
  std::ostream& asStream(std::ostream& stream) {
    stream << opcode << filename << term << mode << term;
    return stream;
  };
};

class Data : TftpPacket {
 public:
  Data(short _blockNumber, char* _data, int _dataLength) {
    blockNumber = _blockNumber;
    data = _data;
    dataLength = _dataLength;
  }
  const short opcode = Opcode::Data;
  short blockNumber;
  char* data;
  int dataLength;
  std::ostream& asStream(std::ostream& stream) {
    stream << opcode << blockNumber;
    for (int i = 0; i < dataLength; i++) {
      stream << data[i];
    }
    return stream;
  };
};

class Ack : TftpPacket {
 public:
  Ack(short _blockNumber) { blockNumber = _blockNumber; }
  const short opcode = Opcode::Ack;
  short blockNumber;
  std::ostream& asStream(std::ostream& stream) {
    stream << opcode << blockNumber;
    return stream;
  };
};

class Error : TftpPacket {
 public:
  Error(short _errorCode, std::string _errorMsg) {
    errorCode = _errorCode;
    errorMsg = _errorMsg;
  }
  const short opcode = Opcode::Error;
  short errorCode;
  std::string errorMsg;
  std::ostream& asStream(std::ostream& stream) {
    stream << opcode << errorCode << errorMsg << term;
    return stream;
  };
};
