#pragma once

#include <cstdint>
#include <memory>
#include <runtime/frame.h>
#include <type_traits>
#include "base/base_insts.h"
#include <runtime/slots.h>
#include <runtime/oo/object.h>
#include <runtime/oo/method.h>
namespace instructions {
using runtime::Frame;
using runtime::LocalVars;
using runtime::OperandStack;
template <typename T>
inline void _load(std::shared_ptr<runtime::Frame> frame, uint16_t index) {
  LocalVars& vars = frame->getLocalVars();
  OperandStack& stack = frame->getOperandStack();
  if (std::is_same<T, int32_t>::value) {
    int32_t val = vars.getInt(index);
    stack.pushInt(val);
  } else if (std::is_same<T, int64_t>::value) {
    int64_t val = vars.getLong(index);
    stack.pushLong(val);
  } else if (std::is_same<T, float>::value) {
    float val = vars.getFloat(index);
    stack.pushFloat(val);
  } else if (std::is_same<T, double>::value) {
    double val = vars.getDouble(index);
    stack.pushDouble(val);
  } else if (std::is_same<T, runtime::Object*>::value) {
    runtime::Object* val = vars.getRef(index);
    stack.pushRef(val);
  }
}

template <typename T>
void _aload(std::shared_ptr<runtime::Frame> frame) {
  OperandStack& stack = frame->getOperandStack();
  int32_t index = stack.popInt();
  auto arrRef = stack.popRef();
  if (arrRef == nullptr) {
    throw std::runtime_error("java.lang.NullPointerException");
  }
  if (index < 0 /*|| index >= arrRef->arrayLength()*/) {
    //LOG(ERROR) << "index: " << index << " array length: " << arrRef->arrayLength();
    LOG(ERROR) << "array addr " << arrRef;
    LOG(ERROR) << "array type " /*<< arrRef->getArrayType()*/ << " " << arrRef->getClass()->getName();
    LOG(ERROR) << "method = " << frame->getMethod()->getName()
               << "descriptor = " << frame->getMethod()->getDescriptor()
               << " class = " << frame->getMethod()->getClass()->getName();
    throw std::runtime_error("ArrayIndexOutOfBoundsException");
  }
  //if (std::is_base_of_v<typeid(arrRef), >)
  // switch(arrRef->getArrayType()) {
  //   case runtime::ARRAY_TYPE::AT_BOOLEAN:
  //   case runtime::ARRAY_TYPE::AT_BYTE:
  //     stack.pushInt(arrRef->getArrayElement<int8_t>(index));
  //     break;
  //   case runtime::ARRAY_TYPE::AT_CHAR:
  //     stack.pushInt(arrRef->getArrayElement<char16_t>(index));
  //     break;
  //   case runtime::ARRAY_TYPE::AT_SHORT:
  //     stack.pushInt(arrRef->getArrayElement<int16_t>(index));
  //     break;
  //   case runtime::ARRAY_TYPE::AT_INT:
  //     stack.pushInt(arrRef->getArrayElement<int32_t>(index));
  //     break;
  //   case runtime::ARRAY_TYPE::AT_LONG:
  //     stack.pushLong(arrRef->getArrayElement<int64_t>(index));
  //     break;
  //   case runtime::ARRAY_TYPE::AT_FLOAT:
  //     stack.pushFloat(arrRef->getArrayElement<float>(index));
  //     break;
  //   case runtime::ARRAY_TYPE::AT_DOUBLE:
  //     stack.pushDouble(arrRef->getArrayElement<double>(index));
  //     break;
  //   case runtime::ARRAY_TYPE::AT_OBJECT:
  //     stack.pushRef(arrRef->getArrayElement<runtime::Object*>(index));
  //     break;
  //   default:
  //     break;
  // }
}
template<typename T>
class LOAD : public Index8Instruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    _load<T>(frame, uint16_t(index));
  }
};
template<typename T>
class LOAD_0 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    _load<T>(frame, 0);
  }
};
template<typename T>
class LOAD_1 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    _load<T>(frame, 1);
  }
};
template<typename T>
class LOAD_2 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    _load<T>(frame, 2);
  }
};
template<typename T>
class LOAD_3 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    _load<T>(frame, 3);
  }
};
template<typename T>
class ALOAD : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override {
    //_load<T>(frame, uint16_t(index));
    _aload<T>(frame);
  }
};
}