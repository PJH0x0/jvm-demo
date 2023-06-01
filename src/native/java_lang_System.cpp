#include "java_lang_System.h"
#include <rtda/heap/object.h>
#include <rtda/local_vars.h>

namespace native {
void arraycopy(rtda::Object* src, rtda::Object* dest, 
               int32_t srcPos, int32_t destPos, int32_t length) {
  
  switch(src->getArrayType()) {
    case rtda::AT_BOOLEAN:
    case rtda::AT_BYTE:
      memcpy(dest->getArray<int8_t>() + destPos,
             src->getArray<int8_t>() + srcPos,
             length * sizeof(int8_t));
    case rtda::AT_CHAR:
      memcpy(dest->getArray<char16_t>() + destPos,
             src->getArray<char16_t>() + srcPos,
             length * sizeof(char16_t));
      break;
    case rtda::AT_SHORT:
      memcpy(dest->getArray<int16_t>() + destPos,
             src->getArray<int16_t>() + srcPos,
             length * sizeof(int16_t));
      break;
    case rtda::AT_INT:
      memcpy(dest->getArray<int32_t>() + destPos,
             src->getArray<int32_t>() + srcPos,
             length * sizeof(int32_t));
      break;
    case rtda::AT_FLOAT:
      memcpy(dest->getArray<float>() + destPos,
             src->getArray<float>() + srcPos,
             length * sizeof(float));
      break;
    case rtda::AT_DOUBLE:
      memcpy(dest->getArray<double>() + destPos,
             src->getArray<double>() + srcPos,
             length * sizeof(double));
      break;
    case rtda::AT_LONG:
      memcpy(dest->getArray<int64_t>() + destPos,
             src->getArray<int64_t>() + srcPos,
             length * sizeof(int64_t));
      break;
    case rtda::AT_OBJECT:
      memcpy(dest->getArray<rtda::Object*>() + destPos,
             src->getArray<rtda::Object*>() + srcPos,
             length * sizeof(rtda::Object*));
      break;
    default:
      break;
  }
}
bool checkArrayCopy(rtda::Object* src, rtda::Object* dest) {
  auto srcClass = src->getClass();
  auto destClass = dest->getClass();
  if (!srcClass->isArrayClass() || !destClass->isArrayClass()) {
    return false;
  }
  if (srcClass->getComponentClass()->isPrimitive() ||
      destClass->getComponentClass()->isPrimitive()) {
    return srcClass == destClass;
  }
  return true;
}
void arraycopy(std::shared_ptr<rtda::Frame> frame) {
  auto vars = frame->getLocalVars();
  auto src = vars.getRef(0);
  auto srcPos = vars.getInt(1);
  auto dest = vars.getRef(2);
  auto destPos = vars.getInt(3);
  auto length = vars.getInt(4);

  if (src == nullptr || dest == nullptr) {
    throw std::runtime_error("java.lang.NullPointerException");
  }
  if (!checkArrayCopy(src, dest)) {
    throw std::runtime_error("java.lang.ArrayStoreException");
  }
  if (srcPos < 0 || destPos < 0 || length < 0 ||
      srcPos + length > src->arrayLength() ||
      destPos + length > dest->arrayLength()) {
    throw std::runtime_error("java.lang.IndexOutOfBoundsException");
  }
  memcpy(dest + destPos,
         src + srcPos,
         length * sizeof(rtda::Slot));
  //Array::arrayCopy(src, dest, srcPos, destPos, length);
}
} // namespace native