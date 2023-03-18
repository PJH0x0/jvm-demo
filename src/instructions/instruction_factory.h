#pragma once
#include "arithmetic_instructions.h"
#include "compare_instructions.h"
#include "conversion_instructions.h"
#include "stack_instructions.h"
#include "base/base_instructions.h"
#include "const_instructions.h"
#include "load_instructions.h"
#include "store_instructions.h"
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