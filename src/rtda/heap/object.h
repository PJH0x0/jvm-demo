#pragma once

#include "class.h"
#include <rtda/local_vars.h>
#include <vector>

namespace rtda {
struct Object {
  typedef rtda::LocalVars Slots; ;
  std::shared_ptr<Class> mClass;
  Slots mSlots;
  Object(std::shared_ptr<Class> classPtr) : mClass(classPtr), mSlots(classPtr->mInstanceSlotCount) {}
};
} // namespace rtda