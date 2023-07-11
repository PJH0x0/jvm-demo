#include "java_lang_Object.h"
#include <runtime/oo/object.h>
#include <runtime/oo/class.h>
#include <runtime/class_loader.h>
namespace native {
void getClass(std::shared_ptr<runtime::Frame> frame) {
  auto thisObj = frame->getLocalVars().getThis();
  auto classPtr = thisObj->getClass();
  auto classObject = classPtr->getJClass();
  frame->getOperandStack().pushRef(classObject);
}
void hashCode(std::shared_ptr<runtime::Frame> frame) {
  auto thisObj = frame->getLocalVars().getThis();
  int32_t hash = (int32_t)(intptr_t)thisObj;
  frame->getOperandStack().pushInt(hash);
}
void clone(std::shared_ptr<runtime::Frame> frame) {
  auto thisObj = frame->getLocalVars().getThis();
  auto cloneable = thisObj->getClass()->getClassLoader()->loadClass("java/lang/Cloneable");
  if (!runtime::Class::isImplements(thisObj->getClass(), cloneable)) {
    LOG(ERROR) << "java.lang.CloneNotSupportedException";
    return;
  }
  runtime::Object* cloneObj = thisObj->clone();
  frame->getOperandStack().pushRef(cloneObj);
}
}// namespace native