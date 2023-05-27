#include "object.h"
#include "field.h"
#include <cstring>

namespace rtda {
Object::Object(std::shared_ptr<Class> classPtr) : mClass(classPtr), mArray(nullptr), mArrayLength(0){
  mSlots = std::make_shared<Slots>(classPtr->getInstanceSlotCount());
}
Object::Object(std::shared_ptr<Class> classPtr, uint32_t count, ARRAY_TYPE type) : 
               mClass(classPtr), 
               mSlots(nullptr), 
               mArrayLength(count),
               mArrType(type) {
  switch (mArrType) {
    case AT_BOOLEAN:
    case AT_BYTE:
      mArray = malloc(count * sizeof(int8_t));
      break;
    case AT_CHAR:
      mArray = malloc((count + 1) * (sizeof(char16_t)));
      memset(mArray, 0, count + 1);
      break;
    case AT_SHORT:
      mArray = malloc(count * sizeof(int16_t));
      break;
    case AT_INT:
      mArray = malloc(count * sizeof(int32_t));
      break;
    case AT_LONG:
      mArray = malloc(count * sizeof(int64_t));
      break;
    case AT_FLOAT:
      mArray = malloc(count * sizeof(float));
      break;
    case AT_DOUBLE:
      mArray = malloc(count * sizeof(double));
      break;
    case AT_OBJECT:
      mArray = malloc(count * sizeof(Object*));
      break;
  }
  memset(mArray, 0, count);
}
void Object::setRefVar(std::string name, std::string descriptor, Object* ref) {
  auto field = mClass->getField(name, descriptor, false);
  mSlots->setRef(field->getSlotId(), ref);
}
Object* Object::getRefVar(std::string name, std::string descriptor) {
  auto field = mClass->getField(name, descriptor, false);
  return mSlots->getRef(field->getSlotId());
}
} // namespace rtda