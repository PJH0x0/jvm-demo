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

  void* mExtra;//used for String, Class, Array
  
  public:
  Object(std::shared_ptr<Class> classPtr);
  Object(std::shared_ptr<Class> classPtr, uint32_t count, ARRAY_TYPE type);
  ARRAY_TYPE getArrayType() {
    return mArrType;
  }
  template<typename T>
  T* getArray() {
    return (T*)mArray;
  }
  template<typename T>
  void setArray(T* arr) {
    mArray = arr;
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
    return Class::isAssignableFrom(classPtr, mClass);
  }
  std::shared_ptr<Slots> getFields() {
    return mSlots;
  }
  std::shared_ptr<Class> getClass() {
    return mClass;
  }
  void setExtra(void* extra) {
    mExtra = extra;
  }
  void* getExtra() {
    return mExtra;
  }
  void setRefVar(std::string name, std::string descriptor, Object* ref);
  Object* getRefVar(std::string name, std::string descriptor);
  ~Object() {
    if (nullptr != mArray) {
      free(mArray);
      mArray = nullptr;
    }
  }
};
} // namespace rtda