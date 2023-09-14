#include "java_lang_System.h"
#include <runtime/oo/object.h>
#include <runtime/oo/class.h>
#include <runtime/local_vars.h>

namespace native {
void ArrayCopy(runtime::Object* src, runtime::Object* dest,
               int32_t srcPos, int32_t destPos, int32_t length) {
  
  // switch(src->getArrayType()) {
  //   case runtime::kBoolean:
  //   case runtime::kByte:
  //     memcpy(dest->getArray<int8_t>() + destPos,
  //            src->getArray<int8_t>() + srcPos,
  //            length * sizeof(int8_t));
  //   case runtime::kChar:
  //     memcpy(dest->getArray<char16_t>() + destPos,
  //            src->getArray<char16_t>() + srcPos,
  //            length * sizeof(char16_t));
  //     //dest->setArrayElement<char16_t>(destPos + length, u'\0');
  //     break;
  //   case runtime::kShort:
  //     memcpy(dest->getArray<int16_t>() + destPos,
  //            src->getArray<int16_t>() + srcPos,
  //            length * sizeof(int16_t));
  //     break;
  //   case runtime::kInt:
  //     memcpy(dest->getArray<int32_t>() + destPos,
  //            src->getArray<int32_t>() + srcPos,
  //            length * sizeof(int32_t));
  //     break;
  //   case runtime::kFloat:
  //     memcpy(dest->getArray<float>() + destPos,
  //            src->getArray<float>() + srcPos,
  //            length * sizeof(float));
  //     break;
  //   case runtime::kDouble:
  //     memcpy(dest->getArray<double>() + destPos,
  //            src->getArray<double>() + srcPos,
  //            length * sizeof(double));
  //     break;
  //   case runtime::kLong:
  //     memcpy(dest->getArray<int64_t>() + destPos,
  //            src->getArray<int64_t>() + srcPos,
  //            length * sizeof(int64_t));
  //     break;
  //   case runtime::kObject:
  //     memcpy(dest->getArray<runtime::Object*>() + destPos,
  //            src->getArray<runtime::Object*>() + srcPos,
  //            length * sizeof(runtime::Object*));
  //     break;
  //   default:
  //     break;
  // }
}
bool CheckArrayCopy(runtime::Object* src, runtime::Object* dest) {
  auto srcClass = src->GetClass();
  auto destClass = dest->GetClass();
  if (!srcClass->IsArrayClass() || !destClass->IsArrayClass()) {
    return false;
  }
  if (srcClass->GetComponentClass()->IsPrimitive() ||
      destClass->GetComponentClass()->IsPrimitive()) {
    return srcClass == destClass;
  }
  return true;
}
void ArrayCopy(std::shared_ptr<runtime::Frame> frame) {
  auto vars = frame->GetLocalVars();
  auto src = vars.GetRef(0);
  auto srcPos = vars.GetInt(1);
  auto dest = vars.GetRef(2);
  auto destPos = vars.GetInt(3);
  auto length = vars.GetInt(4);

  if (src == nullptr || dest == nullptr) {
    throw std::runtime_error("java.lang.NullPointerException");
  }
  if (!CheckArrayCopy(src, dest)) {
    throw std::runtime_error("java.lang.ArrayStoreException");
  }
  // if (srcPos < 0 || destPos < 0 || length < 0 ||
  //     srcPos + length > src->arrayLength() ||
  //     destPos + length > dest->arrayLength()) {
  //   throw std::runtime_error("java.lang.IndexOutOfBoundsException");
  // }
  // memcpy(dest + destPos,
  //        src + srcPos,
  //        length * sizeof(runtime::Slot));
  ArrayCopy(src, dest, srcPos, destPos, length);
}
} // namespace native