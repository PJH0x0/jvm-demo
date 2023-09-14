#pragma once

#include <cstdint>
#include <type_traits>
#include "base/base_insts.h"
#include <glog/logging.h>
#include <cmath>

namespace instructions {
using runtime::OperandStack;
using runtime::LocalVars;

template<typename T>
class ADD : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    T value1 = PopOperandStack<T>(stack);
    T value2 = PopOperandStack<T>(stack);
    T result = value2 + value1;
    PushOperandStack(stack, result);
  }
};
template<typename T>
class SUB : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    T value1 = PopOperandStack<T>(stack);
    T value2 = PopOperandStack<T>(stack);
    T result = value2 - value1;
    PushOperandStack(stack, result);
  }
};
template<typename T>
class MUL : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    T value1 = PopOperandStack<T>(stack);
    T value2 = PopOperandStack<T>(stack);
    T result = value2 * value1;
    PushOperandStack(stack, result);
  }
};
template<typename T>
class DIV : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    T value1 = PopOperandStack<T>(stack);
    T value2 = PopOperandStack<T>(stack);
    if (value1 == 0) {
      LOG(FATAL) << "ArithmeticException " << value2 << " / " << value1;
    }
    T result = value2 / value1;
    PushOperandStack(stack, result);
  }
};
template<typename T>
class IREM : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    T value1 = PopOperandStack<T>(stack);
    T value2 = PopOperandStack<T>(stack);
    if (value1 == 0) {
      LOG(FATAL) << "ArithmeticException " << value2 << " / " << value1;
    }
    T result = value2 % value1;
    PushOperandStack(stack, result);
  }
};
template<typename T>
class FREM : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    T value1 = PopOperandStack<T>(stack);
    T value2 = PopOperandStack<T>(stack);
    if (value1 == 0) {
      LOG(FATAL) << "ArithmeticException " << value2 << " / " << value1;
    }
    T result = std::fmod(value2, value1);
    PushOperandStack(stack, result);
  }
};
template<typename T>
class NEG : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    T value = PopOperandStack<T>(stack);
    PushOperandStack(stack, -value);
  }
};
template<typename T>
class SHL : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    int32_t value1 = PopOperandStack<int32_t>(stack);
    T value2 = PopOperandStack<T>(stack);
    uint32_t shift = uint32_t(value1);
    T result = value2 << shift;
    PushOperandStack(stack, result);
  }
};
template<typename T>
class SHR : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    int32_t value1 = PopOperandStack<int32_t>(stack);
    T value2 = PopOperandStack<T>(stack);
    uint32_t shift = uint32_t(value1);
    T result = value2 >> shift;
    PushOperandStack(stack, result);
  }
};
template<typename T>
class USHR : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    LOG(FATAL) << "kUnknown type to ushr";
  }
};

template<>
class USHR<int32_t> : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    int32_t value1 = PopOperandStack<int32_t>(stack);
    int32_t value2 = PopOperandStack<int32_t>(stack);
    uint32_t shift = uint32_t(value1);
    int32_t result = int32_t(uint32_t(value2) >> shift);
    PushOperandStack(stack, result);
  }
};

template<>
class USHR<int64_t> : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    int32_t value1 = PopOperandStack<int32_t>(stack);
    int64_t value2 = PopOperandStack<int64_t>(stack);
    uint32_t shift = uint32_t(value1);
    int64_t result = int64_t(uint64_t(value2) >> shift);
    PushOperandStack(stack, result);
  }
};
template<typename T>
class AND : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    T value1 = PopOperandStack<T>(stack);
    T value2 = PopOperandStack<T>(stack);
    T result = value1 & value2;
    PushOperandStack(stack, result);
  }
};
template<typename T>
class OR : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    T value1 = PopOperandStack<T>(stack);
    T value2 = PopOperandStack<T>(stack);
    T result = value1 | value2;
    PushOperandStack(stack, result);
  }
};
template<typename T>
class XOR : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    T value1 = PopOperandStack<T>(stack);
    T value2 = PopOperandStack<T>(stack);
    T result = value1 ^ value2;
    PushOperandStack(stack, result);
  }
};
class IINC : public Instruction {
  private:
  uint32_t index_;
  int32_t const_;
  public:
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override {
    index_ = uint32_t(reader->ReadUnsignedInt8());
    const_ = int32_t(reader->ReadInt8());
  }
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    LocalVars& vars = frame->GetLocalVars();
    int32_t val = vars.GetInt(index_);
    val += const_;
    vars.SetInt(index_, val);
  }
  void SetIndex(uint32_t _index) {
    index_ = _index;
  }
  uint32_t GetIndex() const {
    return index_;
  }
  void SetConst(int32_t constant) {
    const_ = constant;
  }
  int32_t GetConst() const {
    return const_;
  }
};
}