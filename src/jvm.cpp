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

JVM* JVM::jvmCurrent = nullptr;

JVM::JVM(std::shared_ptr<Command> cmd) {
  this->cmd = cmd;
  std::shared_ptr<classpath::ClassPathParser> parser = std::make_shared<classpath::ClassPathParser>(cmd->jrePath, 
    cmd->userClassPath);
  this->classLoader = runtime::ClassLoader::getBootClassLoader(parser);
  classLoader->loadBasicClass();
  classLoader->loadPrimitiveClasses();
  native::init();
  this->mainThread = std::make_shared<runtime::Thread>();
}
void JVM::start() {
  jvmCurrent = this;
  gHeap = new heap::Heap(0);
  //initVM();
  std::string clsName = cmd->className;
  runtime::Class* mainClsPtr = classLoader->loadClass(clsName);
  if (mainClsPtr == nullptr) {
    LOG(ERROR) << "main class not found";
    return;
  }
  std::shared_ptr<runtime::Method> mainMethod = mainClsPtr->GetMainMethod();
  if (mainMethod == nullptr) {
    LOG(ERROR) << "main method not found";
    return;
  }
  std::shared_ptr<runtime::Frame> frame = std::make_shared<runtime::Frame>(mainThread, 
                                                      mainMethod->getMaxLocals(), 
                                                      mainMethod->getMaxStack(),
                                                      mainMethod);
  mainThread->pushFrame(frame);
  frame->getLocalVars().setRef(0, createArgsArray());
  interpret(mainThread);
}
runtime::Object* JVM::createArgsArray() {
  auto stringCls = classLoader->loadClass("java/lang/String");
  size_t size = cmd->args.size();
  auto argsArr = stringCls->GetArrayClass()->NewArray(size);
  for (size_t i = 0; i < size; i++) {
    //argsArr->setArrayElement<runtime::Object*>(i, runtime::Class::NewJString(cmd->args[i]));
  }
  return argsArr;
}
void JVM::initVM() {
  std::string clsName = "sun/misc/VM";
  runtime::Class* vmClsPtr = classLoader->loadClass(clsName);
  std::shared_ptr<runtime::Method> initializeMethod = vmClsPtr->GetStaticMethod("initialize", "()V");
  std::shared_ptr<runtime::Frame> frame = std::make_shared<runtime::Frame>(mainThread, 
                                                                     initializeMethod->getMaxLocals(), 
                                                                     initializeMethod->getMaxStack(),
                                                                     initializeMethod);
  mainThread->pushFrame(frame);
  interpret(mainThread);
}