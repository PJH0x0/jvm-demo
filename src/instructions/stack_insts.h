#pragma once


#include "base/base_insts.h"
namespace instructions {
class POP : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    stack.PopSlot();
  }
};
class POP2 : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    stack.PopSlot();
    stack.PopSlot();
  }
};
class DUP : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    runtime::Slot slot = stack.PopSlot();
    stack.PushSlot(slot);
    stack.PushSlot(slot);
  }
};
class DUP_X1 : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    runtime::Slot slot = stack.PopSlot();
    runtime::Slot slotX_1 = stack.PopSlot();
    stack.PushSlot(slot);
    stack.PushSlot(slotX_1);
    stack.PushSlot(slot);
  }
};
class DUP_X2 : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    runtime::Slot slot = stack.PopSlot();
    runtime::Slot slotX_1 = stack.PopSlot();
    runtime::Slot slotX_2 = stack.PopSlot();
    stack.PushSlot(slot);
    stack.PushSlot(slotX_2);
    stack.PushSlot(slotX_1);
    stack.PushSlot(slot);
  }
};
class DUP2 : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    runtime::Slot slot1 = stack.PopSlot();
    runtime::Slot slot2 = stack.PopSlot();
    stack.PushSlot(slot2);
    stack.PushSlot(slot1);
    stack.PushSlot(slot2);
    stack.PushSlot(slot1);
  }
};
class DUP2_X1 : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    runtime::Slot slot1 = stack.PopSlot();
    runtime::Slot slot2 = stack.PopSlot();
    runtime::Slot slotX_1 = stack.PopSlot();
    stack.PushSlot(slot2);
    stack.PushSlot(slot1);
    stack.PushSlot(slotX_1);
    stack.PushSlot(slot2);
    stack.PushSlot(slot1);
  }
};
class DUP2_X2 : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    runtime::Slot slot1 = stack.PopSlot();
    runtime::Slot slot2 = stack.PopSlot();
    runtime::Slot slotX_1 = stack.PopSlot();
    runtime::Slot slotX_2 = stack.PopSlot();
    stack.PushSlot(slot2);
    stack.PushSlot(slot1);
    stack.PushSlot(slotX_2);
    stack.PushSlot(slotX_1);
    stack.PushSlot(slot2);
    stack.PushSlot(slot1);
  }
};
class SWAP : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->GetOperandStack();
    runtime::Slot slot1 = stack.PopSlot();
    runtime::Slot slot2 = stack.PopSlot();
    stack.PushSlot(slot1);
    stack.PushSlot(slot2);
  }
};
}