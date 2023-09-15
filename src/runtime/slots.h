#pragma once

#include <cstdint>
#include <cstring>
#include <vector>
#include <glog/logging.h>

namespace runtime {
class Object;
struct Slot {
  int32_t num;
};
class Slots {

public:
  explicit Slots(uint16_t count) : slots_(count) {
  }
  void SetInt(uint16_t index, int32_t val) {
    if (index >= slots_.capacity()) {
      LOG(FATAL) << "SetInt out of index, index = " << index << ", maxIndex = " << slots_.size();
    }
    slots_[index].num = val;

  }
  int32_t GetInt(uint16_t index) {
    if (index >= slots_.size()) {
      LOG(FATAL) << "GetInt out of index, index = " << index << ", maxIndex = " << slots_.size();
    }
    return slots_[index].num;
  }
  void SetFloat(uint16_t index, float val) {
    if (index >= slots_.capacity()) {
      LOG(FATAL) << "SetFloat out of index, index = " << index << ", maxIndex = " << slots_.size();
    }
    int32_t tmp = 0;
    memcpy(&tmp, &val, sizeof(val));
    slots_[index].num = tmp;
  }
  float GetFloat(uint16_t index) {
    if (index >= slots_.size()) {
      LOG(FATAL) << "SetFloat out of index, index = " << index << ", maxIndex = " << slots_.size();
    }
    float tmp = 0.0f;
    memcpy(&tmp, &slots_[index].num, sizeof(tmp));
    return tmp;
  }
  void SetLong(uint16_t index, int64_t val) {
    if (index >= slots_.capacity() - 1) {
      LOG(FATAL) << "SetLong out of index, long need two slots, index = " << index << ", maxIndex = " << slots_.size();
    }
    int32_t low = val & UINT32_MAX;
    int32_t high = (val >> 32) & UINT32_MAX;
    slots_[index].num = low;
    slots_[index + 1].num = high;
  }
  int64_t GetLong(uint16_t index) {
    if (index >= slots_.size() - 1) {
      LOG(FATAL) << "GetLong out of index, long need two slots, index = " << index << ", maxIndex = " << slots_.size();
    }
    uint32_t low = slots_[index].num;
    uint32_t high = slots_[index + 1].num;
    return int64_t(high) << 32 | int64_t(low);
  }
  void SetDouble(uint16_t index, double val) {
    if (index >= slots_.capacity() - 1) {
      LOG(FATAL) << "SetDouble out of index, double need two slots, index = " << index << ", maxIndex = " << slots_.size();
    }
    int64_t tmp = 0;
    memcpy(&tmp, &val, sizeof(val));
    SetLong(index, tmp);
  }
  double GetDouble(uint16_t index) {
    if (index >= slots_.size() - 1) {
      LOG(FATAL) << "GetDouble out of index, double need two slots, index = " << index << ", maxIndex = " << slots_.size();
    }
    uint64_t tmp = GetLong(index);
    double result = 0.0;
    memcpy(&result, &tmp, sizeof(result));
    return result;
  }
  void SetRef(uint16_t index, Object* ref) {
    SetLong(index, (int64_t) ref);
  }
  Object* GetRef(uint16_t index) {
    int64_t tmp = GetLong(index);
    return reinterpret_cast<Object*>(tmp);
  }
  void SetSlot(uint16_t index, Slot slot) {
    if (index >= slots_.capacity()) {
      LOG(FATAL) << "SetSlot out of index, index = " << index << ", maxIndex = " << slots_.size();
    }
    slots_[index] = slot;
  }
  Slot GetSlot(uint16_t index) {
    if (index >= slots_.size() - 1) {
      LOG(FATAL) << "GetSlot out of index, long need two slots, index = " << index << ", maxIndex = " << slots_.size();
    }
    return slots_[index];
  }
  size_t GetSize() const {
    return slots_.size();
  }
private:
  std::vector<Slot> slots_;
};
}