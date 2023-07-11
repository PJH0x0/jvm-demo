#pragma once

#include "class.h"
#include <memory>
#include <vector>
#include <runtime/slots.h>


namespace runtime {
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
  Class* mClass;
  std::shared_ptr<Slots> mSlots;//used for non-array object
  void* mArray;//used for array object
  const uint32_t mArrayLength;
  ARRAY_TYPE mArrType;

  void* mExtra;//used for String, Class, Array
  
  public:
  Object(Class* clazz);
  Object(Class* classPtr, uint32_t count, ARRAY_TYPE type);
  ARRAY_TYPE getArrayType() {
    return mArrType;
  }
  template<typename T>
  T* getArray() {
    return (T*)mArray;
  }
  // template<typename T>
  // void setArray(T* arr) {
  //   LOG(INFO) << "set array " << (uintptr_t)arr << " " << (uintptr_t)mArray;
  //   mArray = arr;
  // }
  template<typename T>
  T getArrayElement(uint32_t index) {
    return getArray<T>()[index];
  }
  template<typename T>
  void setArrayElement(uint32_t index, T val) {
    getArray<T>()[index] = val;
  }
  uint32_t arrayLength() const {
    return mArrayLength;
  }
  bool isInstanceOf(Class* classPtr) {
    return Class::isAssignableFrom(classPtr, mClass);
  }
  std::shared_ptr<Slots> getFields() {
    return mSlots;
  }
  Class* getClass() {
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
  Object* clone();
  ~Object() {
    if (nullptr != mArray) {
      free(mArray);
      mArray = nullptr;
    }
  }
};
} // namespace runtime