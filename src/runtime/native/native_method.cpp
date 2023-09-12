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
    RegisterNativeMethod("java/lang/Object", "GetClass", "()Ljava/lang/Class;", GetClass);
    RegisterNativeMethod("java/lang/Object", "HashCode", "()I", HashCode);
    RegisterNativeMethod("java/lang/Object", "Clone", "()Ljava/lang/Object;", Clone);
    RegisterNativeMethod("java/lang/Class", "GetPrimitiveClass", "(Ljava/lang/String;)Ljava/lang/Class;",
                         GetPrimitiveClass);
    RegisterNativeMethod("java/lang/Class", "GetName0", "()Ljava/lang/String;", GetName0);
    RegisterNativeMethod("java/lang/Class", "DesiredAssertionStatus0", "(Ljava/lang/Class;)Z",
                         DesiredAssertionStatus0);
    RegisterNativeMethod("java/lang/System", "ArrayCopy",
                         "(Ljava/lang/Object;ILjava/lang/Object;II)V", ArrayCopy);
    RegisterNativeMethod("java/lang/Double", "DoubleToRawLongBits", "(D)J", DoubleToRawLongBits);
    RegisterNativeMethod("java/lang/Double", "LongBitsToDouble", "(J)D", LongBitsToDouble);
    RegisterNativeMethod("java/lang/Float", "FloatToRawIntBits", "(F)I", FloatToRawIntBits);
    RegisterNativeMethod("java/lang/String", "Intern", "()Ljava/lang/String;", Intern);
    RegisterNativeMethod("sun/misc/VM", "initialize", "()V", Initialize);
    RegisterNativeMethod("java/lang/Throwable", "FillInStackTrace", "(I)Ljava/lang/Throwable;", FillInStackTrace);
}

void RegisterNativeMethod(std::string className, std::string methodName, std::string methodDescriptor, NativeMethod method) {
  std::string key = className + "~" + methodName + "~" + methodDescriptor;
  //LOG(INFO) << "register native method: " << key;
  registry[key] = method;
}
NativeMethod FindNativeMethod(std::string className, std::string methodName, std::string methodDescriptor) {
  std::string key = className + "~" + methodName + "~" + methodDescriptor;
  //LOG(INFO) << "find native method: " << key;
  if (registry.find(key) != registry.end()) {
    return registry[key];
  }
  if (methodDescriptor == "()V" && methodName == "registerNatives") {
    return EmptyNativeMethod;
  }
  return nullptr;
}
void EmptyNativeMethod(std::shared_ptr<runtime::Frame> frame) {
  //do nothing
}

}// namespace native