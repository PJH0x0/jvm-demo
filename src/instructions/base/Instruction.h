#pragma once

#include <cstdint>
#include <memory>
#include <rtda/Frame.h>
#include <type_traits>
#include "BytecodeReader.h"
namespace instructions {
template<typename T>
T popOperandStack(rtda::OperandStack& stack) {
  T value;
  if (std::is_same<T, int32_t>::value) {
    value = stack.popInt();
  } else if (std::is_same<T, void*>::value) {
    value = stack.popRef();
  } else if (std::is_same<T, int64_t>::value) {
    value = stack.popLong();
  } else if (std::is_same<T, float>::value) {
    value = stack.popFloat();
  } else if (std::is_same<T, double>::value) {
    value = stack.popDouble();
  } else {
    LOG(ERROR) << "popOperandStack T not match int/long/float/double/ref";
  }
  return value;
}
template<typename T>
void pushOperandStack(rtda::OperandStack& stack, T value) {
  if (std::is_same<T, int32_t>::value) {
    stack.pushInt(value);
  } else if (std::is_same<T, void*>::value) {
    stack.pushRef(value);
  } else if (std::is_same<T, int64_t>::value) {
    stack.pushLong(value);
  } else if (std::is_same<T, float>::value) {
    stack.pushFloat(value);
  } else if (std::is_same<T, double>::value) {
    stack.pushDouble(value);
  } else {
    LOG(ERROR) << "pushOperandStack T not match int/long/float/double/ref";
  }
}

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
  int32_t currentPc;
  void branch() {
    currentPc += offset;
  }
  public:
  void fetchOperands(std::shared_ptr<BytecodeReader> reader) override {
    offset = int32_t(reader->readInt16());
    currentPc = reader->currentPc();
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