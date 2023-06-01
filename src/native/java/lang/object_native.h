#include "rtda/frame.h"
#include "rtda/heap/object.h"
#include <memory>
#include <native/native_method.h>
#include <memory>
#include <rtda/frame.h>
namespace native {
void getClass(std::shared_ptr<rtda::Frame> frame) {
  auto thisObj = frame->getLocalVars().getThis();
  auto classPtr = thisObj->getExtra();
  frame->getOperandStack().pushRef((rtda::Object*)classPtr);
}
void init() {
  registerNativeMethod("java/lang/Objec", "getClass", "()Ljava/lang/Class", getClass);
}
}