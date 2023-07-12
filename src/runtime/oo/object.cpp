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
  bool Object::isInstanceOf(Class* clazz) {
    return Class::isAssignableFrom(clazz, kClass);
  }

  
  
  void Object::setRefVar(std::string name, std::string descriptor, Object* ref) {
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
    return JVM::current()->getHeap()->allocObject(thread, clazz, objSize);
  }

  Slots* DataObject::getFields() {
    return nullptr;
  }


} // namespace runtime