#pragma once

#include "slots.h"
#include <glog/logging.h>
#include <cstdint>
#include <cstring>
#include <ios>
#include <iostream>
#include <stack>
#include <vector>
namespace runtime {
class OperandStack {
public:
  explicit OperandStack(uint16_t max_size) : slots_(max_size), top_(0) {}
  void PushInt(int32_t val);
  int32_t PopInt();
  void PushFloat(float val);
  float PopFloat();
  void PushLong(int64_t val);
  int64_t PopLong();
  void PushDouble(double val);
  double PopDouble();
  void PushRef(Object* ref);
  Object* PopRef();
  Object* GetRefFromTop(uint32_t index);
  void PushSlot(Slot slot);
  Slot PopSlot();
  void Clear() {
    top_ = 0;
  }

  bool IsEmpty() const {
    return top_ <= 0;
  }
  bool IsFull() {
    return top_ >= slots_.GetSize();
  }
  bool IsLongEmpty() const {
    return top_ <= 1;
  }
  bool IsLongFull() {
    return top_ >= slots_.GetSize() - 1;
  }

  void Dump() {
    LOG(INFO) << "OperandStack size = " << slots_.GetSize();
    
  }
private:
  uint16_t top_;
  Slots slots_;
};
}