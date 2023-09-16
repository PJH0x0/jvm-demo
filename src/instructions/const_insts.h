#pragma once

#include "base/base_insts.h"
#include "runtime/frame.h"
#include <runtime/oo/method.h>
#include <memory>
namespace instructions {
class ACONST_NULL : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    frame->GetOperandStack().PushRef(nullptr);
  }
};
class DCONST_0 : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    frame->GetOperandStack().PushDouble(0.0);
  }
};
class DCONST_1 : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    frame->GetOperandStack().PushDouble(1.0);
  }
};
class FCONST_0 : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    frame->GetOperandStack().PushFloat(0.0f);
  }
};
class FCONST_1 : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    frame->GetOperandStack().PushFloat(1.0f);
  }
};
class FCONST_2 : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    frame->GetOperandStack().PushFloat(2.0f);
  }
};
class ICONST_M1 : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    frame->GetOperandStack().PushInt(-1);
  }
};
class ICONST_0 : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    frame->GetOperandStack().PushInt(0);
  }
};
class ICONST_1 : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    frame->GetOperandStack().PushInt(1);
  }
};
class ICONST_2 : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    frame->GetOperandStack().PushInt(2);
  }
};
class ICONST_3 : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    frame->GetOperandStack().PushInt(3);
  }
};
class ICONST_4 : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    frame->GetOperandStack().PushInt(4);
  }
};
class ICONST_5 : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    frame->GetOperandStack().PushInt(5);
  }
};
class LCONST_0 : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    frame->GetOperandStack().PushLong(0L);
  }
};
class LCONST_1 : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override {
    frame->GetOperandStack().PushLong(1L);
  }
};
class BIPUSH : public Instruction {
  private:
  int8_t val_;
  public:
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override {
    val_ = reader->ReadInt8();
  }
  void Execute(runtime::Frame* frame) override {
    auto i = int32_t(val_);
    frame->GetOperandStack().PushInt(i);
  }
};
class SIPUSH : public Instruction {
  private:
  int16_t val_;
  public:
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override {
    val_ = reader->ReadInt16();
  }
  void Execute(runtime::Frame* frame) override {
    auto i = int32_t(val_);
    frame->GetOperandStack().PushInt(i);
  }
};
}