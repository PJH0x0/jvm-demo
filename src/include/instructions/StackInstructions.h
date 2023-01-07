#pragma once


#include "base/Instruction.h"
namespace instructions {
class POP : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    stack.popSlot();
  }
};
class POP2 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    stack.popSlot();
    stack.popSlot();
  }
};
class DUP : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    rtda::Slot slot = stack.popSlot();
    stack.pushSlot();
    stack.pushSlot();
  }
};
class DUP_X1 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    rtda::Slot slot = stack.popSlot();
    rtda::Slot slotX_1 = stack.popSlot();
    stack.pushSlot(slot);
    stack.pushSlot(slotX_1);
    stack.pushSlot(slot);
  }
};
class DUP_X2 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    rtda::Slot slot = stack.popSlot();
    rtda::Slot slotX_1 = stack.popSlot();
    rtda::Slot slotX_2 = stack.popSlot();
    stack.pushSlot(slot);
    stack.pushSlot(slotX_2);
    stack.pushSlot(slotX_1);
    stack.pushSlot(slot);
  }
};
class DUP2 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    rtda::Slot slot1 = stack.popSlot();
    rtda::Slot slot2 = stack.popSlot();
    stack.pushSlot(slot2);
    stack.pushSlot(slot1);
    stack.pushSlot(slot2);
    stack.pushSlot(slot1);
  }
};
class DUP2_X1 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    rtda::Slot slot1 = stack.popSlot();
    rtda::Slot slot2 = stack.popSlot();
    rtda::Slot slotX_1 = stack.popSlot();
    stack.pushSlot(slot2);
    stack.pushSlot(slot1);
    stack.pushSlot(slotX_1);
    stack.pushSlot(slot2);
    stack.pushSlot(slot1);
  }
};
class DUP2_X1 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    rtda::Slot slot1 = stack.popSlot();
    rtda::Slot slot2 = stack.popSlot();
    rtda::Slot slotX_1 = stack.popSlot();
    rtda::Slot slotX_2 = stack.popSlot();
    stack.pushSlot(slot2);
    stack.pushSlot(slot1);
    stack.pushSlot(slotX_2);
    stack.pushSlot(slotX_1);
    stack.pushSlot(slot2);
    stack.pushSlot(slot1);
  }
};
class SWAP : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    OperandStack& stack = frame->getOperandStack();
    rtda::Slot slot1 = stack.popSlot();
    rtda::Slot slot2 = stack.popSlot();
    stack.pushSlot(slot1);
    stack.pushSlot(slot2);
  }
};
}