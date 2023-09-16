#pragma once

#include <cstdint>
#include <memory>
#include <runtime/frame.h>
#include <type_traits>
#include "base/base_insts.h"
#include <runtime/slots.h>
#include <runtime/constant_pool.h>
#include <runtime/oo/object.h>
#include <runtime/oo/method.h>

namespace instructions {
using runtime::Frame;
using runtime::LocalVars;
using runtime::OperandStack;
template <typename T>
inline void Store(runtime::Frame* frame, uint16_t index) {
  LocalVars& vars = frame->GetLocalVars();
  OperandStack& stack = frame->GetOperandStack();
  if (std::is_same<T, int32_t>::value) {
    int32_t val = stack.PopInt();
    vars.SetInt(index, val);
  } else if (std::is_same<T, int64_t>::value) {
    int64_t val = stack.PopLong();
    vars.SetLong(index, val);
  } else if (std::is_same<T, float>::value) {
    float val = stack.PopFloat();
    vars.SetFloat(index, val);
  } else if (std::is_same<T, double>::value) {
    double val = stack.PopDouble();
    vars.SetDouble(index, val);
  } else if (std::is_same<T, runtime::Object*>::value) {
    runtime::Object* val = stack.PopRef();
    vars.SetRef(index, val);
  }
}



template <typename T>
void SetElement(OperandStack& stack, T val) {
  int32_t index = stack.PopInt();
  auto arr_ref = stack.PopRef();
  if (arr_ref == nullptr) {
    LOG(ERROR) << "java.lang.NullPointerException";
    return;
  }
  if (index < 0 /*|| index >= arr_ref->arrayLength()*/) {
    LOG(FATAL) << "ArrayIndexOutOfBoundsException";
    return;
  }
  //arr_ref->setArrayElement<T>(index, val);
}

template <typename T>
void ArrayStore(runtime::Frame* frame) {
  
  OperandStack& stack = frame->GetOperandStack();
  if (std::is_same<T, int8_t>::value) {
    int8_t val = stack.PopInt();
    SetElement<int8_t>(stack, val);
  } else if (std::is_same<T, char16_t>::value) {
    char16_t val = stack.PopInt();
    SetElement<char16_t>(stack, val);
  } else if (std::is_same<T, int16_t>::value) {
    int16_t val = stack.PopInt();
    SetElement<int16_t>(stack, val);
  } else if (std::is_same<T, int32_t>::value) {
    int32_t val = stack.PopInt();
    SetElement<int32_t>(stack, val);
  } else if (std::is_same<T, int64_t>::value) {
    int64_t val = stack.PopLong();
    SetElement<int64_t>(stack, val);
  } else if (std::is_same<T, float>::value) {
    float val = stack.PopFloat();
    SetElement<float>(stack, val);
  } else if (std::is_same<T, double>::value) {
    double val = stack.PopDouble();
    SetElement<double>(stack, val);
  } else if (std::is_same<T, runtime::Object*>::value) {
    runtime::Object* val = stack.PopRef();
    SetElement<runtime::Object*>(stack, val);
  } 
}

template <typename T>
class STORE : public Index8Instruction {
  public:
  void Execute(runtime::Frame* frame) override {
    Store<T>(frame, uint16_t(index_));
  }
};
template<typename T>
class STORE_0 : public NoOperandsInstruction {
  public:
  void Execute(runtime::Frame* frame) override {
    Store<T>(frame, 0);
  }
};
template<typename T>
class STORE_1 : public NoOperandsInstruction {
  public:
  void Execute(runtime::Frame* frame) override {
    Store<T>(frame, 1);
  }
};
template<typename T>
class STORE_2 : public NoOperandsInstruction {
  public:
  void Execute(runtime::Frame* frame) override {
    Store<T>(frame, 2);
  }
};
template<typename T>
class STORE_3 : public NoOperandsInstruction {
  public:
  void Execute(runtime::Frame* frame) override {
    Store<T>(frame, 3);
  }
};
template<typename T>
class ASTORE : public NoOperandsInstruction {
  public:
  void Execute(runtime::Frame* frame) override {
    ArrayStore<T>(frame);
  }
};
}