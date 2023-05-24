#pragma once

#include <cstdint>
#include <classfile/class_data_t.h>
#include <memory>
#include <vector>
namespace instructions {
using classfile::u1;
class BytecodeReader {
  private:
  std::shared_ptr<std::vector<u1>> code;
  int32_t pc;
  public:
  BytecodeReader() : code(nullptr), pc(0) {}
  void reset(std::shared_ptr<std::vector<u1>> _code, int32_t _pc) {
    code = _code;
    pc = _pc;
  }
  int32_t currentPc() {
    return pc;
  }
  uint8_t readUInt8() {
    return code->at(pc++);
  }
  int8_t readInt8() {
    return int8_t(readUInt8());
  }
  uint16_t readUInt16() {
    uint16_t b1 = uint16_t(readUInt8());
    uint16_t b2 = uint16_t(readUInt8());
    return (b1 << 8) | b2;
  }
  int16_t readInt16() {
    return int16_t(readUInt16());
  }
  int32_t readInt32() {
    int32_t b1 = int32_t(readUInt8());
    int32_t b2 = int32_t(readUInt8());
    int32_t b3 = int32_t(readUInt8());
    int32_t b4 = int32_t(readUInt8());
    return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
  }

  void readInt32s(std::vector<int32_t>& v, int32_t n) {
    for (int i = 0; i < n; i++) {
      v.push_back(readInt32());
    }
  }
  void skipPadding() {
    while (pc % 4 != 0) {
      readUInt8();
    }
  }
};
}