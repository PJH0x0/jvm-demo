#pragma once

#include <cstdint>
#include <cstring>
#include <sys/_types/_int64_t.h>
#include <sys/_types/_size_t.h>
#include <vector>
#include <glog/logging.h>

namespace runtime {
class Object;
struct Slot {
  int32_t num;
};
class Slots {
  private:
  std::vector<Slot> slots;
  public:
  Slots(uint16_t count) : slots(count) {
  }
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
  void setRef(uint16_t index, Object* ref) {
    setLong(index, (int64_t)ref);
  }
  Object* getRef(uint16_t index) {
    int64_t tmp = getLong(index);
    return reinterpret_cast<Object*>(tmp);
  }
  void setSlot(uint16_t index, Slot slot) {
    if (index >= slots.capacity()) {
      LOG(FATAL) << "setSlot out of index, index = " << index << ", maxIndex = " << slots.size();
    }
    slots[index] = slot;
  }
};
}