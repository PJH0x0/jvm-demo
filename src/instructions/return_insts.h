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
void _return(std::shared_ptr<runtime::Frame> frame) {
  std::shared_ptr<runtime::Frame> currentFrame = frame->getThread()->popFrame();
  LOG_IF(INFO, INST_DEBUG) << "return from " << currentFrame->getMethod()->GetName();
  if (std::is_same<T, void>::value) {
    return;
  }
  std::shared_ptr<runtime::Frame> invokerFrame = frame->getThread()->currentFrame();
  
  if (std::is_same<T, int32_t>::value) {
    int32_t val = currentFrame->getOperandStack().popInt();
    invokerFrame->getOperandStack().pushInt(val);
  } else if (std::is_same<T, int64_t>::value) {
    int64_t val = currentFrame->getOperandStack().popLong();
    invokerFrame->getOperandStack().pushLong(val);
  } else if (std::is_same<T, float>::value) {
    float val = currentFrame->getOperandStack().popFloat();
    invokerFrame->getOperandStack().pushFloat(val);
  } else if (std::is_same<T, double>::value) {
    double val = currentFrame->getOperandStack().popDouble();
    invokerFrame->getOperandStack().pushDouble(val);
  } else if (std::is_same<T, runtime::Object*>::value) {
    runtime::Object* val = currentFrame->getOperandStack().popRef();
    invokerFrame->getOperandStack().pushRef(val);
  }
}
class RETURN : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    //LOG_IF(INFO, INST_DEBUG) << "return from " << frame->GetMethod()->name_;
    _return<void>(frame);
  }
};
class IRETURN : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    _return<int32_t>(frame);
  }
};
class LRETURN : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    _return<int64_t>(frame);
  }
};
class FRETURN : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    _return<float>(frame);
  }
};
class DRETURN : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    _return<double>(frame);
  }
};
class ARETURN : public NoOperandsInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    _return<runtime::Object*>(frame);
  }
};
} // namespace instructions