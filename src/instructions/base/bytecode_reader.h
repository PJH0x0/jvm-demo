#pragma once

#include <cstdint>
#include <classfile/class_data_t.h>
#include <memory>
#include <vector>
namespace instructions {
using classfile::u1;
class BytecodeReader {
  private:
  const std::vector<u1>* code_;
  int32_t pc_;
  public:
  BytecodeReader() : code_(nullptr), pc_(0) {}
  void reset(const std::vector<u1>* codes, int32_t pc) {
    code_ = codes;
    pc_ = pc;
  }
  int32_t CurrentPc() const {
    return pc_;
  }
  uint8_t ReadUnsignedInt8() {
    return code_->at(pc_++);
  }
  int8_t ReadInt8() {
    return int8_t(ReadUnsignedInt8());
  }
  uint16_t ReadUnsignedInt16() {
    auto b1 = uint16_t(ReadUnsignedInt8());
    auto b2 = uint16_t(ReadUnsignedInt8());
    return (b1 << 8) | b2;
  }
  int16_t ReadInt16() {
    return int16_t(ReadUnsignedInt16());
  }
  int32_t readInt32() {
    auto b1 = int32_t(ReadUnsignedInt8());
    auto b2 = int32_t(ReadUnsignedInt8());
    auto b3 = int32_t(ReadUnsignedInt8());
    auto b4 = int32_t(ReadUnsignedInt8());
    return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
  }

  void readInt32s(std::vector<int32_t>& v, int32_t n) {
    for (int i = 0; i < n; i++) {
      v.push_back(readInt32());
    }
  }
  void skipPadding() {
    while (pc_ % 4 != 0) {
      ReadUnsignedInt8();
    }
  }
};
}