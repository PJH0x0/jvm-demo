#pragma once

#include <cstdint>
#include <type_traits>
#include "base/Instruction.h"
#include <glog/logging.h>
#include <cmath>

namespace instructions {

template<typename T>
class ADD : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    T value1 = popOperandStack<T>(stack);
    T value2 = popOperandStack<T>(stack);
    T result = value2 + value1;
    pushOperandStack(stack, result);
  }
};
template<typename T>
class SUB : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    T value1 = popOperandStack<T>(stack);
    T value2 = popOperandStack<T>(stack);
    T result = value2 - value1;
    pushOperandStack(stack, result);
  }
};
template<typename T>
class MUL : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    T value1 = popOperandStack<T>(stack);
    T value2 = popOperandStack<T>(stack);
    T result = value2 * value1;
    pushOperandStack(stack, result);
  }
};
template<typename T>
class DIV : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    T value1 = popOperandStack<T>(stack);
    T value2 = popOperandStack<T>(stack);
    if (value1 == 0) {
      LOG(FATAL) << "ArithmeticException " << value2 << " / " << value1;
    }
    T result = value2 / value1;
    pushOperandStack(stack, result);
  }
};
template<typename T>
class IREM : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    T value1 = popOperandStack<T>(stack);
    T value2 = popOperandStack<T>(stack);
    if (value1 == 0) {
      LOG(FATAL) << "ArithmeticException " << value2 << " / " << value1;
    }
    T result = value2 % value1;
    pushOperandStack(stack, result);
  }
};
template<typename T>
class FREM : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    T value1 = popOperandStack<T>(stack);
    T value2 = popOperandStack<T>(stack);
    if (value1 == 0) {
      LOG(FATAL) << "ArithmeticException " << value2 << " / " << value1;
    }
    T result = std::fmod(value2, value1);
    pushOperandStack(stack, result);
  }
};
template<typename T>
class NEG : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    T value = popOperandStack<T>(stack);
    pushOperandStack(stack, -value);
  }
};
template<typename T>
class SHL : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    int32_t value1 = popOperandStack<int32_t>(stack);
    T value2 = popOperandStack<T>(stack);
    uint32_t shift = uint32_t(value1);
    T result = value2 << shift;
    pushOperandStack(stack, result);
  }
};
template<typename T>
class SHR : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    int32_t value1 = popOperandStack<int32_t>(stack);
    T value2 = popOperandStack<T>(stack);
    uint32_t shift = uint32_t(value1);
    T result = value2 >> shift;
    pushOperandStack(stack, result);
  }
};
template<typename T>
class USHR : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    LOG(FATAL) << "UNKNOWN type to ushr";
  }
};

template<>
class USHR<int32_t> : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    int32_t value1 = popOperandStack<int32_t>(stack);
    int32_t value2 = popOperandStack<int32_t>(stack);
    uint32_t shift = uint32_t(value1);
    int32_t result = int32_t(uint32_t(value2) >> shift);
    pushOperandStack(stack, result);
  }
};

template<>
class USHR<int64_t> : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    int32_t value1 = popOperandStack<int32_t>(stack);
    int64_t value2 = popOperandStack<int64_t>(stack);
    uint32_t shift = uint32_t(value1);
    int64_t result = int64_t(uint64_t(value2) >> shift);
    pushOperandStack(stack, result);
  }
};
template<typename T>
class AND : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    T value1 = popOperandStack<T>(stack);
    T value2 = popOperandStack<T>(stack);
    T result = value1 & value2;
    pushOperandStack(stack, result);
  }
};
template<typename T>
class OR : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    T value1 = popOperandStack<T>(stack);
    T value2 = popOperandStack<T>(stack);
    T result = value1 | value2;
    pushOperandStack(stack, result);
  }
};
template<typename T>
class XOR : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    T value1 = popOperandStack<T>(stack);
    T value2 = popOperandStack<T>(stack);
    T result = value1 ^ value2;
    pushOperandStack(stack, result);
  }
};
class IINC : public Instruction {
  private:
  uint32_t index;
  int32_t _const;
  public:
  void fetchOperands(std::shared_ptr<BytecodeReader> reader) override {
    index = uint32_t(reader->readUInt8());
    _const = int32_t(reader->readInt8());
  }
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    LocalVars& vars = frame->getLocalVars();
    int32_t val = vars.getInt(index);
    val += _const;
    vars.setInt(index, val);
  }
};
}