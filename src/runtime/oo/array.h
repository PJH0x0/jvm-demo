#pragma once
#include "object.h"
#include <cstdint>
#include <cstddef>

namespace runtime {

class Array : public Object {
  private:
  int32_t length;
  uint32_t firstElement[0];
  template<typename T>
  static constexpr T RoundUp(T x, T n) {
    return (x + n -1) & -n;
  }
  static constexpr size_t dataOffset(size_t elementSize) {
    return RoundUp(offsetof(Array, firstElement), elementSize);
  }
  public:
  int32_t getArrayLength() const {
    return length;
  }
  void* getRawData(size_t elementSize, int32_t index) {
    intptr_t data = reinterpret_cast<intptr_t>(this) 
        + static_cast<int32_t>(dataOffset(elementSize)) + (index * elementSize);
    return reinterpret_cast<void*>(data);
  }
};

template<typename T>
class PrimitiveArray : Array {

};

}//namespace runtime