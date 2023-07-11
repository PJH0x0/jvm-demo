#include "jvm.h"
#include <runtime/native/native_method.h>
#include "classpath/class_path_parser.h"
#include "interpreter.h"
#include "runtime/frame.h"
#include "runtime/thread.h"
#include "runtime/oo/class.h"

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
  //initVM();
  std::string clsName = cmd->className;
  std::shared_ptr<runtime::Class> mainClsPtr = classLoader->loadClass(clsName);
  if (mainClsPtr == nullptr) {
    LOG(ERROR) << "main class not found";
    return;
  }
  std::shared_ptr<runtime::Method> mainMethod = mainClsPtr->getMainMethod();
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
  auto argsArr = stringCls->getArrayClass()->newArray(size);
  for (size_t i = 0; i < size; i++) {
    argsArr->setArrayElement<runtime::Object*>(i, runtime::Class::newJString(cmd->args[i]));
  }
  return argsArr;
}
void JVM::initVM() {
  std::string clsName = "sun/misc/VM";
  std::shared_ptr<runtime::Class> vmClsPtr = classLoader->loadClass(clsName);
  std::shared_ptr<runtime::Method> initializeMethod = vmClsPtr->getStaticMethod("initialize", "()V");
  std::shared_ptr<runtime::Frame> frame = std::make_shared<runtime::Frame>(mainThread, 
                                                                     initializeMethod->getMaxLocals(), 
                                                                     initializeMethod->getMaxStack(),
                                                                     initializeMethod);
  mainThread->pushFrame(frame);
  interpret(mainThread);
}