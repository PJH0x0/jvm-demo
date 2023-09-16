#include "frame.h"
#include "thread.h"
#include <runtime/oo/method.h>

namespace runtime {
uint32_t Frame::GetHeaderSize() {
  return sizeof(Frame*) + sizeof(Method*) + sizeof(int32_t);
}
Frame::Frame(const Method* method, Frame* prev_fp) :
    local_vars_(method->GetMaxLocals()),
    operand_stack_(method->GetMaxStack()),
    next_pc_(0),
    method_(method),
    prev_fp_(prev_fp) {}
LocalVars& Frame::GetLocalVars() {
  return local_vars_;
}
OperandStack& Frame::GetOperandStack() {
  return operand_stack_;
}
int32_t Frame::NextPc() const {
  return next_pc_;
}
void Frame::SetNextPc(int32_t next_pc) {
  next_pc_ = next_pc;
}
void Frame::RevertNextPc() {
  next_pc_ = Thread::Current()->GetPc();
}
const Method* Frame::GetMethod() const {
  return method_;
}
Frame* Frame::GetPrevFrame() const {
  return prev_fp_;
}
void Frame::Dump() const {
  //LOG(INFO) << "pc: " << next_pc_ << " " << method_->GetName() << " " << method_->GetDescriptor() << " " << method_->GetClass()->GetName();
  local_vars_.Dump();
  operand_stack_.Dump();
}
}//namespace runtime
