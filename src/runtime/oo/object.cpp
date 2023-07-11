#include "object.h"
#include "field.h"
#include <runtime/constant_pool.h>
#include <cstring>
#include <cstdint>

namespace runtime {
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
      memset(mArray, 0, count * sizeof(int8_t));
      break;
    case AT_CHAR: {
      mArray = malloc((count + 1) * (sizeof(char16_t)));
      memset(mArray, 0, count + 1);
      char16_t* arr = static_cast<char16_t*>(mArray);
      arr[count] = u'\0';
      break;
    }
    case AT_SHORT:
      mArray = malloc(count * sizeof(int16_t));
      memset(mArray, 0, count * sizeof(int16_t));
      break;
    case AT_INT:
      mArray = malloc(count * sizeof(int32_t));
      memset(mArray, 0, count * sizeof(int32_t));
      break;
    case AT_LONG:
      mArray = malloc(count * sizeof(int64_t));
      memset(mArray, 0, count * sizeof(int64_t));
      break;
    case AT_FLOAT:
      mArray = malloc(count * sizeof(float));
      memset(mArray, 0, count * sizeof(float));
      break;
    case AT_DOUBLE:
      mArray = malloc(count * sizeof(double));
      memset(mArray, 0, count * sizeof(double));
      break;
    case AT_OBJECT:
      mArray = malloc(count * sizeof(Object*));
      memset(mArray, 0, count * sizeof(Object*));
      break;
  }
}
void Object::setRefVar(std::string name, std::string descriptor, Object* ref) {
  auto field = mClass->getField(name, descriptor, false);
  mSlots->setRef(field->getSlotId(), ref);
}
Object* Object::getRefVar(std::string name, std::string descriptor) {
  auto field = mClass->getField(name, descriptor, false);
  return mSlots->getRef(field->getSlotId());
}
Object* Object::clone() {
  if (mClass->isArrayClass()) {
    Object* cloneObj = new Object(mClass, mArrayLength, mArrType);
    switch (mArrType) {
      case AT_BOOLEAN:
      case AT_BYTE: {
        int8_t* cloneArr = cloneObj->getArray<int8_t>();
        int8_t* arr = getArray<int8_t>();
        memcpy(cloneArr, arr, mArrayLength * sizeof(int8_t));
        break;
      }
      case AT_CHAR: {
        char16_t* cloneArr = cloneObj->getArray<char16_t>();
        char16_t* arr = getArray<char16_t>();
        memcpy(cloneArr, arr, mArrayLength * sizeof(char16_t));
        //cloneArr[mArrayLength] = u'\0';
        
        break;
      }
      case AT_SHORT: {
        int16_t* cloneArr = cloneObj->getArray<int16_t>();
        int16_t* arr = getArray<int16_t>();
        memcpy(cloneArr, arr, mArrayLength * sizeof(int16_t));
        break;
      }
      case AT_INT: {
        int32_t* cloneArr = cloneObj->getArray<int32_t>();
        int32_t* arr = getArray<int32_t>();
        memcpy(cloneArr, arr, mArrayLength * sizeof(int32_t));
        break;
      }
      case AT_LONG: {
        int64_t* cloneArr = cloneObj->getArray<int64_t>();
        int64_t* arr = getArray<int64_t>();
        memcpy(cloneArr, arr, mArrayLength * sizeof(int64_t));
        break;
      }
      case AT_FLOAT: {
        float* cloneArr = cloneObj->getArray<float>();
        float* arr = getArray<float>();
        memcpy(cloneArr, arr, mArrayLength * sizeof(float));
        break;
      }
      case AT_DOUBLE: {
        double* cloneArr = cloneObj->getArray<double>();
        double* arr = getArray<double>();
        memcpy(cloneArr, arr, mArrayLength * sizeof(double));
        break;
      }
      case AT_OBJECT: {
        Object** cloneArr = cloneObj->getArray<Object*>();
        Object** arr = getArray<Object*>();
        memcpy(cloneArr, arr, mArrayLength * sizeof(Object*));
        break;
      }
      default:
        break;
    }
    return cloneObj;
  } else {
    Object* cloneObj = new Object(mClass);
    Slots* cloneSlots = cloneObj->getFields().get();
    Slots* slots = mSlots.get();
    memcpy(cloneSlots, slots, sizeof(Slots));
    return cloneObj;
  }
}
} // namespace runtime