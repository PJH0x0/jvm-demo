#pragma once

#include <cstdint>
#include <memory>
#include "local_vars.h"
#include "operand_stack.h"
#include "thread.h"
#include <glog/logging.h>
namespace runtime {
class Method;
class Frame {
  private:
  LocalVars localVars;
  OperandStack operandStack;
  std::shared_ptr<Thread> thread_;
  const std::shared_ptr<Method> mMethod;
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
  void revertNextPC() {
    nextPc_ = thread_->getPC();
  }
  std::shared_ptr<Thread> getThread() {
    return thread_;
  }
  std::shared_ptr<Method> getMethod() {
    return mMethod;
  }
  void dump() {
    //LOG(INFO) << "pc: " << nextPc_ << " " << mMethod->getName() << " " << mMethod->getDescriptor() << " " << mMethod->getClass()->getName();
    localVars.dump();
    operandStack.dump();
  }
};
}