#pragma once

#include <cstdint>
#include <memory>
#include "LocalVars.h"
#include "OperandStack.h"
namespace rtda {
struct Frame {
  private:
  LocalVars localVars;
  OperandStack operandStack;

  public:
  Frame(uint16_t maxLocals, uint16_t maxOperandStacks) : localVars(maxLocals), operandStack(maxOperandStacks) {}
  LocalVars& getLocalVars() {
    return localVars;
  }
  OperandStack& getOperandStack() {
    return operandStack;
  }
};
}