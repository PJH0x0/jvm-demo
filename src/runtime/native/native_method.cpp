#include "native_method.h"
#include "java_lang_Class.h"
#include "java_lang_Object.h"
#include "java_lang_System.h"
#include "java_lang_Double.h"
#include "java_lang_Float.h"
#include "java_lang_String.h"
#include "java_lang_Throwable.h"
#include "sun_misc_VM.h"
#include <unordered_map>
#include <glog/logging.h>
#include <runtime/class_loader.h>
#include <runtime/constant_pool.h>
#include <runtime/oo/object.h>
#include <runtime/oo/class.h>

namespace native {
static std::unordered_map<std::string, NativeMethod> registry;

void init() {
  registerNativeMethod("java/lang/Object", "getClass", "()Ljava/lang/Class;", getClass);
  registerNativeMethod("java/lang/Object", "hashCode", "()I", hashCode);
  registerNativeMethod("java/lang/Object", "clone", "()Ljava/lang/Object;", clone);
  registerNativeMethod("java/lang/Class", "getPrimitiveClass", "(Ljava/lang/String;)Ljava/lang/Class;", 
                        getPrimitiveClass);
  registerNativeMethod("java/lang/Class", "getName0", "()Ljava/lang/String;", getName0);
  registerNativeMethod("java/lang/Class", "desiredAssertionStatus0", "(Ljava/lang/Class;)Z", 
                        desiredAssertionStatus0);
  registerNativeMethod("java/lang/System", "arraycopy", 
      "(Ljava/lang/Object;ILjava/lang/Object;II)V", arraycopy);
  registerNativeMethod("java/lang/Double", "doubleToRawLongBits", "(D)J", doubleToRawLongBits);
  registerNativeMethod("java/lang/Double", "longBitsToDouble", "(J)D", longBitsToDouble);
  registerNativeMethod("java/lang/Float", "floatToRawIntBits", "(F)I", floatToRawIntBits);
  registerNativeMethod("java/lang/String", "intern", "()Ljava/lang/String;", intern);
  registerNativeMethod("sun/misc/VM", "initialize", "()V", initialize);
  registerNativeMethod("java/lang/Throwable", "fillInStackTrace", "(I)Ljava/lang/Throwable;", fillInStackTrace);
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
void emptyNativeMethod(std::shared_ptr<runtime::Frame> frame) {
  //do nothing
}

}// namespace native