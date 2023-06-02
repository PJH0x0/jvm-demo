#include "java_lang_Object.h"
#include <rtda/heap/object.h>
namespace native {
void getClass(std::shared_ptr<rtda::Frame> frame) {
  auto thisObj = frame->getLocalVars().getThis();
  auto classPtr = thisObj->getClass();
  auto classObject = classPtr->getJClass();
  frame->getOperandStack().pushRef(classObject);
}
void hashCode(std::shared_ptr<rtda::Frame> frame) {
  auto thisObj = frame->getLocalVars().getThis();
  int32_t hash = (int32_t)(intptr_t)thisObj;
  frame->getOperandStack().pushInt(hash);
}
}// namespace native