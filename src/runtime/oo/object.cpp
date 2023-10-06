#include "object.h"
#include "class.h"
#include "field.h"
#include <runtime/alloc/heap.h>
#include <runtime/thread.h>
#include <runtime/slots.h>
#include <memory>
#include <runtime/constant_pool.h>
#include <runtime/slots.h>
#include <jvm.h>
#include <cstring>
#include <cstdint>

namespace runtime {
  bool Object::IsInstanceOf(Class* clazz) {
    return Class::IsAssignableFrom(clazz, klass_);
  }

  
  
  void Object::SetRefVar(std::string name, std::string descriptor, Object* ref) {
    auto field = klass_->GetField(name, descriptor, false);
    //fields[name + descriptor] = ref;
  }
  Object* Object::GetRefVar(std::string name, std::string descriptor) {
    //return fields[name + descriptor];
    return nullptr;
  }
  Object* Object::Clone() {
    return nullptr;
  }

  runtime::Object* DataObject::Alloc(Thread *thread, Class *clazz, size_t obj_size) {
    return JVM::Current()->GetHeap()->AllocObject(thread, clazz, obj_size);
  }

  Slot* DataObject::GetFields() {
    return (Slot*)data_;
    //return nullptr;
  }
  Object* DataObject::GetRefByName(std::string name, std::string descriptor) {
    auto field = GetClass()->GetField(name, descriptor, false);
    int slot_id = field->GetSlotId();
    return GetRefField(slot_id);
  }
  Slot DataObject::GetField(int slot_id) {
    return *(data_ + slot_id * sizeof(Slot));
  }
  void DataObject::SetField(int slotId, Slot value) {
    *(data_ + slotId * sizeof(Slot)) = value;
  }
  int32_t DataObject::GetIntField(int slotId) {
    return GetField(slotId).num;
  }
  void DataObject::SetIntField(int slotId, int32_t value) {
    //GetField(slotId).num = val;
    Slot* slotPtr = data_ + slotId * sizeof(Slot);
    slotPtr->num = value;
  }
  int64_t DataObject::GetLongField(int slot_id) {
    //return GetField(slot_id).num;
    Slot* low = data_ + slot_id * sizeof(Slot);
    Slot* high = low + 1;
    return ((int64_t)high->num << 32) | low->num;
  }
  void DataObject::SetLongField(int slot_id, int64_t value) {
    //GetField(slot_id).num = val;
    Slot* low = data_ + slot_id * sizeof(Slot);
    Slot* high = low + 1;
    low->num = (int32_t)value;
    high->num = (int32_t)(value >> 32);
  }
  float DataObject::GetFloatField(int slot_id) {
    //return GetField(slot_id).num;
    Slot* slot_ptr = data_ + slot_id * sizeof(Slot);
    float tmp = 0.0f;
    memcpy(&tmp, &slot_ptr->num, sizeof(float));
    return tmp;
  }
  void DataObject::SetFloatField(int slot_id, float val) {
    //GetField(slot_id).num = val;
    Slot* slot_ptr = data_ + slot_id * sizeof(Slot);
    memcpy(&slot_ptr->num, &val, sizeof(float));
  }
  double DataObject::GetDoubleField(int slot_id) {
    //return GetField(slot_id).num;
    Slot* low = data_ + slot_id * sizeof(Slot);
    Slot* high = low + 1;
    int64_t tmp = ((int64_t)high->num << 32) | low->num;
    double ret = 0.0;
    memcpy(&ret, &tmp, sizeof(double));
    return ret;
  }
  void DataObject::SetDoubleField(int slotId, double val) {
    //GetField(slotId).num = val;
    Slot* low = data_ + slotId * sizeof(Slot);
    Slot* high = low + 1;
    int64_t tmp = 0;
    memcpy(&tmp, &val, sizeof(double));
    low->num = (int32_t)tmp;
    high->num = (int32_t)(tmp >> 32);
  }
  Object* DataObject::GetRefField(int slotId) {
    Slot* low = data_ + slotId * sizeof(Slot);
    Slot* high = low + 1;
    return (Object*)(((int64_t)high->num << 32) | low->num);
  }
  void DataObject::SetRefField(int slot_id, Object* value) {
    Slot* low = data_ + slot_id * sizeof(Slot);
    Slot* high = low + 1;
    int64_t tmp = (int64_t)value;
    low->num = (int32_t)tmp;
    high->num = (int32_t)(tmp >> 32);
  }
  
  


} // namespace runtime