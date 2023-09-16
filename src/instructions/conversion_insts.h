#pragma once

#include "base/base_insts.h"
#include "runtime/frame.h"

namespace instructions {
template<typename T, typename R>
class T2R : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    OperandStack& stack = frame->GetOperandStack();
    T value = PopOperandStack<T>(stack);
    R result = static_cast<R>(value);
    PushOperandStack(stack, result);
  }
};
template<typename R>
class I2R : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    OperandStack& stack = frame->GetOperandStack();
    int32_t value = PopOperandStack<int32_t>(stack);
    R temp = static_cast<R>(value);
    int32_t result = static_cast<int32_t>(temp);
    PushOperandStack(stack, result);
  }
};

}