#pragma once
#include <memory>
#include <rtda/frame.h>
#include <type_traits>
#include "base/base_instructions.h"
#include "instructions/arithmetic_instructions.h"
#include <rtda/heap/method.h>
namespace instructions {
template<typename T>
void _return(std::shared_ptr<rtda::Frame> frame) {
  std::shared_ptr<rtda::Frame> currentFrame = frame->getThread()->popFrame();
  LOG(INFO) << "return from " << currentFrame->getMethod()->mName;
  if (!frame->getThread()->isStackEmpty()) {
    std::shared_ptr<rtda::Frame> invokerFrame = frame->getThread()->currentFrame();
    LOG(INFO) << "return to " << invokerFrame->getMethod()->mName;
  }
  if (std::is_same<T, void>::value) {
    return;
  }
  std::shared_ptr<rtda::Frame> invokerFrame = frame->getThread()->currentFrame();
  
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
  } else if (std::is_same<T, void*>::value) {
    void* val = currentFrame->getOperandStack().popRef();
    invokerFrame->getOperandStack().pushRef(val);
  }
}
class RETURN : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    //LOG(INFO) << "return from " << frame->getMethod()->mName;
    _return<void>(frame);
  }
};
class IRETURN : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    _return<int32_t>(frame);
  }
};
class LRETURN : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    _return<int64_t>(frame);
  }
};
class FRETURN : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    _return<float>(frame);
  }
};
class DRETURN : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    _return<double>(frame);
  }
};
class ARETURN : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    _return<void*>(frame);
  }
};
} // namespace instructions