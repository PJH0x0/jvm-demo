#pragma once

#include "class.h"
#include <memory>
#include <rtda/local_vars.h>
#include <vector>


namespace rtda {
class Object {
  typedef LocalVars Slots;
  private:
  std::shared_ptr<Class> mClass;
  std::shared_ptr<Slots> mSlots;
  public:
  Object(std::shared_ptr<Class> classPtr) : mClass(classPtr) {
    mSlots = std::make_shared<Slots>(classPtr->getInstanceSlotCount());
  }
  bool isInstanceOf(std::shared_ptr<Class> classPtr) {
    return classPtr->isAssignableFrom(mClass);
  }
  std::shared_ptr<Slots> getFields() {
    return mSlots;
  }
  std::shared_ptr<Class> getClass() {
    return mClass;
  }
  ~Object() {}
};
} // namespace rtda