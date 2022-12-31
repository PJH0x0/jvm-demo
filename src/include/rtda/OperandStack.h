#pragma once

#include "Slot.h"
#include <cstdint>
#include <cstring>
#include <stack>
#include <vector>
namespace rtda {
class OperandStack {
  private:
  uint16_t capacity;
  std::stack<Slot> slots;
  public:
  OperandStack(uint16_t maxSize) : capacity(maxSize) {}
  void pushInt(int32_t val) {
    if (slots.size() >= capacity) {
      LOG(FATAL) << "pushInt operandStack overflow";
    }
    Slot slot;
    slot.num = val;
    slots.push(slot);
  }
  int32_t popInt() {
    Slot slot = slots.top();
    slots.pop();
    return slot.num;
  }
  void pushFloat(float val) {
    if (slots.size() >= capacity) {
      LOG(FATAL) << "pushFloat operandStack overflow";
    }
    Slot slot;
    memcpy(&slot.num, &val, sizeof(val));
    slots.push(slot);
  }
  float popFloat() {
    Slot slot = slots.top();
    float result = 0.0f;
    memcpy(&result, &slot.num, sizeof(result));
    slots.pop();
    return result;
  }
  void pushLong(int64_t val) {
    if (slots.size() >= capacity - 1) {
      LOG(FATAL) << "pushLong operandStack overflow";
    }
    int32_t low = val & UINT32_MAX;
    int32_t high = (val >> 32) & UINT32_MAX;
    Slot lowSlot;
    lowSlot.num = low;
    slots.push(lowSlot);
    Slot highSlot;
    highSlot.num = high;
    slots.push(highSlot);
  }
  int64_t popLong() {
    Slot highSlot = slots.top();
    uint32_t high = highSlot.num;
    slots.pop();
    Slot lowSlot = slots.top();
    uint32_t low = lowSlot.num;
    slots.pop();
    return int64_t(high) << 32 | int64_t(low);
  }

  void pushDouble(double val) {
    if (slots.size() >= capacity - 1) {
      LOG(FATAL) << "pushDouble operandStack overflow";
    }
    int64_t tmp = 0;
    memcpy(&tmp, &val, sizeof(tmp));
    pushLong(tmp);
  }
  double popDouble() {
    int64_t tmp = popLong();
    double result = 0.0;
    memcpy(&result, &tmp, sizeof(result));
    return result;
  }
  
  void pushRef(void* ref) {
    if (slots.size() >= capacity) {
      LOG(FATAL) << "pushRef operandStack overflow";
    }
    Slot slot;
    slot.ref = reinterpret_cast<uintptr_t>(ref);
    slots.push(slot);
  }

  void* popRef() {
    Slot slot = slots.top();
    slots.pop();
    return reinterpret_cast<void*>(slot.ref);
  }  

};
}