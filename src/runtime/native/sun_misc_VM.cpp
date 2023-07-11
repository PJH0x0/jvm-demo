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
void initialize(std::shared_ptr<runtime::Frame> frame) {
  // TODO
  auto vmClass = frame->getMethod()->getClass()->getClassLoader()->loadClass("sun/misc/VM");
  auto savedPropsField = vmClass->getField("savedProps", "Ljava/util/Properties;", true);
  auto propClass = vmClass->getClassLoader()->loadClass("java/util/Properties");
  auto savedPropsObj = vmClass->getStaticVars()->getRef(savedPropsField->getSlotId());
  auto key = runtime::Class::newJString("foo");
  auto val = runtime::Class::newJString("bar");
  frame->getOperandStack().pushRef(savedPropsObj);
  frame->getOperandStack().pushRef(key);
  frame->getOperandStack().pushRef(val);
  auto setPropMethod = propClass->getMethod("setProperty", 
                "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/Object;", false);
  instructions::invokeMethod(frame, setPropMethod);
}
} // namespace native