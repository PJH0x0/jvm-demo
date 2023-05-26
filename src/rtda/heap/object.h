#pragma once

#include "class.h"
#include <memory>
#include <vector>
#include <rtda/slots.h>


namespace rtda {
enum ARRAY_TYPE {
  AT_BOOLEAN = 4,
  AT_CHAR = 5,
  AT_FLOAT = 6,
  AT_DOUBLE = 7,
  AT_BYTE = 8,
  AT_SHORT = 9,
  AT_INT = 10,
  AT_LONG = 11,
  AT_OBJECT = 12,//for reference
};
struct Object {
  
  private:
  std::shared_ptr<Class> mClass;
  std::shared_ptr<Slots> mSlots;//used for non-array object
  void* mArray;//used for array object
  uint32_t mArrayLength;
  ARRAY_TYPE mArrType;
  
  public:
  Object(std::shared_ptr<Class> classPtr) : mClass(classPtr), mArray(nullptr), mArrayLength(0){
    mSlots = std::make_shared<Slots>(classPtr->getInstanceSlotCount());
  }
  Object(std::shared_ptr<Class> classPtr, uint32_t count, ARRAY_TYPE type) : 
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
        mArray = malloc(count * sizeof(uint16_t));
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
  }
  ARRAY_TYPE getArrayType() {
    return mArrType;
  }
  template<typename T>
  T* getArray() {
    return (T*)mArray;
  }
  template<typename T>
  T getArrayElement(uint32_t index) {
    return getArray<T>()[index];
  }
  template<typename T>
  void setArrayElement(uint32_t index, T val) {
    getArray<T>()[index] = val;
  }
  uint32_t arrayLength() {
    return mArrayLength;
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
  ~Object() {
    if (nullptr != mArray) {
      free(mArray);
    }
  }
};
} // namespace rtda