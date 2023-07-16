#pragma once
#include "object.h"
#include <cstdint>
#include <cstddef>
class Thread;
namespace runtime {

enum ARRAY_TYPE {
  AT_BOOLEAN = 4,
  AT_CHAR = 5,
  AT_FLOAT = 6,
  AT_DOUBLE = 7,
  AT_BYTE = 8,
  AT_SHORT = 9,
  AT_INT = 10,
  AT_LONG = 11,
  AT_OBJECT = 12,//for reference
};

class Array : public Object {
  private:
  int32_t length;
  uint32_t firstElement[0];
  public:
  template<typename T>
  static constexpr T RoundUp(T x, T n) {
    return (x + n -1) & -n;
  }
  static constexpr size_t dataOffset(size_t elementSize) {
    return RoundUp(offsetof(Array, firstElement), elementSize);
  }
  int32_t getArrayLength() const {
    return length;
  }
  void setArrayLength(int32_t length) {
    this->length = length;
  }
  void* getRawData(size_t elementSize, int32_t index) {
    intptr_t data = reinterpret_cast<intptr_t>(this) 
        + static_cast<int32_t>(dataOffset(elementSize)) + (index * elementSize);
    return reinterpret_cast<void*>(data);
  }
};

template<typename T>
class PrimitiveArray : Array {
  public:
  static PrimitiveArray<T>* alloc(Thread* self, Class* clazz, int32_t length) {
    size_t size = sizeof(PrimitiveArray<T>) + (length * sizeof(T));
    PrimitiveArray<T>* array = (PrimitiveArray<T>*)DataObject::alloc(self, clazz, size);
    array->setArrayLength(length);
    return array;
  }
};

// Declare the different primitive arrays. Instantiations will be in array.cc.
extern template class PrimitiveArray<uint8_t>;   // BooleanArray
extern template class PrimitiveArray<int8_t>;    // ByteArray
extern template class PrimitiveArray<char16_t>;  // CharArray
extern template class PrimitiveArray<double>;    // DoubleArray
extern template class PrimitiveArray<float>;     // FloatArray
extern template class PrimitiveArray<int32_t>;   // IntArray
extern template class PrimitiveArray<int64_t>;   // LongArray
extern template class PrimitiveArray<int16_t>;   // ShortArray

template<typename T>
class ObjectArray : Array {
  static ObjectArray<T>* alloc(Thread* self, int32_t length);
};

}//namespace runtime