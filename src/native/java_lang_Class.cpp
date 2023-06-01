#include "java_lang_Class.h"
#include <rtda/heap/class_loader.h>
#include <rtda/heap/constant_pool.h>
#include <rtda/heap/object.h>
#include <rtda/heap/class.h>

namespace native {

void getPrimitiveClass(std::shared_ptr<rtda::Frame> frame) {
  auto nameObj = frame->getLocalVars().getRef(0);
  auto charArr = nameObj->getRefVar("value", "[C");
  const char16_t* u16Chars = charArr->getArray<char16_t>();
  std::string name = rtda::StringConstant::utf16ToUtf8(u16Chars);
  auto classLoader = rtda::ClassLoader::getBootClassLoader(nullptr);
  auto classPtr = classLoader->loadClass(name);
  auto classObject = classPtr->getJClass();
  frame->getOperandStack().pushRef(classObject);
}

void getName0(std::shared_ptr<rtda::Frame> frame) {
  auto thisObj = frame->getLocalVars().getThis();
  auto classPtr = static_cast<rtda::Class*>(thisObj->getExtra());
  auto name = classPtr->getJavaName();
  auto nameObj = rtda::Class::newJString(name);
  frame->getOperandStack().pushRef(nameObj);
}

void desiredAssertionStatus0(std::shared_ptr<rtda::Frame> frame) {
  frame->getOperandStack().pushInt(0);
}
} // namespace rtda