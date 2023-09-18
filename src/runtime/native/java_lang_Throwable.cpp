#include "java_lang_Throwable.h"

#include <memory>
#include <runtime/frame.h>
#include <runtime/oo/object.h>
#include <runtime/oo/method.h>
#include <cstdint>

namespace native {
static int32_t DistanceToObject(runtime::Class* klass) {
  int32_t distance = 0;
  for (auto k = klass->GetSuperClass(); k != nullptr; k = k->GetSuperClass()) {
    distance++;
  }
  return distance;
}
static std::shared_ptr<StackTraceElement> CreateStackTraceElement(std::shared_ptr<runtime::Method> method,
                                                                  runtime::Frame* frame) {
  std::string file_name = method->GetClass()->GetSourceFile();
  int32_t line_number = method->GetLineNumber(frame->NextPc() - 1);
  std::string class_name = method->GetClass()->GetJavaName();
  std::string method_name = method->GetName();
  auto ste = std::make_shared<StackTraceElement>(file_name, class_name, method_name, line_number);
  return ste;
}
static void CreateStackTraceElements(runtime::Object* obj, const runtime::Thread* thread) {
  int32_t skip = DistanceToObject(obj->GetClass()) + 2;
  auto stes = new std::vector<std::shared_ptr<StackTraceElement>>();
  auto frames = thread->GetFrames();
//  for (auto frame : *frames) {
//    auto method = frame->GetMethod();
//    if (skip > 0) {
//      skip--;
//    } else {
//      stes->push_back(CreateStackTraceElement(method, frame));
//    }
//  }
  //obj->setExtra(stes);
}
void FillInStackTrace(runtime::Frame* frame) {
  auto this_obj = frame->GetLocalVars().GetThis();
  frame->GetOperandStack().PushRef(this_obj);
    CreateStackTraceElements(this_obj, runtime::Thread::Current());
}
} // namespace native