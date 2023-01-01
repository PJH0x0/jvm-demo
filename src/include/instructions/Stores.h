#pragma once

#include <cstdint>
#include <memory>
#include <rtda/Frame.h>
#include <type_traits>
#include "base/Instruction.h"

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
  } else if (std::is_same<T, void*>::value) {
    void* val = stack.popRef();
    vars.setRef(index, val);
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
}