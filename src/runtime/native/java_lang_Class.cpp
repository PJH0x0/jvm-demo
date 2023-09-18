#include "java_lang_Class.h"
#include <runtime/class_loader.h>
#include <runtime/constant_pool.h>
#include <runtime/oo/object.h>
#include <runtime/oo/class.h>
#include <runtime/string_pool.h>

namespace native {

void GetPrimitiveClass(runtime::Frame* frame) {
  auto name_obj = frame->GetLocalVars().GetRef(0);
  std::string name = runtime::StringPool::javaStringToString(name_obj);
  auto boot_class_loader = runtime::ClassLoader::GetBootClassLoader(nullptr);
  auto class_ptr = boot_class_loader->LoadClass(name);
  auto class_ref = static_cast<runtime::Object*>(class_ptr);
  frame->GetOperandStack().PushRef(class_ref);
}

void GetName0(runtime::Frame* frame) {
  // auto thisObj = frame->GetLocalVars().GetThis();
  // auto classPtr = static_cast<runtime::Class*>(thisObj->getExtra());
  // auto name = classPtr->GetJavaName();
  // auto nameObj = runtime::Class::NewJString(name);
  // frame->GetOperandStack().PushRef(nameObj);
}

void DesiredAssertionStatus0(runtime::Frame* frame) {
  frame->GetOperandStack().PushInt(0);
}
} // namespace runtime