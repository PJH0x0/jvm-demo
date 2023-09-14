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
  explicit OperandStack(uint16_t max_size) : capacity_(max_size) {}
  void PushInt(int32_t val) {
    if (slots_.size() >= capacity_) {
      LOG(FATAL) << "PushInt operand_stack_ overflow";
    }
    Slot slot;
    slot.num = val;
    slots_.push(slot);
  }
  int32_t PopInt() {
    if (slots_.size() <= 0) {
      LOG(FATAL) << "PopInt operand_stack_ empty";
    }
    Slot slot = slots_.top();
    slots_.pop();
    return slot.num;
  }
  void PushFloat(float val) {
    if (slots_.size() >= capacity_) {
      LOG(FATAL) << "PushFloat operand_stack_ overflow";
    }
    Slot slot;
    memcpy(&slot.num, &val, sizeof(val));
    slots_.push(slot);
  }
  float PopFloat() {
    if (slots_.size() <= 0) {
      LOG(FATAL) << "PopFloat operand_stack_ empty";
    }
    Slot slot = slots_.top();
    float result = 0.0f;
    memcpy(&result, &slot.num, sizeof(result));
    slots_.pop();
    return result;
  }
  void PushLong(int64_t val) {
    if (slots_.size() >= capacity_ - 1) {
      LOG(FATAL) << "PushLong operand_stack_ overflow";
    }
    int32_t low = val & UINT32_MAX;
    int32_t high = (val >> 32) & UINT32_MAX;
    Slot low_slot;
    low_slot.num = low;
    slots_.push(low_slot);
    Slot high_slot;
    high_slot.num = high;
    slots_.push(high_slot);
  }
  int64_t PopLong() {
    if (slots_.size() <= 1) {
      LOG(FATAL) << "PopLong operand_stack_ empty";
    }
    Slot high_slot = slots_.top();
    uint32_t high = high_slot.num;
    slots_.pop();
    Slot low_slot = slots_.top();
    uint32_t low = low_slot.num;
    slots_.pop();
    return int64_t(high) << 32 | int64_t(low);
  }

  void PushDouble(double val) {
    if (slots_.size() >= capacity_ - 1) {
      LOG(FATAL) << "PushDouble operand_stack_ overflow";
    }
    int64_t tmp = 0;
    memcpy(&tmp, &val, sizeof(tmp));
    PushLong(tmp);
  }
  double PopDouble() {
    if (slots_.size() <= 1) {
      LOG(FATAL) << "PopDouble operand_stack_ empty";
    }
    int64_t tmp = PopLong();
    double result = 0.0;
    memcpy(&result, &tmp, sizeof(result));
    return result;
  }
  
  void PushRef(Object* ref) {
    if (slots_.size() >= capacity_) {
      LOG(FATAL) << "PushRef operand_stack_ overflow";
    }
    PushLong(reinterpret_cast<int64_t>(ref));
  }

  Object* PopRef() {
    if (slots_.size() <= 0) {
      LOG(FATAL) << "PopRef operand_stack_ empty";
    }
    int64_t tmp = PopLong();
    return reinterpret_cast<Object*>(tmp);
  }

  Object* GetRefFromTop(uint32_t index) {
    if (slots_.size() <= index) {
      LOG(FATAL) << "GetRefFromTop operand_stack_ size less than " << index;
    }
    std::stack<Slot> tmp;
    for (uint32_t i = 0; i < index; i++) {
      tmp.push(slots_.top());
      slots_.pop();
    }
    int64_t tmpLong = PopLong();
    Object* ref = reinterpret_cast<Object*>(tmpLong);
    while (!tmp.empty()) {
      slots_.push(tmp.top());
      tmp.pop();
    }
    return ref;
  }

  void PushSlot(Slot slot) {
    if (slots_.size() >= capacity_) {
      LOG(FATAL) << "PushSlot operand_stack_ overflow";
    }
    slots_.push(slot);
  }
  Slot PopSlot() {
    if (slots_.size() <= 0) {
      LOG(FATAL) << "PopSlot operand_stack_ empty";
    }
    Slot slot = slots_.top();
    slots_.pop();
    return slot;
  }
  void Clear() {
    while (!slots_.empty()) {
      slots_.pop();
    }
  }

  void Dump() {
    LOG(INFO) << "OperandStack size = " << slots_.size();
    
  }
private:
  uint16_t capacity_;
  std::stack<Slot> slots_;

};
}