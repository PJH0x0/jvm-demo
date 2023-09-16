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
inline void Load(runtime::Frame* frame, uint16_t index) {
  LocalVars& vars = frame->GetLocalVars();
  OperandStack& stack = frame->GetOperandStack();
  if (std::is_same<T, int32_t>::value) {
    int32_t val = vars.GetInt(index);
    stack.PushInt(val);
  } else if (std::is_same<T, int64_t>::value) {
    int64_t val = vars.GetLong(index);
    stack.PushLong(val);
  } else if (std::is_same<T, float>::value) {
    float val = vars.GetFloat(index);
    stack.PushFloat(val);
  } else if (std::is_same<T, double>::value) {
    double val = vars.GetDouble(index);
    stack.PushDouble(val);
  } else if (std::is_same<T, runtime::Object*>::value) {
    runtime::Object* val = vars.GetRef(index);
    stack.PushRef(val);
  }
}

template <typename T>
void ArrayLoad(runtime::Frame* frame) {
  OperandStack& stack = frame->GetOperandStack();
  int32_t index = stack.PopInt();
  auto arr_ref = stack.PopRef();
  if (arr_ref == nullptr) {
    throw std::runtime_error("java.lang.NullPointerException");
  }
  if (index < 0 /*|| index >= arr_ref->arrayLength()*/) {
    //LOG(ERROR) << "index: " << index << " array length: " << arr_ref->arrayLength();
    LOG(ERROR) << "array addr " << arr_ref;
    LOG(ERROR) << "array type " /*<< arr_ref->getArrayType()*/ << " " << arr_ref->GetClass()->GetName();
    LOG(ERROR) << "method = " << frame->GetMethod()->GetName()
               << "descriptor = " << frame->GetMethod()->GetDescriptor()
               << " class = " << frame->GetMethod()->GetClass()->GetName();
    throw std::runtime_error("ArrayIndexOutOfBoundsException");
  }
  //if (std::is_base_of_v<typeid(arr_ref), >)
  // switch(arr_ref->getArrayType()) {
  //   case runtime::ArrayType::kBoolean:
  //   case runtime::ArrayType::kByte:
  //     stack.PushInt(arr_ref->getArrayElement<int8_t>(index));
  //     break;
  //   case runtime::ArrayType::kChar:
  //     stack.PushInt(arr_ref->getArrayElement<char16_t>(index));
  //     break;
  //   case runtime::ArrayType::kShort:
  //     stack.PushInt(arr_ref->getArrayElement<int16_t>(index));
  //     break;
  //   case runtime::ArrayType::kInt:
  //     stack.PushInt(arr_ref->getArrayElement<int32_t>(index));
  //     break;
  //   case runtime::ArrayType::kLong:
  //     stack.PushLong(arr_ref->getArrayElement<int64_t>(index));
  //     break;
  //   case runtime::ArrayType::kFloat:
  //     stack.PushFloat(arr_ref->getArrayElement<float>(index));
  //     break;
  //   case runtime::ArrayType::kDouble:
  //     stack.PushDouble(arr_ref->getArrayElement<double>(index));
  //     break;
  //   case runtime::ArrayType::kObject:
  //     stack.PushRef(arr_ref->getArrayElement<runtime::Object*>(index));
  //     break;
  //   default:
  //     break;
  // }
}
template<typename T>
class LOAD : public Index8Instruction {
  public:
  void Execute(runtime::Frame* frame) override {
    Load<T>(frame, uint16_t(index_));
  }
};
template<typename T>
class LOAD_0 : public NoOperandsInstruction {
  public:
  void Execute(runtime::Frame* frame) override {
    Load<T>(frame, 0);
  }
};
template<typename T>
class LOAD_1 : public NoOperandsInstruction {
  public:
  void Execute(runtime::Frame* frame) override {
    Load<T>(frame, 1);
  }
};
template<typename T>
class LOAD_2 : public NoOperandsInstruction {
  public:
  void Execute(runtime::Frame* frame) override {
    Load<T>(frame, 2);
  }
};
template<typename T>
class LOAD_3 : public NoOperandsInstruction {
  public:
  void Execute(runtime::Frame* frame) override {
    Load<T>(frame, 3);
  }
};
template<typename T>
class ALOAD : public NoOperandsInstruction {
  public:
  void Execute(runtime::Frame* frame) override {
    //Load<T>(frame, uint16_t(index));
    ArrayLoad<T>(frame);
  }
};
}