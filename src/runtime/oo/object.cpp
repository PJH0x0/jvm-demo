#include "object.h"
#include "class.h"
#include "field.h"
#include <runtime/slots.h>
#include <memory>
#include <runtime/constant_pool.h>
#include <cstring>
#include <cstdint>

namespace runtime {
  bool Object::isInstanceOf(Class* clazz) {
    return Class::isAssignableFrom(clazz, kClass);
  }

  std::shared_ptr<Slots> Object::getField(int32_t slotId) {
    //return fields[slotId];
    return nullptr;
  }
  std::shared_ptr<Slots> Object::getFields() {
    return nullptr;
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


} // namespace runtime