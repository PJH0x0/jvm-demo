#include "java_lang_Throwable.h"

#include <memory>
#include <rtda/frame.h>
#include <rtda/heap/object.h>
#include <rtda/heap/method.h>
#include <cstdint>

namespace native {
static int32_t distanceToObject(std::shared_ptr<rtda::Class> klass) {
  int32_t distance = 0;
  for (auto k = klass->getSuperClass(); k != nullptr; k = k->getSuperClass()) {
    distance++;
  }
  return distance;
}
static std::shared_ptr<StackTraceElement> createStackTraceElement(std::shared_ptr<rtda::Method> method, 
  std::shared_ptr<rtda::Frame> frame) {
  std::string fileName = method->getClass()->getSourceFile();
  int32_t lineNumber = method->getLineNumber(frame->nextPC() - 1);
  std::string className = method->getClass()->getJavaName();
  std::string methodName = method->getName();
  auto ste = std::make_shared<StackTraceElement>(fileName, className, methodName, lineNumber);
  return ste;
}
static void createStackTraceElements(rtda::Object* obj, std::shared_ptr<rtda::Thread> thread) {
  int32_t skip = distanceToObject(obj->getClass()) + 2;
  auto stes = new std::vector<std::shared_ptr<StackTraceElement>>();
  std::shared_ptr<std::vector<std::shared_ptr<rtda::Frame>>> frames = thread->getFrames();
  for (auto frame : *frames) {
    auto method = frame->getMethod();
    if (skip > 0) {
      skip--;
    } else {
      stes->push_back(createStackTraceElement(method, frame));
    }
  }
  obj->setExtra(stes);
}
void fillInStackTrace(std::shared_ptr<rtda::Frame> frame) {
  auto thisPtr = frame->getLocalVars().getThis();
  frame->getOperandStack().pushRef(thisPtr);
  createStackTraceElements(thisPtr, frame->getThread());
}
} // namespace native