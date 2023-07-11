#include "java_lang_Throwable.h"

#include <memory>
#include <runtime/frame.h>
#include <runtime/oo/object.h>
#include <runtime/oo/method.h>
#include <cstdint>

namespace native {
static int32_t distanceToObject(runtime::Class* klass) {
  int32_t distance = 0;
  for (auto k = klass->getSuperClass(); k != nullptr; k = k->getSuperClass()) {
    distance++;
  }
  return distance;
}
static std::shared_ptr<StackTraceElement> createStackTraceElement(std::shared_ptr<runtime::Method> method, 
  std::shared_ptr<runtime::Frame> frame) {
  std::string fileName = method->getClass()->getSourceFile();
  int32_t lineNumber = method->getLineNumber(frame->nextPC() - 1);
  std::string className = method->getClass()->getJavaName();
  std::string methodName = method->getName();
  auto ste = std::make_shared<StackTraceElement>(fileName, className, methodName, lineNumber);
  return ste;
}
static void createStackTraceElements(runtime::Object* obj, std::shared_ptr<runtime::Thread> thread) {
  int32_t skip = distanceToObject(obj->getClass()) + 2;
  auto stes = new std::vector<std::shared_ptr<StackTraceElement>>();
  std::shared_ptr<std::vector<std::shared_ptr<runtime::Frame>>> frames = thread->getFrames();
  for (auto frame : *frames) {
    auto method = frame->getMethod();
    if (skip > 0) {
      skip--;
    } else {
      stes->push_back(createStackTraceElement(method, frame));
    }
  }
  //obj->setExtra(stes);
}
void fillInStackTrace(std::shared_ptr<runtime::Frame> frame) {
  auto thisPtr = frame->getLocalVars().getThis();
  frame->getOperandStack().pushRef(thisPtr);
  createStackTraceElements(thisPtr, frame->getThread());
}
} // namespace native