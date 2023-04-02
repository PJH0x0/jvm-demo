#pragma once

#include <cstdint>
#include <memory>
#include "local_vars.h"
#include "operand_stack.h"
#include "thread.h"
namespace rtda {
class Method;
struct Frame {
  private:
  LocalVars localVars;
  OperandStack operandStack;
  std::shared_ptr<Thread> thread_;
  std::shared_ptr<Method> mMethod;
  int32_t nextPc_;

  public:
  Frame(std::shared_ptr<Thread> thread, uint16_t maxLocals, uint16_t maxOperandStacks, std::shared_ptr<Method> method) : 
      thread_(thread), 
      localVars(maxLocals), 
      operandStack(maxOperandStacks),
      nextPc_(0),
      mMethod(method) {}
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
  std::shared_ptr<Method> getMethod() {
    return mMethod;
  }
};
}