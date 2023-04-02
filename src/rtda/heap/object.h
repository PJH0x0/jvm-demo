#pragma once

#include "class.h"
#include <memory>
#include <rtda/local_vars.h>
#include <vector>

namespace rtda {
struct Object {
  typedef rtda::LocalVars Slots; ;
  std::shared_ptr<Class> mClass;
  std::shared_ptr<Slots> mSlots;
  Object(std::shared_ptr<Class> classPtr) : mClass(classPtr) {
    mSlots = std::make_shared<Slots>(classPtr->mInstanceSlotCount);
  }
  bool isInstanceOf(std::shared_ptr<Class> classPtr) {
    return classPtr->isAssignableFrom(mClass);
  }
};
} // namespace rtda