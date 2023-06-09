#include "jvm.h"
#include "native/native_method.h"
#include "classpath/class_path_parser.h"
#include "interpreter.h"
#include "rtda/frame.h"
#include "rtda/thread.h"
#include "rtda/heap/class.h"

JVM::JVM(std::shared_ptr<Command> cmd) {
  this->cmd = cmd;
  std::shared_ptr<classpath::ClassPathParser> parser = std::make_shared<classpath::ClassPathParser>(cmd->jrePath, 
    cmd->userClassPath);
  this->classLoader = rtda::ClassLoader::getBootClassLoader(parser);
  classLoader->loadBasicClass();
  classLoader->loadPrimitiveClasses();
  native::init();
  this->mainThread = std::make_shared<rtda::Thread>();
}
void JVM::start() {
  //initVM();
  std::string clsName = cmd->className;
  std::shared_ptr<rtda::Class> mainClsPtr = classLoader->loadClass(clsName);
  if (mainClsPtr == nullptr) {
    LOG(ERROR) << "main class not found";
    return;
  }
  std::shared_ptr<rtda::Method> mainMethod = mainClsPtr->getMainMethod();
  if (mainMethod == nullptr) {
    LOG(ERROR) << "main method not found";
    return;
  }
  std::shared_ptr<rtda::Frame> frame = std::make_shared<rtda::Frame>(mainThread, 
                                                      mainMethod->getMaxLocals(), 
                                                      mainMethod->getMaxStack(),
                                                      mainMethod);
  mainThread->pushFrame(frame);
  frame->getLocalVars().setRef(0, createArgsArray());
  interpret(mainThread);
}
rtda::Object* JVM::createArgsArray() {
  auto stringCls = classLoader->loadClass("java/lang/String");
  size_t size = cmd->args.size();
  auto argsArr = stringCls->getArrayClass()->newArray(size);
  for (size_t i = 0; i < size; i++) {
    argsArr->setArrayElement<rtda::Object*>(i, rtda::Class::newJString(cmd->args[i]));
  }
  return argsArr;
}
void JVM::initVM() {
  std::string clsName = "sun/misc/VM";
  std::shared_ptr<rtda::Class> vmClsPtr = classLoader->loadClass(clsName);
  std::shared_ptr<rtda::Method> initializeMethod = vmClsPtr->getStaticMethod("initialize", "()V");
  std::shared_ptr<rtda::Frame> frame = std::make_shared<rtda::Frame>(mainThread, 
                                                                     initializeMethod->getMaxLocals(), 
                                                                     initializeMethod->getMaxStack(),
                                                                     initializeMethod);
  mainThread->pushFrame(frame);
  interpret(mainThread);
}