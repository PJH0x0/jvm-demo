#include "java_lang_Class.h"
#include <runtime/class_loader.h>
#include <runtime/constant_pool.h>
#include <runtime/oo/object.h>
#include <runtime/oo/class.h>
#include <runtime/string_pool.h>

namespace native {

void getPrimitiveClass(std::shared_ptr<runtime::Frame> frame) {
  auto nameObj = frame->getLocalVars().getRef(0);
  std::string name = runtime::StringPool::javaStringToString(nameObj);
  auto classLoader = runtime::ClassLoader::getBootClassLoader(nullptr);
  auto classPtr = classLoader->loadClass(name);
  auto classObject = static_cast<runtime::Object*>(classPtr);
  frame->getOperandStack().pushRef(classObject);
}

void getName0(std::shared_ptr<runtime::Frame> frame) {
  // auto thisObj = frame->getLocalVars().getThis();
  // auto classPtr = static_cast<runtime::Class*>(thisObj->getExtra());
  // auto name = classPtr->getJavaName();
  // auto nameObj = runtime::Class::newJString(name);
  // frame->getOperandStack().pushRef(nameObj);
}

void desiredAssertionStatus0(std::shared_ptr<runtime::Frame> frame) {
  frame->getOperandStack().pushInt(0);
}
} // namespace runtime