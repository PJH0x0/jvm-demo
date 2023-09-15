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
  static uint32_t GetHeaderSize();
  explicit Frame(const Method* method, Frame* prev_fp);
  LocalVars& GetLocalVars();
  OperandStack& GetOperandStack();
  int32_t NextPc() const;
  void SetNextPc(int32_t next_pc);
  void RevertNextPc();
  const Method* GetMethod() const;
  Frame* GetPrevFrame() const;
  void Dump();
private:
  Frame* prev_fp_;
  LocalVars local_vars_;
  OperandStack operand_stack_;
  const Method* method_;
  int32_t next_pc_;
};
}