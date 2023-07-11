#pragma once
#include "arithmetic_insts.h"
#include "compare_insts.h"
#include "conversion_insts.h"
#include "stack_insts.h"
#include "base/base_insts.h"
#include "const_insts.h"
#include "load_insts.h"
#include "store_insts.h"
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