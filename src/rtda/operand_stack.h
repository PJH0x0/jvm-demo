#pragma once

#include "slot.h"
#include <glog/logging.h>
#include <cstdint>
#include <cstring>
#include <ios>
#include <iostream>
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
    if (slots.size() <= 0) {
      LOG(FATAL) << "popInt operandStack empty";
    }
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
    if (slots.size() <= 0) {
      LOG(FATAL) << "popFloat operandStack empty";
    }
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
    if (slots.size() <= 1) {
      LOG(FATAL) << "popLong operandStack empty";
    }
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
    if (slots.size() <= 1) {
      LOG(FATAL) << "popDouble operandStack empty";
    }
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
    if (slots.size() <= 0) {
      LOG(FATAL) << "popRef operandStack empty";
    }
    Slot slot = slots.top();
    slots.pop();
    return reinterpret_cast<void*>(slot.ref);
  }

  void* getRefFromTop(uint32_t index) {
    if (slots.size() <= index) {
      LOG(FATAL) << "getRefFromTop operandStack size less than " << index;
    }
    std::stack<Slot> tmp;
    for (uint32_t i = 0; i < index; i++) {
      tmp.push(slots.top());
      slots.pop();
    }
    Slot slot = slots.top();
    void* ref = reinterpret_cast<void*>(slot.ref);
    while (!tmp.empty()) {
      slots.push(tmp.top());
      tmp.pop();
    }
    return ref;
  }

  void pushSlot(Slot slot) {
    if (slots.size() >= capacity) {
      LOG(FATAL) << "pushSlot operandStack overflow";
    }
    slots.push(slot);
  }
  Slot popSlot() {
    if (slots.size() <= 0) {
      LOG(FATAL) << "popSlot operandStack empty";
    }
    Slot slot = slots.top();
    slots.pop();
    return slot;
  }

  void dump() {
    std::vector<Slot> tmp;
    while (!slots.empty()) {
      tmp.push_back(slots.top());
      slots.pop();
    }
    std::cout << "--------- OperandStack --------\n";
    for (auto it = tmp.rbegin(); it != tmp.rend(); it++) {
      Slot slot = *it;
      if (slot.ref <= UINT32_MAX) {
        std::cout << "(" << slot.num << ") \n";
      } else {
        std::cout << " (0x" << std::hex << slot.ref << ") \n";
      }
      slots.push(slot);
    }
    std::cout << "--------- OperandStack END --------\n";
  }

};
}