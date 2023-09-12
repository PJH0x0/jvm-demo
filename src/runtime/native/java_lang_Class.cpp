#include "java_lang_Class.h"
#include <runtime/class_loader.h>
#include <runtime/constant_pool.h>
#include <runtime/oo/object.h>
#include <runtime/oo/class.h>
#include <runtime/string_pool.h>

namespace native {

void GetPrimitiveClass(std::shared_ptr<runtime::Frame> frame) {
  auto nameObj = frame->getLocalVars().getRef(0);
  std::string name = runtime::StringPool::javaStringToString(nameObj);
  auto classLoader = runtime::ClassLoader::getBootClassLoader(nullptr);
  auto classPtr = classLoader->loadClass(name);
  auto classObject = static_cast<runtime::Object*>(classPtr);
  frame->getOperandStack().pushRef(classObject);
}

void GetName0(std::shared_ptr<runtime::Frame> frame) {
  // auto thisObj = frame->getLocalVars().getThis();
  // auto classPtr = static_cast<runtime::Class*>(thisObj->getExtra());
  // auto name = classPtr->GetJavaName();
  // auto nameObj = runtime::Class::NewJString(name);
  // frame->getOperandStack().pushRef(nameObj);
}

void DesiredAssertionStatus0(std::shared_ptr<runtime::Frame> frame) {
  frame->getOperandStack().pushInt(0);
}
} // namespace runtime