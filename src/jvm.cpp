#include "jvm.h"
#include <runtime/native/native_method.h>
#include "classpath/class_path_parser.h"
#include "interpreter.h"
#include "runtime/frame.h"
#include "runtime/thread.h"
#include "runtime/oo/class.h"
#include <runtime/alloc/heap.h>
#include <runtime/oo/object.h>
#include <runtime/oo/method.h>
#include <runtime/class_loader.h>
#include "command.h"

JVM* JVM::jvm_current_ = nullptr;

JVM::JVM(std::shared_ptr<Command> cmd) {
  this->command_ = cmd;
  std::shared_ptr<classpath::ClassPathParser> parser = std::make_shared<classpath::ClassPathParser>(cmd->GetJrePath(),
    cmd->GetUserClassPath());
  this->class_loader_ = runtime::ClassLoader::GetBootClassLoader(parser);
  class_loader_->LoadBasicClass();
  class_loader_->LoadPrimitiveClasses();
  native::init();
  this->main_thread_ = std::make_shared<runtime::Thread>();
}
void JVM::Start() {
  jvm_current_ = this;
  heap_ = new heap::Heap(0);
  //InitVm();
  std::string class_name = command_->GetClassName();
  runtime::Class* main_class = class_loader_->LoadClass(class_name);
  if (main_class == nullptr) {
    LOG(ERROR) << "main class not found";
    return;
  }
  std::shared_ptr<runtime::Method> main_method = main_class->GetMainMethod();
  if (main_method == nullptr) {
    LOG(ERROR) << "main method not found";
    return;
  }
  std::shared_ptr<runtime::Frame> frame = std::make_shared<runtime::Frame>(main_thread_,
                                                                           main_method->GetMaxLocals(),
                                                                           main_method->GetMaxStack(),
                                                                           main_method);
  main_thread_->PushFrame(frame);
  frame->GetLocalVars().SetRef(0, CreateArgsArray());
  Interpret(main_thread_);
}
runtime::Object* JVM::CreateArgsArray() {
  auto string_class = class_loader_->LoadClass("java/lang/String");
  size_t size = command_->GetArgs().size();
  auto args_arr = string_class->GetArrayClass()->NewArray(size);
  for (size_t i = 0; i < size; i++) {
    //args_arr->setArrayElement<runtime::Object*>(i, runtime::Class::NewJString(cmd->args[i]));
  }
  return args_arr;
}
void JVM::InitVm() {
  std::string class_name = "sun/misc/VM";
  runtime::Class* vm_class_ptr = class_loader_->LoadClass(class_name);
  std::shared_ptr<runtime::Method> initialize_method = vm_class_ptr->GetStaticMethod("initialize", "()V");
  std::shared_ptr<runtime::Frame> frame = std::make_shared<runtime::Frame>(main_thread_,
                                                                           initialize_method->GetMaxLocals(),
                                                                           initialize_method->GetMaxStack(),
                                                                           initialize_method);
  main_thread_->PushFrame(frame);
  Interpret(main_thread_);
}