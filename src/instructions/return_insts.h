#pragma once
#include <memory>
#include <runtime/frame.h>
#include <type_traits>
#include "base/base_insts.h"
#include "instructions/arithmetic_insts.h"
#include <runtime/oo/method.h>
#include <runtime/slots.h>
typedef classfile::u1 u1;

namespace instructions {
template<typename T>
void Return(runtime::Frame* frame) {
  auto currentFrame = runtime::Thread::Current()->PopFrame();
  LOG_IF(INFO, INST_DEBUG) << "return from " << currentFrame->GetMethod()->GetName();
  if (std::is_same<T, void>::value) {
    return;
  }
  auto invokerFrame = runtime::Thread::Current()->CurrentFrame();
  
  if (std::is_same<T, int32_t>::value) {
    int32_t val = currentFrame->GetOperandStack().PopInt();
    invokerFrame->GetOperandStack().PushInt(val);
  } else if (std::is_same<T, int64_t>::value) {
    int64_t val = currentFrame->GetOperandStack().PopLong();
    invokerFrame->GetOperandStack().PushLong(val);
  } else if (std::is_same<T, float>::value) {
    float val = currentFrame->GetOperandStack().PopFloat();
    invokerFrame->GetOperandStack().PushFloat(val);
  } else if (std::is_same<T, double>::value) {
    double val = currentFrame->GetOperandStack().PopDouble();
    invokerFrame->GetOperandStack().PushDouble(val);
  } else if (std::is_same<T, runtime::Object*>::value) {
    runtime::Object* val = currentFrame->GetOperandStack().PopRef();
    invokerFrame->GetOperandStack().PushRef(val);
  }
}
class RETURN : public NoOperandsInstruction {
  public:
  void Execute(runtime::Frame* frame) override {
    //LOG_IF(INFO, INST_DEBUG) << "return from " << frame->GetMethod()->name_;
    Return<void>(frame);
  }
};
class IRETURN : public NoOperandsInstruction {
  public:
  void Execute(runtime::Frame* frame) override {
    Return<int32_t>(frame);
  }
};
class LRETURN : public NoOperandsInstruction {
  public:
  void Execute(runtime::Frame* frame) override {
    Return<int64_t>(frame);
  }
};
class FRETURN : public NoOperandsInstruction {
  public:
  void Execute(runtime::Frame* frame) override {
    Return<float>(frame);
  }
};
class DRETURN : public NoOperandsInstruction {
  public:
  void Execute(runtime::Frame* frame) override {
    Return<double>(frame);
  }
};
class ARETURN : public NoOperandsInstruction {
  public:
  void Execute(runtime::Frame* frame) override {
    Return<runtime::Object*>(frame);
  }
};
} // namespace instructions