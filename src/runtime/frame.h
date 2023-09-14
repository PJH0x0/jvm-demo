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

public:
  Frame(std::shared_ptr<Thread> thread, uint16_t max_locals,
        uint16_t max_operand_stacks, std::shared_ptr<Method> method) :
      thread_(thread),
      local_vars_(max_locals),
      operand_stack_(max_operand_stacks),
      next_pc_(0),
      method_(method) {}
  LocalVars& GetLocalVars() {
    return local_vars_;
  }
  OperandStack& GetOperandStack() {
    return operand_stack_;
  }
  int32_t NextPc() {
    return next_pc_;
  }
  void SetNextPc(int32_t nextPc) {
    next_pc_ = nextPc;
  }
  void RevertNextPc() {
    next_pc_ = thread_->GetPc();
  }
  std::shared_ptr<Thread> GetThread() {
    return thread_;
  }
  std::shared_ptr<Method> GetMethod() {
    return method_;
  }
  void Dump() {
    //LOG(INFO) << "pc: " << next_pc_ << " " << method_->GetName() << " " << method_->GetDescriptor() << " " << method_->GetClass()->GetName();
    local_vars_.Dump();
    operand_stack_.Dump();
  }
private:
  LocalVars local_vars_;
  OperandStack operand_stack_;
  std::shared_ptr<Thread> thread_;
  const std::shared_ptr<Method> method_;
  int32_t next_pc_;
};
}