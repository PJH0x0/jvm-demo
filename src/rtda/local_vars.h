#pragma once

#include <climits>
#include <cstdint>
#include <cstring>
#include <vector>
#include "slots.h"
#include <glog/logging.h>
#include <iostream>
namespace rtda {
class LocalVars {
  private:
  Slots slots;
  public:
  LocalVars(uint16_t _maxLocals) : slots(_maxLocals){}
  void setInt(uint16_t index, int32_t val) {
    slots.setInt(index, val);
  }
  int32_t getInt(uint16_t index) {
    return slots.getInt(index);
  }
  void setFloat(uint16_t index, float val) {
    slots.setFloat(index, val);
  }
  float getFloat(uint16_t index) {
    return slots.getFloat(index);
  }
  void setLong(uint16_t index, int64_t val) {
    slots.setLong(index, val);
  }
  int64_t getLong(uint16_t index) {
    return slots.getLong(index);
  }
  void setDouble(uint16_t index, double val) {
    slots.setDouble(index, val);
  }
  double getDouble(uint16_t index) {
    return slots.getDouble(index);
  }
  void setRef(uint16_t index, Object* ref) {
    slots.setRef(index, ref);
  }
  Object* getRef(uint16_t index) {
    return slots.getRef(index);
  }
  Object* getThis() {
    return getRef(0);
  }
  void setSlot(uint16_t index, Slot slot) {
    slots.setSlot(index, slot);
  }
  void dump(){}
};
}