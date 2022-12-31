#pragma once

#include <cstdint>
#include <cstring>
#include <vector>
#include "Slot.h"
namespace rtda {

class LocalVars {
  private:
  std::vector<Slot> slots;
  public:
  LocalVars(uint16_t _maxLocals) : slots(_maxLocals){}
  void setInt(uint16_t index, int32_t val) {
    if (index >= slots.capacity()) {
      LOG(FATAL) << "setInt out of index, index = " << index << ", maxIndex = " << slots.size();
    }
    slots[index].num = val;
  }
  int32_t getInt(uint16_t index) {
    if (index >= slots.size()) {
      LOG(FATAL) << "getInt out of index, index = " << index << ", maxIndex = " << slots.size();
    }
    return slots[index].num;
  }
  void setFloat(uint16_t index, float val) {
    if (index >= slots.capacity()) {
      LOG(FATAL) << "setFloat out of index, index = " << index << ", maxIndex = " << slots.size();
    }
    int32_t tmp = 0;
    memcpy(&tmp, &val, sizeof(val));
    slots[index].num = tmp;
  }
  float getFloat(uint16_t index) {
    if (index >= slots.size()) {
      LOG(FATAL) << "setFloat out of index, index = " << index << ", maxIndex = " << slots.size();
    }
    float tmp = 0.0f;
    memcpy(&tmp, &slots[index].num, sizeof(tmp));
    return tmp;
  }
  void setLong(uint16_t index, int64_t val) {
    if (index >= slots.capacity() - 1) {
      LOG(FATAL) << "setLong out of index, long need two slots, index = " << index << ", maxIndex = " << slots.size();
    }
    int32_t low = val & UINT32_MAX;
    int32_t high = (val >> 32) & UINT32_MAX;
    slots[index].num = low;
    slots[index+1].num = high;
  }
  int64_t getLong(uint16_t index) {
    if (index >= slots.size() - 1) {
      LOG(FATAL) << "getLong out of index, long need two slots, index = " << index << ", maxIndex = " << slots.size();
    }
    uint32_t low = slots[index].num;
    uint32_t high = slots[index+1].num;
    return int64_t(high) << 32 | int64_t(low);
  }
  void setDouble(uint16_t index, double val) {
    if (index >= slots.capacity() - 1) {
      LOG(FATAL) << "setDouble out of index, double need two slots, index = " << index << ", maxIndex = " << slots.size();
    }
    int64_t tmp = 0;
    memcpy(&tmp, &val, sizeof(val));
    setLong(index, tmp);
  }
  double getDouble(uint16_t index) {
    if (index >= slots.size() - 1) {
      LOG(FATAL) << "getDouble out of index, double need two slots, index = " << index << ", maxIndex = " << slots.size();
    }
    uint64_t tmp = getLong(index);
    double result = 0.0;
    memcpy(&result, &tmp, sizeof(result));
    return result;
  }
  void setRef(uint16_t index, void* ref) {
    if (index >= slots.capacity()) {
      LOG(FATAL) << "setRef out of index, index = " << index << ", maxIndex = " << slots.size();
    }
    slots[index].ref = reinterpret_cast<uintptr_t>(ref);
  }
  void* getRef(uint16_t index) {
    if (index >= slots.size()) {
      LOG(FATAL) << "getRef out of index, index = " << index << ", maxIndex = " << slots.size();
    }
    return reinterpret_cast<void*>(slots[index].ref);
  }
};
}