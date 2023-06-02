#include "sun_misc_VM.h"

#include <memory>
#include <rtda/frame.h>
#include <rtda/heap/class_loader.h>
#include <rtda/heap/object.h>
#include <rtda/heap/constant_pool.h>
#include <rtda/heap/field.h>
#include <rtda/heap/class.h>
#include <instructions/invoke_instructions.h>
#include <rtda/heap/method.h>
namespace native {
void initialize(std::shared_ptr<rtda::Frame> frame) {
  // TODO
  auto vmClass = frame->getMethod()->getClass()->getClassLoader()->loadClass("sun/misc/VM");
  //auto savedProps = vmClass->getField("savedProps", "Ljava/util/Properties;", false);
  auto propClass = vmClass->getClassLoader()->loadClass("java/util/Properties");
  auto savedPropsObj = new rtda::Object(propClass);
  auto key = rtda::Class::newJString("foo");
  auto val = rtda::Class::newJString("bar");
  frame->getOperandStack().pushRef(savedPropsObj);
  frame->getOperandStack().pushRef(key);
  frame->getOperandStack().pushRef(val);
  auto setPropMethod = propClass->getMethod("setProperty", 
                "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/Object;", false);
  instructions::invokeMethod(frame, setPropMethod);
}
} // namespace native