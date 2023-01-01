#pragma once

#include <cstdint>
#include <memory>
#include <rtda/Frame.h>
#include <type_traits>
#include "base/Instruction.h"
namespace instructions {
using namespace rtda;
template <typename T>
inline void _load(std::shared_ptr<rtda::Frame> frame, uint16_t index) {
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
  } else if (std::is_same<T, void*>::value) {
    void* val = vars.getRef(index);
    stack.pushRef(val);
  }
}
template<typename T>
class LOAD : public Index8Instruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    _load<T>(frame, uint16_t(index));
  }
};
template<typename T>
class LOAD_0 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    _load<T>(frame, 0);
  }
};
template<typename T>
class LOAD_1 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    _load<T>(frame, 1);
  }
};
template<typename T>
class LOAD_2 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    _load<T>(frame, 2);
  }
};
template<typename T>
class LOAD_3 : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override {
    _load<T>(frame, 3);
  }
};
}