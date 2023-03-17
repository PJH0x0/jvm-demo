#pragma once
#include "ArithmeticInstructions.h"
#include "CompareInstructions.h"
#include "ConversionInstructions.h"
#include "StackInstructions.h"
#include "base/Instruction.h"
#include "ConstInstructions.h"
#include "LoadInstructions.h"
#include "StoreInstructions.h"
#include <cmath>
#include <memory>

namespace instructions {
class InstNotFoundException : public std::exception {
  private:
  int32_t opcode_;
  public:
  InstNotFoundException(int32_t opcode) : opcode_(opcode) {}
  int32_t opcode() {
    return opcode_;
  }
  const char* what() const throw() {
    return "InstNotFoundException ";
  }
};
std::shared_ptr<Instruction> createInstruction(uint8_t opcode);

}