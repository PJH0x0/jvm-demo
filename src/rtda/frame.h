#pragma once

#include <cstdint>
#include <memory>
#include "local_vars.h"
#include "operand_stack.h"
#include "thread.h"
namespace rtda {
struct Frame {
  private:
  LocalVars localVars;
  OperandStack operandStack;
  std::shared_ptr<Thread> thread_;
  int32_t nextPc_;

  public:
  Frame(std::shared_ptr<Thread> thread, uint16_t maxLocals, uint16_t maxOperandStacks) : 
      thread_(thread), 
      localVars(maxLocals), 
      operandStack(maxOperandStacks),
      nextPc_(0) {}
  LocalVars& getLocalVars() {
    return localVars;
  }
  OperandStack& getOperandStack() {
    return operandStack;
  }
  int32_t nextPC() {
    return nextPc_;
  }
  void setNextPC(int32_t nextPc) {
    nextPc_ = nextPc;
  }
  std::shared_ptr<Thread> getThread() {
    return thread_;
  }
};
}