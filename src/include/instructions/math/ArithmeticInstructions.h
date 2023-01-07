#pragma once

#include <cstdint>
#include <type_traits>
#include <instructions/base/Instruction.h>
#include <glog/logging.h>
#include <cmath>

namespace instructions {
template<typename T>
T popOperandStack(rtda::OperandStack& stack) {
  T value;
  if (std::is_same<T, int32_t>::value) {
    value = stack.popInt();
  } else if (std::is_same<T, int64_t>::value) {
    value = stack.popLong();
  } else if (std::is_same<T, float>::value) {
    value = stack.popFloat();
  } else if (std::is_same<T, double>::value) {
    value = stack.popDouble();
  } else {
    LOG(ERROR) << "popOperandStack T not match int/long/float/double";
  }
  return value;
}
template<typename T>
void pushOperandStack(rtda::OperandStack& stack, T value) {
  if (std::is_same<T, int32_t>::value) {
    stack.pushInt(value);
  } else if (std::is_same<T, int64_t>::value) {
    stack.pushLong(value);
  } else if (std::is_same<T, float>::value) {
    stack.pushFloat(value);
  } else if (std::is_same<T, double>::value) {
    stack.pushDouble(value);
  } else {
    LOG(ERROR) << "pushOperandStack T not match int/long/float/double";
  }
}
template<typename T>
class ADD : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) {
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
  void execute(std::shared_ptr<rtda::Frame> frame) {
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
  void execute(std::shared_ptr<rtda::Frame> frame) {
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
  void execute(std::shared_ptr<rtda::Frame> frame) {
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
  void execute(std::shared_ptr<rtda::Frame> frame) {
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
  void execute(std::shared_ptr<rtda::Frame> frame) {
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
}