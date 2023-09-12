#include "java_lang_Object.h"
#include <runtime/oo/object.h>
#include <runtime/oo/class.h>
#include <runtime/class_loader.h>
namespace native {
void GetClass(std::shared_ptr<runtime::Frame> frame) {
  auto thisObj = frame->getLocalVars().getThis();
  auto classPtr = thisObj->getClass();
  auto classObject = static_cast<runtime::Object*>(classPtr);
  frame->getOperandStack().pushRef(classObject);
}
void HashCode(std::shared_ptr<runtime::Frame> frame) {
  auto thisObj = frame->getLocalVars().getThis();
  int32_t hash = (int32_t)(intptr_t)thisObj;
  frame->getOperandStack().pushInt(hash);
}
void Clone(std::shared_ptr<runtime::Frame> frame) {
  auto thisObj = frame->getLocalVars().getThis();
  auto cloneable = thisObj->getClass()->GetClassLoader()->loadClass("java/lang/Cloneable");
  if (!runtime::Class::IsImplements(thisObj->getClass(), cloneable)) {
    LOG(ERROR) << "java.lang.CloneNotSupportedException";
    return;
  }
  runtime::Object* cloneObj = thisObj->clone();
  frame->getOperandStack().pushRef(cloneObj);
}
}// namespace native