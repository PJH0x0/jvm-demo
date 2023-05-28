#include "native_method.h"
#include <unordered_map>

namespace native {
static std::unordered_map<std::string, NativeMethod> registry;
void registerNativeMethod(std::string className, std::string methodName, std::string methodDescriptor, NativeMethod method) {
  std::string key = className + "~" + methodName + "~" + methodDescriptor;
  registry[key] = method;
}
NativeMethod findNativeMethod(std::string className, std::string methodName, std::string methodDescriptor) {
  std::string key = className + "~" + methodName + "~" + methodDescriptor;
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