#pragma once
#include "object.h"
#include <cstdint>
#include <cstddef>
class Thread;
namespace runtime {

enum ArrayType {
  kBoolean = 4,
  kChar = 5,
  kFloat = 6,
  kDouble = 7,
  kByte = 8,
  kShort = 9,
  kInt = 10,
  kLong = 11,
  kObject = 12,//for reference
};

class Array : public Object {
  private:
  int32_t length_;
  uint32_t first_element_[0];
  public:
  template<typename T>
  static constexpr T RoundUp(T x, T n) {
    return (x + n -1) & -n;
  }
  static constexpr size_t DataOffset(size_t elementSize) {
    return RoundUp(offsetof(Array, first_element_), elementSize);
  }
  int32_t GetArrayLength() const {
    return length_;
  }
  void SetArrayLength(int32_t length) {
    this->length_ = length;
  }
  void* GetRawData(size_t elementSize, int32_t index) {
    intptr_t data = reinterpret_cast<intptr_t>(this)
                    + static_cast<int32_t>(DataOffset(elementSize)) + (index * elementSize);
    return reinterpret_cast<void*>(data);
  }
};

template<typename T>
class PrimitiveArray : Array {
  public:
  static PrimitiveArray<T>* Alloc(Thread* self, Class* clazz, int32_t length) {
    size_t size = sizeof(PrimitiveArray<T>) + (length * sizeof(T));
    PrimitiveArray<T>* array = (PrimitiveArray<T>*) DataObject::Alloc(self, clazz, size);
      array->SetArrayLength(length);
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
  static ObjectArray<T>* Alloc(Thread* self, int32_t length);
};

}//namespace runtime