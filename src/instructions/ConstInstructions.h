#pragma once

#include "base/Instruction.h"
#include <memory>
namespace instructions {
class ACONST_NULL : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    frame->getOperandStack().pushRef(nullptr);
  }
};
class DCONST_0 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    frame->getOperandStack().pushDouble(0.0);
  }
};
class DCONST_1 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    frame->getOperandStack().pushDouble(1.0);
  }
};
class FCONST_0 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    frame->getOperandStack().pushFloat(0.0f);
  }
};
class FCONST_1 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    frame->getOperandStack().pushFloat(1.0f);
  }
};
class FCONST_2 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    frame->getOperandStack().pushFloat(2.0f);
  }
};
class ICONST_M1 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    frame->getOperandStack().pushInt(-1);
  }
};
class ICONST_0 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    frame->getOperandStack().pushInt(0);
  }
};
class ICONST_1 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    frame->getOperandStack().pushInt(1);
  }
};
class ICONST_2 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    frame->getOperandStack().pushInt(2);
  }
};
class ICONST_3 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    frame->getOperandStack().pushInt(3);
  }
};
class ICONST_4 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    frame->getOperandStack().pushInt(4);
  }
};
class ICONST_5 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    frame->getOperandStack().pushInt(5);
  }
};
class LCONST_0 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    frame->getOperandStack().pushLong(0L);
  }
};
class LCONST_1 : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    frame->getOperandStack().pushLong(1L);
  }
};
class BIPUSH : public Instruction {
  private:
  int8_t val;
  public:
  void fetchOperands(std::shared_ptr<BytecodeReader> reader) override {
    val = reader->readInt8();
  }
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    int32_t i = int32_t(val);
    frame->getOperandStack().pushInt(i);
  }
};
class SIPUSH : public Instruction {
  private:
  int16_t val;
  public:
  void fetchOperands(std::shared_ptr<BytecodeReader> reader) override {
    val = reader->readInt8();
  }
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    int32_t i = int32_t(val);
    frame->getOperandStack().pushInt(i);
  }
};
}