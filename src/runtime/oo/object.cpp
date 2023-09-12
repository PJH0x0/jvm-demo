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
#include <sys/_types/_int64_t.h>

namespace runtime {
  bool Object::isInstanceOf(Class* clazz) {
    return Class::IsAssignableFrom(clazz, kClass);
  }

  
  
  void Object::setRefVar(std::string name, std::string descriptor, Object* ref) {
    auto field = kClass->GetField(name, descriptor, false);
    //fields[name + descriptor] = ref;
  }
  Object* Object::getRefVar(std::string name, std::string descriptor) {
    //return fields[name + descriptor];
    return nullptr;
  }
  Object* Object::clone() {
    return nullptr;
  }

  runtime::Object* DataObject::alloc(Thread *thread, Class *clazz, size_t objSize) {
    return JVM::current()->getHeap()->AllocObject(thread, clazz, objSize);
  }

  Slot* DataObject::getFields() {
    return (Slot*)data;
    //return nullptr;
  }
  Object* DataObject::getRefByName(std::string name, std::string descriptor) {
    auto field = getClass()->GetField(name, descriptor, false);
    int slotId = field->getSlotId();
    return getRefField(slotId);
  }
  Slot DataObject::getField(int slotId) {
    return *(data + slotId * sizeof(Slot));
  }
  void DataObject::setField(int slotId, Slot val) {
    *(data + slotId * sizeof(Slot)) = val;
  }
  int32_t DataObject::getIntField(int slotId) {
    return getField(slotId).num;
  }
  void DataObject::setIntField(int slotId, int32_t val) {
    //GetField(slotId).num = val;
    Slot* slotPtr = data + slotId * sizeof(Slot);
    slotPtr->num = val;
  }
  int64_t DataObject::getLongField(int slotId) {
    //return GetField(slotId).num;
    Slot* low = data + slotId * sizeof(Slot);
    Slot* high = low + 1;
    return ((int64_t)high->num << 32) | low->num;
  }
  void DataObject::setLongField(int slotId, int64_t val) {
    //GetField(slotId).num = val;
    Slot* low = data + slotId * sizeof(Slot);
    Slot* high = low + 1;
    low->num = (int32_t)val;
    high->num = (int32_t)(val >> 32);
  }
  float DataObject::getFloatField(int slotId) {
    //return GetField(slotId).num;
    Slot* slotPtr = data + slotId * sizeof(Slot);
    float tmp = 0.0f;
    memcpy(&tmp, &slotPtr->num, sizeof(float));
    return tmp;
  }
  void DataObject::setFloatField(int slotId, float val) {
    //GetField(slotId).num = val;
    Slot* slotPtr = data + slotId * sizeof(Slot);
    memcpy(&slotPtr->num, &val, sizeof(float));
  }
  double DataObject::getDoubleField(int slotId) {
    //return GetField(slotId).num;
    Slot* low = data + slotId * sizeof(Slot);
    Slot* high = low + 1;
    int64_t tmp = ((int64_t)high->num << 32) | low->num;
    double ret = 0.0;
    memcpy(&ret, &tmp, sizeof(double));
    return ret;
  }
  void DataObject::setDoubleField(int slotId, double val) {
    //GetField(slotId).num = val;
    Slot* low = data + slotId * sizeof(Slot);
    Slot* high = low + 1;
    int64_t tmp = 0;
    memcpy(&tmp, &val, sizeof(double));
    low->num = (int32_t)tmp;
    high->num = (int32_t)(tmp >> 32);
  }
  Object* DataObject::getRefField(int slotId) {
    Slot* low = data + slotId * sizeof(Slot);
    Slot* high = low + 1;
    return (Object*)(((int64_t)high->num << 32) | low->num);
  }
  void DataObject::setRefField(int slotId, Object* val) {
    Slot* low = data + slotId * sizeof(Slot);
    Slot* high = low + 1;
    int64_t tmp = (int64_t)val;
    low->num = (int32_t)tmp;
    high->num = (int32_t)(tmp >> 32);
  }
  
  


} // namespace runtime