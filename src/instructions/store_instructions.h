#pragma once

#include <cstdint>
#include <memory>
#include <rtda/frame.h>
#include <type_traits>
#include "base/base_instructions.h"
#include <rtda/slots.h>
#include <rtda/heap/object.h>

namespace instructions {
using rtda::Frame;
using rtda::LocalVars;
using rtda::OperandStack;
template <typename T>
inline void _store(std::shared_ptr<rtda::Frame> frame, uint16_t index) {
  LocalVars& vars = frame->getLocalVars();
  OperandStack& stack = frame->getOperandStack();
  if (std::is_same<T, int32_t>::value) {
    int32_t val = stack.popInt();
    vars.setInt(index, val);
  } else if (std::is_same<T, int64_t>::value) {
    int64_t val = stack.popLong();
    vars.setLong(index, val);
  } else if (std::is_same<T, float>::value) {
    float val = stack.popFloat();
    vars.setFloat(index, val);
  } else if (std::is_same<T, double>::value) {
    double val = stack.popDouble();
    vars.setDouble(index, val);
  } else if (std::is_same<T, rtda::Object*>::value) {
    rtda::Object* val = stack.popRef();
    vars.setRef(index, val);
  }
}



template <typename T>
void _astore(OperandStack& stack, T val) {
  int32_t index = stack.popInt();
  auto arrRef = stack.popRef();
  if (arrRef == nullptr) {
    LOG(ERROR) << "java.lang.NullPointerException";
    return;
  }
  if (index < 0 || index >= arrRef->arrayLength()) {
    LOG(ERROR) << "ArrayIndexOutOfBoundsException";
    return;
  }
  arrRef->setArrayElement<T>(index, val);
}

template <typename T>
void _astore(std::shared_ptr<rtda::Frame> frame) {
  OperandStack& stack = frame->getOperandStack();
  if (std::is_same<T, int8_t>::value) {
    int8_t val = stack.popInt();
    _astore<int8_t>(stack, val);
  } else if (std::is_same<T, uint16_t>::value) {
    uint16_t val = stack.popInt();
    _astore<uint16_t>(stack, val);
  } else if (std::is_same<T, int16_t>::value) {
    int16_t val = stack.popInt();
    _astore<int16_t>(stack, val);
  } else if (std::is_same<T, int32_t>::value) {
    int32_t val = stack.popInt();
    _astore<int32_t>(stack, val);
  } else if (std::is_same<T, int64_t>::value) {
    int64_t val = stack.popLong();
    _astore<int64_t>(stack, val);
  } else if (std::is_same<T, float>::value) {
    float val = stack.popFloat();
    _astore<float>(stack, val);
  } else if (std::is_same<T, double>::value) {
    double val = stack.popDouble();
    _astore<double>(stack, val);
  } else if (std::is_same<T, rtda::Object*>::value) {
    rtda::Object* val = stack.popRef();
    _astore<rtda::Object*>(stack, val);
  } 
}

template <typename T>
class STORE : public Index8Instruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    _store<T>(frame, uint16_t(index));
  }
};
template<typename T>
class STORE_0 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    _store<T>(frame, 0);
  }
};
template<typename T>
class STORE_1 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    _store<T>(frame, 1);
  }
};
template<typename T>
class STORE_2 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    _store<T>(frame, 2);
  }
};
template<typename T>
class STORE_3 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    _store<T>(frame, 3);
  }
};
template<typename T>
class ASTORE : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    _astore<T>(frame);
  }
};
}