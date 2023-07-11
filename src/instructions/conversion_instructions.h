#pragma once

#include "base/base_instructions.h"

namespace instructions {
template<typename T, typename R>
class T2R : public NoOperandsInstruction {
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    T value = popOperandStack<T>(stack);
    R result = static_cast<R>(value);
    pushOperandStack(stack, result);
  }
};
template<typename R>
class I2R : public NoOperandsInstruction {
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    int32_t value = popOperandStack<int32_t>(stack);
    R temp = static_cast<R>(value);
    int32_t result = static_cast<int32_t>(temp);
    pushOperandStack(stack, result);
  }
};

}