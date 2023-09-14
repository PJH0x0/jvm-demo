#pragma once

#include <cstdint>
#include <memory>
#include <runtime/frame.h>
#include <type_traits>
#include "bytecode_reader.h"
#include <glog/logging.h>
namespace instructions {
template<typename T>
T PopOperandStack(runtime::OperandStack& stack) {
  T value;
  if (std::is_same<T, int32_t>::value) {
    value = stack.PopInt();
  } else if (std::is_same<T, int64_t>::value) {
    value = stack.PopLong();
  } else if (std::is_same<T, float>::value) {
    value = stack.PopFloat();
  } else if (std::is_same<T, double>::value) {
    value = stack.PopDouble();
  } else {
    LOG(ERROR) << "PopOperandStack T not match int/long/float/double";
  }
  return value;
}
template<typename T>
void PushOperandStack(runtime::OperandStack& stack, T value) {
  if (std::is_same<T, int32_t>::value) {
    stack.PushInt(value);
  } else if (std::is_same<T, int64_t>::value) {
    stack.PushLong(value);
  } else if (std::is_same<T, float>::value) {
    stack.PushFloat(value);
  } else if (std::is_same<T, double>::value) {
    stack.PushDouble(value);
  } else {
    LOG(ERROR) << "PushOperandStack T not match int/long/float/double";
  }
}

class Instruction {
  public:
  virtual void FetchOperands(std::shared_ptr<BytecodeReader> reader) = 0;
  virtual void Execute(std::shared_ptr<runtime::Frame> frame) = 0;
};

class NoOperandsInstruction : public Instruction{
  public:
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override {

  }
};
class NopInstruction : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    //nop instruction do nothing
  }
};
class BranchInstruction : public Instruction {
  protected:
  int32_t offset_;
  int32_t current_pc_;
  void Branch(std::shared_ptr<runtime::Frame> frame) {
    current_pc_ = frame->GetThread()->GetPc();
    frame->SetNextPc(current_pc_ + offset_);
  }
  public:
  BranchInstruction() : offset_(0), current_pc_(0) {}
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override {
    //current_pc_ = reader->current_pc_();
    offset_ = int32_t(reader->ReadInt16());
  }
};
class Index8Instruction : public Instruction {
  protected:
  uint32_t index_;
  public:
  Index8Instruction() : index_(0) {}
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override {
    index_ = uint32_t(reader->ReadUnsignedInt8());
  }
  void SetIndex(uint32_t _index) {
    index_ = _index;
  }
  uint32_t GetIndex() const {
    return index_;
  }
};
class Index16Instruction : public Instruction {
  protected:
  uint32_t index_;
  public:
  Index16Instruction() : index_(0) {}
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override {
    index_ = uint32_t(reader->ReadUnsignedInt16());
  }
};
}