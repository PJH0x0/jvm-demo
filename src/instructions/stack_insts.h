#pragma once


#include "base/base_insts.h"
namespace instructions {
class POP : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    stack.popSlot();
  }
};
class POP2 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    stack.popSlot();
    stack.popSlot();
  }
};
class DUP : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    runtime::Slot slot = stack.popSlot();
    stack.pushSlot(slot);
    stack.pushSlot(slot);
  }
};
class DUP_X1 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    runtime::Slot slot = stack.popSlot();
    runtime::Slot slotX_1 = stack.popSlot();
    stack.pushSlot(slot);
    stack.pushSlot(slotX_1);
    stack.pushSlot(slot);
  }
};
class DUP_X2 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    runtime::Slot slot = stack.popSlot();
    runtime::Slot slotX_1 = stack.popSlot();
    runtime::Slot slotX_2 = stack.popSlot();
    stack.pushSlot(slot);
    stack.pushSlot(slotX_2);
    stack.pushSlot(slotX_1);
    stack.pushSlot(slot);
  }
};
class DUP2 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    runtime::Slot slot1 = stack.popSlot();
    runtime::Slot slot2 = stack.popSlot();
    stack.pushSlot(slot2);
    stack.pushSlot(slot1);
    stack.pushSlot(slot2);
    stack.pushSlot(slot1);
  }
};
class DUP2_X1 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    runtime::Slot slot1 = stack.popSlot();
    runtime::Slot slot2 = stack.popSlot();
    runtime::Slot slotX_1 = stack.popSlot();
    stack.pushSlot(slot2);
    stack.pushSlot(slot1);
    stack.pushSlot(slotX_1);
    stack.pushSlot(slot2);
    stack.pushSlot(slot1);
  }
};
class DUP2_X2 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    runtime::Slot slot1 = stack.popSlot();
    runtime::Slot slot2 = stack.popSlot();
    runtime::Slot slotX_1 = stack.popSlot();
    runtime::Slot slotX_2 = stack.popSlot();
    stack.pushSlot(slot2);
    stack.pushSlot(slot1);
    stack.pushSlot(slotX_2);
    stack.pushSlot(slotX_1);
    stack.pushSlot(slot2);
    stack.pushSlot(slot1);
  }
};
class SWAP : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    runtime::Slot slot1 = stack.popSlot();
    runtime::Slot slot2 = stack.popSlot();
    stack.pushSlot(slot1);
    stack.pushSlot(slot2);
  }
};
}