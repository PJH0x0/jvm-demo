#include "java_lang_Object.h"
#include <runtime/oo/object.h>
#include <runtime/oo/class.h>
#include <runtime/class_loader.h>
namespace native {
void GetClass(runtime::Frame* frame) {
  auto this_obj = frame->GetLocalVars().GetThis();
  auto class_ptr = this_obj->GetClass();
  auto class_ref = static_cast<runtime::Object*>(class_ptr);
  frame->GetOperandStack().PushRef(class_ref);
}
void HashCode(runtime::Frame* frame) {
  auto this_obj = frame->GetLocalVars().GetThis();
  int32_t hash = (int32_t)(intptr_t)this_obj;
  frame->GetOperandStack().PushInt(hash);
}
void Clone(runtime::Frame* frame) {
  auto this_obj = frame->GetLocalVars().GetThis();
  auto cloneable = this_obj->GetClass()->GetClassLoader()->LoadClass("java/lang/Cloneable");
  if (!runtime::Class::IsImplements(this_obj->GetClass(), cloneable)) {
    LOG(ERROR) << "java.lang.CloneNotSupportedException";
    return;
  }
  runtime::Object* clone_obj = this_obj->Clone();
  frame->GetOperandStack().PushRef(clone_obj);
}
}// namespace native