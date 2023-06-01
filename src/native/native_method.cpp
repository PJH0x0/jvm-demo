#include "native_method.h"
#include <unordered_map>
#include <glog/logging.h>
#include <rtda/heap/class_loader.h>
#include <rtda/heap/constant_pool.h>
#include <rtda/heap/object.h>
#include <rtda/heap/class.h>

namespace native {
static std::unordered_map<std::string, NativeMethod> registry;

void getClass(std::shared_ptr<rtda::Frame> frame) {
  auto thisObj = frame->getLocalVars().getThis();
  auto classPtr = thisObj->getClass();
  auto classObject = classPtr->getJClass();
  frame->getOperandStack().pushRef(classObject);
}

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

void init() {
  registerNativeMethod("java/lang/Object", "getClass", "()Ljava/lang/Class;", getClass);
  registerNativeMethod("java/lang/Class", "getPrimitiveClass", "(Ljava/lang/String;)Ljava/lang/Class;", 
                        getPrimitiveClass);
  registerNativeMethod("java/lang/Class", "getName0", "()Ljava/lang/String;", getName0);
  registerNativeMethod("java/lang/Class", "desiredAssertionStatus0", "(Ljava/lang/Class;)Z", 
                        desiredAssertionStatus0);
}

void registerNativeMethod(std::string className, std::string methodName, std::string methodDescriptor, NativeMethod method) {
  std::string key = className + "~" + methodName + "~" + methodDescriptor;
  //LOG(INFO) << "register native method: " << key;
  registry[key] = method;
}
NativeMethod findNativeMethod(std::string className, std::string methodName, std::string methodDescriptor) {
  std::string key = className + "~" + methodName + "~" + methodDescriptor;
  //LOG(INFO) << "find native method: " << key;
  if (registry.find(key) != registry.end()) {
    return registry[key];
  }
  if (methodDescriptor == "()V" && methodName == "registerNatives") {
    return emptyNativeMethod;
  }
  return nullptr;
}
void emptyNativeMethod(std::shared_ptr<rtda::Frame> frame) {
  //do nothing
}

}// namespace native