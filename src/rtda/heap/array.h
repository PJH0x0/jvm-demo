#pragma once
#include "object.h"

namespace rtda {


template <typename T>
class Array : public Object {
  private:
  std::vector<T> mArray;
  public:
  Array(std::shared_ptr<Class> classPtr, uint32_t count) : Object(classPtr) {
    mArray.resize(count);
  }
  const std::vector<T>& getArray() {
    return mArray;
  }
  uint32_t getArrayLength() {
    return mArray.size();
  }
  T& operator[](uint32_t index) {
    return mArray[index];
  }

};
} // namespace rtda