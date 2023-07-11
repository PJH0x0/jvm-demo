#pragma once

#include <memory>
#include <vector>
#include <runtime/slots.h>


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
  Class* kClass;
  uint32_t lock;
  public:
  bool isInstanceOf(Class* clazz); 
  virtual std::shared_ptr<Slots> getField(int32_t slotId);
  std::shared_ptr<Slots> getFields();
  Class* getClass() {
    return kClass;
  }
  
  void setRefVar(std::string name, std::string descriptor, Object* ref);
  Object* getRefVar(std::string name, std::string descriptor);
  Object* clone();
};
struct DataObject : Object {
  uint32_t data[0];
};
} // namespace runtime