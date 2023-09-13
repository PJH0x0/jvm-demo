#pragma once

#include "base/base_insts.h"

namespace instructions {
template<typename T, typename R>
class T2R : public NoOperandsInstruction {
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    T value = PopOperandStack<T>(stack);
    R result = static_cast<R>(value);
    PushOperandStack(stack, result);
  }
};
template<typename R>
class I2R : public NoOperandsInstruction {
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    int32_t value = PopOperandStack<int32_t>(stack);
    R temp = static_cast<R>(value);
    int32_t result = static_cast<int32_t>(temp);
    PushOperandStack(stack, result);
  }
};

}