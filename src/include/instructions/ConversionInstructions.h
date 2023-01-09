#pragma once

#include "base/Instruction.h"

namespace instructions {
template<typename T, typename R>
class T2R : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    T value = popOperandStack<T>(stack);
    R result = static_cast<R>(value);
    pushOperandStack(stack, result);
  }
};
}