#include "sun_misc_VM.h"

#include <memory>
#include <runtime/frame.h>
#include <runtime/class_loader.h>
#include <runtime/oo/object.h>
#include <runtime/constant_pool.h>
#include <runtime/oo/field.h>
#include <runtime/oo/class.h>
#include <instructions/invoke_insts.h>
#include <runtime/oo/method.h>
namespace native {
void Initialize(runtime::Frame* frame) {
  // TODO
  auto vm_class = frame->GetMethod()->GetClass()->GetClassLoader()->LoadClass("sun/misc/VM");
  auto saved_props_field = vm_class->GetField("savedProps", "Ljava/util/Properties;", true);
  auto prop_class = vm_class->GetClassLoader()->LoadClass("java/util/Properties");
  auto saved_props_obj = vm_class->GetStaticVars()->GetRef(saved_props_field->GetSlotId());
  auto key = runtime::Class::NewJString("foo");
  auto val = runtime::Class::NewJString("bar");
  frame->GetOperandStack().PushRef(saved_props_obj);
  frame->GetOperandStack().PushRef(key);
  frame->GetOperandStack().PushRef(val);
  auto set_prop_method = prop_class->GetMethod("setProperty",
                                               "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/Object;", false);
  instructions::InvokeMethod(frame, set_prop_method);
}
} // namespace native