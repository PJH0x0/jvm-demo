#pragma once

#include <cstdint>
#include <memory>
#include "LocalVars.h"
#include "OperandStack.h"
#include "Thread.h"
namespace rtda {
class LocalVars;
class OperandStack;
struct Frame {
  private:
  LocalVars localVars;
  OperandStack operandStack;
  std::shared_ptr<Thread> thread;
  int64_t nextPc;

  public:
  Frame(uint16_t maxLocals, uint16_t maxOperandStacks, std::shared_ptr<Thread> _thread) : localVars(maxLocals), operandStack(maxOperandStacks), thread(_thread){}
  LocalVars& getLocalVars() {
    return localVars;
  }
  OperandStack& getOperandStack() {
    return operandStack;
  }
  std::shared_ptr<Thread> getThread() {
    return thread;
  }
  void setNextPC(int64_t pc) {
    nextPc = pc;
  }
  int64_t getNextPC() {
    return nextPc;
  }
};
}