#pragma once

#include <cstdint>
#include <memory>
#include "LocalVars.h"
#include "OperandStack.h"
namespace rtda {
class LocalVars;
class OperandStack;
struct Frame {
  LocalVars localVars;
  OperandStack operandStack;
  Frame(uint16_t maxLocals, uint16_t maxOperandStacks) : localVars(maxLocals), operandStack(maxOperandStacks){}
  
};
}