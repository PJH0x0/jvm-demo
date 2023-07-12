#pragma once

#include <memory>
#include <vector>
#include <runtime/slots.h>
#include <runtime/thread.h>


namespace runtime {
class Class;
template<class T> class PrimitiveArray;
using BooleanArray = PrimitiveArray<uint8_t>;
using ByteArray = PrimitiveArray<int8_t>;
using CharArray = PrimitiveArray<char16_t>;
using DoubleArray = PrimitiveArray<double>;
using FloatArray = PrimitiveArray<float>;
using IntArray = PrimitiveArray<int32_t>;
using LongArray = PrimitiveArray<int64_t>;
using ShortArray = PrimitiveArray<int16_t>; 

struct Object {
  
  private:
  uint8_t mAge;
  bool mForwarded;
  bool mRemembered;
  Class* kClass;
  uint32_t mLock;
  public:
  void setAge(uint8_t age) {
    mAge = age;
  }
  void setForwarded(bool forwarded) {
    mForwarded = forwarded;
  }
  void setRemembered(bool remembered) {
    mRemembered = remembered;
  }
  void setClass(Class* clazz) {
    kClass = clazz;
  }
  bool isInstanceOf(Class* clazz); 
  
  Class* getClass() {
    return kClass;
  }
  
  void setRefVar(std::string name, std::string descriptor, Object* ref);
  Object* getRefVar(std::string name, std::string descriptor);
  Object* clone();
};
struct DataObject : Object {
  private:
  Slot data[0];
  public:
  static runtime::Object* alloc(Thread* thread, Class* clazz, size_t objSize);
  Slots* getFields();
};
} // namespace runtime