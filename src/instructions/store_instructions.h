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
void _astore(std::shared_ptr<rtda::Frame> frame) {
  OperandStack& stack = frame->getOperandStack();
  int32_t index = stack.popInt();
  auto arrRef = stack.popRef();
  if (arrRef == nullptr) {
    throw std::runtime_error("java.lang.NullPointerException");
  }
  if (index < 0 || index >= arrRef->arrayLength()) {
    throw std::runtime_error("ArrayIndexOutOfBoundsException");
  }
  switch(arrRef->getArrayType()) {
    case rtda::ARRAY_TYPE::AT_BOOLEAN:
    case rtda::ARRAY_TYPE::AT_BYTE:
      arrRef->setArrayElement<int8_t>(index, stack.popInt());
      break;
    case rtda::ARRAY_TYPE::AT_CHAR:
      arrRef->setArrayElement<uint16_t>(index, stack.popInt());
      break;
    case rtda::ARRAY_TYPE::AT_SHORT:
      arrRef->setArrayElement<int16_t>(index, stack.popInt());
      break;
    case rtda::ARRAY_TYPE::AT_INT:
      arrRef->setArrayElement<int32_t>(index, stack.popInt());
      break;
    case rtda::ARRAY_TYPE::AT_FLOAT:
      arrRef->setArrayElement<float>(index, stack.popFloat());
      break;
    case rtda::ARRAY_TYPE::AT_DOUBLE:
      arrRef->setArrayElement<double>(index, stack.popDouble());
      break;
    case rtda::ARRAY_TYPE::AT_LONG:
      arrRef->setArrayElement<int64_t>(index, stack.popLong());
      break;
    case rtda::ARRAY_TYPE::AT_OBJECT:
      arrRef->setArrayElement<rtda::Object*>(index, stack.popRef());
      break;
    default:
      break;
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