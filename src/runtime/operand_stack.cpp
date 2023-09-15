#include "operand_stack.h"

namespace runtime {

void OperandStack::PushInt(int32_t val) {
  if (IsFull()) {
    LOG(FATAL) << "PushInt operand_stack_ overflow";
  }
  slots_.SetInt(top_, val);
  top_++;
}
int32_t OperandStack::PopInt() {
  if (IsEmpty()) {
    LOG(FATAL) << "PopInt operand_stack_ empty";
  }
  top_--;
  return slots_.GetInt(top_);
}
void OperandStack::PushFloat(float val) {
  if (IsFull()) {
    LOG(FATAL) << "PushFloat operand_stack_ overflow";
  }
  slots_.SetFloat(top_, val);
  top_++;
}
float OperandStack::PopFloat() {
  if (IsEmpty()) {
    LOG(FATAL) << "PopFloat operand_stack_ empty";
  }
  top_--;
  return slots_.GetFloat(top_);
}
void OperandStack::PushLong(int64_t val) {
  if (IsLongFull()) {
    LOG(FATAL) << "PushLong operand_stack_ overflow";
  }
  slots_.SetLong(top_, val);
  top_ += 2;
}
int64_t OperandStack::PopLong() {
  if (IsLongEmpty()) {
    LOG(FATAL) << "PopLong operand_stack_ empty";
  }
  top_ -= 2;
  return slots_.GetLong(top_);
}
void OperandStack::PushDouble(double val) {
  if (IsLongFull()) {
    LOG(FATAL) << "PushDouble operand_stack_ overflow";
  }
  slots_.SetDouble(top_, val);
  top_ += 2;
}
double OperandStack::PopDouble() {
  if (IsLongEmpty()) {
    LOG(FATAL) << "PopDouble operand_stack_ empty";
  }
  top_ -= 2;
  return slots_.GetDouble(top_);
}
void OperandStack::PushRef(Object* ref) {
  if (IsLongFull()) {
    LOG(FATAL) << "PushRef operand_stack_ overflow";
  }
  slots_.SetRef(top_, ref);
  top_ += 2;
}
Object* OperandStack::PopRef() {
  if (IsLongEmpty()) {
    LOG(FATAL) << "PopRef operand_stack_ empty";
  }
  top_ -= 2;
  slots_.GetRef(top_);
}
Object* OperandStack::GetRefFromTop(uint32_t index) {
  if (top_ <= index + 2) {
    LOG(FATAL) << "GetRefFromTop operand_stack_ size less than " << index;
  }
  return slots_.GetRef(top_ - index - 2);
}
void OperandStack::PushSlot(Slot slot) {
  if (IsFull()) {
    LOG(FATAL) << "PushSlot operand_stack_ overflow";
  }
  slots_.SetSlot(top_, slot);
  top_++;
}
Slot OperandStack::PopSlot() {
  if (IsEmpty()) {
    LOG(FATAL) << "PopSlot operand_stack_ empty";
  }
  top_--;
  return slots_.GetSlot(top_);
}

}//namespace runtime
