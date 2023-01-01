#pragma once

#include <cstdint>
#include <memory>
#include <rtda/Frame.h>
#include "BytecodeReader.h"
namespace instructions {
class Instruction {
  public:
  virtual void fetchOperands(std::shared_ptr<BytecodeReader> reader) = 0;
  virtual void execute(std::shared_ptr<rtda::Frame> frame) = 0;
};

class NoOperandsInstruction : public Instruction{
  public:
  void fetchOperands(std::shared_ptr<BytecodeReader> reader) override {

  }
};
class NopInstruction : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    //nop instruction do nothing
  }
};
class BranchInstruction : public Instruction {
  protected:
  int32_t offset;
  public:
  void fetchOperands(std::shared_ptr<BytecodeReader> reader) override {
    offset = int32_t(reader->readInt16());
  }
};
class Index8Instruction : public Instruction {
  protected:
  uint32_t index;
  public:
  void fetchOperands(std::shared_ptr<BytecodeReader> reader) override {
    index = uint32_t(reader->readUInt8());
  }
};
class Index16Instruction : public Instruction {
  protected:
  uint32_t index;
  public:
  void fetchOperands(std::shared_ptr<BytecodeReader> reader) override {
    index = uint32_t(reader->readUInt16());
  }
};
}