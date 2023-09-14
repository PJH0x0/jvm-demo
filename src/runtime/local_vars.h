#pragma once

#include <climits>
#include <cstdint>
#include <cstring>
#include <vector>
#include "slots.h"
#include <glog/logging.h>
#include <iostream>
namespace runtime {
class LocalVars {
public:
  explicit LocalVars(uint16_t max_locals) : slots_(max_locals){}
  void SetInt(uint16_t index, int32_t val) {
    slots_.SetInt(index, val);
  }
  int32_t GetInt(uint16_t index) {
    return slots_.GetInt(index);
  }
  void SetFloat(uint16_t index, float val) {
    slots_.SetFloat(index, val);
  }
  float GetFloat(uint16_t index) {
    return slots_.GetFloat(index);
  }
  void SetLong(uint16_t index, int64_t val) {
    slots_.SetLong(index, val);
  }
  int64_t GetLong(uint16_t index) {
    return slots_.GetLong(index);
  }
  void SetDouble(uint16_t index, double val) {
    slots_.SetDouble(index, val);
  }
  double GetDouble(uint16_t index) {
    return slots_.GetDouble(index);
  }
  void SetRef(uint16_t index, Object* ref) {
    slots_.SetRef(index, ref);
  }
  Object* GetRef(uint16_t index) {
    return slots_.GetRef(index);
  }
  Object* GetThis() {
    return GetRef(0);
  }
  void SetSlot(uint16_t index, Slot slot) {
    slots_.SetSlot(index, slot);
  }
  void Dump(){}
private:
  Slots slots_;
};
}