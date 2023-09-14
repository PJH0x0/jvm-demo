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
  

public:
  Class* GetClass() const {
    return klass_;
  }
  void SetAge(uint8_t age) {
    age_ = age;
  }
  void SetForwarded(bool forwarded) {
    forwarded_ = forwarded;
  }
  void SetRemembered(bool remembered) {
    remembered_ = remembered;
  }
  void SetClass(Class* clazz) {
    klass_ = clazz;
  }
  bool IsInstanceOf(Class* clazz);
  
  void SetRefVar(std::string name, std::string descriptor, Object* ref);
  Object* GetRefVar(std::string name, std::string descriptor);
  Object* Clone();
private:
  uint8_t age_;
  bool forwarded_;
  bool remembered_;
  Class* klass_;
  uint32_t lock_;
};
struct DataObject : Object {
  private:
  Slot data_[0];
  public:
  static Object* Alloc(Thread* thread, Class* clazz, size_t obj_size);
  Slot* GetFields();
  Object* GetRefByName(std::string name, std::string descriptor);
  
  int32_t GetIntField(int slotId);
  void SetIntField(int slotId, int32_t value);
  int64_t GetLongField(int slot_id);
  void SetLongField(int slot_id, int64_t value);
  float GetFloatField(int slot_id);
  void SetFloatField(int slot_id, float value);
  double GetDoubleField(int slot_id);
  void SetDoubleField(int slotId, double value);
  Object* GetRefField(int slotId);
  void SetRefField(int slot_id, Object* value);
  Slot GetField(int slot_id);
  void SetField(int slotId, Slot value);

};
} // namespace runtime