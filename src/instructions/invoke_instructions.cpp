#include "invoke_instructions.h"

#include <array>
#include <ios>
#include <rtda/heap/method.h>
#include <rtda/heap/class.h>
#include <rtda/heap/class_member.h>
#include <rtda/heap/constant_pool.h>
#include <rtda/heap/object.h>
#include "base/bytecode_reader.h"
#include <native/native_method.h>

namespace instructions {
void invokeMethod(std::shared_ptr<rtda::Frame> frame, std::shared_ptr<rtda::Method> method) {
  std::shared_ptr<rtda::Thread> thread = frame->getThread();
  std::shared_ptr<rtda::Frame> newFrame = std::make_shared<rtda::Frame>(thread, method->getMaxLocals(), method->getMaxStack(), method);
  thread->pushFrame(newFrame);
  rtda::LocalVars& vars = newFrame->getLocalVars();
  LOG_IF(INFO, INST_DEBUG) << "method argSlotCount = " << method->getArgSlotCount();
  for (int32_t i = method->getArgSlotCount() - 1; i >= 0; i--) {
    vars.setSlot(i, frame->getOperandStack().popSlot());
  }
  LOG_IF(INFO, INST_DEBUG) << "method name = " << method->getName();
  //LOG_IF(INFO, INST_DEBUG) << "method descriptor = " << method->descriptor;
  LOG_IF(INFO, INST_DEBUG) << "method maxLocals = " << method->getMaxLocals();
  LOG_IF(INFO, INST_DEBUG) << "method maxStack = " << method->getMaxStack();
  //LOG_IF(INFO, INST_DEBUG) << "method accessFlags = " << method->accessFlags;
  //LOG_IF(INFO, INST_DEBUG) << "method code length = " << method->codes.size();
  // if (method->isNative()) {
  //   if (method->getName() == "registerNatives") {
  //     native::init();
  //   }
  // }
}
void INVOKE_STATIC::execute(std::shared_ptr<rtda::Frame> frame) {
  
  std::shared_ptr<rtda::Method> methodPtr = frame->getMethod();
  std::shared_ptr<rtda::ConstantPool> cp = methodPtr->getClass()->getConstantPool();
  std::shared_ptr<rtda::Constant> constant = cp->getConstant(index);
  std::shared_ptr<rtda::MethodRefConstant> methodRefInfo = std::dynamic_pointer_cast<rtda::MethodRefConstant>(constant);
  
  std::shared_ptr<rtda::Method> resolvedMethod = methodRefInfo->resolveMethod();
  if (!resolvedMethod->isStatic()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }

  LOG_IF(INFO, INST_DEBUG) << "INVOKE_STATIC " << resolvedMethod->getName() 
                           << " " << resolvedMethod->getDescriptor() 
                           << " " << resolvedMethod->getClass()->getName();
  
  //Check class initialization
  std::shared_ptr<rtda::Class> resolvedClass = resolvedMethod->getClass();
  if (!resolvedClass->isClinitStarted()) {
    frame->revertNextPC();
    rtda::Class::initClass(frame->getThread(), resolvedClass);
    return;
  }
  if (resolvedMethod->getName() == "println") {
    //LOG_IF(INFO, INST_DEBUG) << "hack println";
    hackPrintln(resolvedMethod, frame);
    return;
  }
  
  invokeMethod(frame, resolvedMethod);
}
void INVOKE_SPECIAL::execute(std::shared_ptr<rtda::Frame> frame) {
  std::shared_ptr<rtda::Method> methodPtr = frame->getMethod();
  std::shared_ptr<rtda::ConstantPool> cp = methodPtr->getClass()->getConstantPool();
  std::shared_ptr<rtda::Constant> constant = cp->getConstant(index);
  std::shared_ptr<rtda::MethodRefConstant> methodRefInfo = std::dynamic_pointer_cast<rtda::MethodRefConstant>(constant);
  std::shared_ptr<rtda::Method> resolvedMethod = methodRefInfo->resolveMethod();
  std::shared_ptr<rtda::Class> resolvedClass = methodRefInfo->resolveClass();
  if (resolvedMethod->getName() == "<init>" && resolvedMethod->getClass() != resolvedClass) {
    LOG(FATAL) << "java.lang.NoSuchMethodError";
  }
  if (resolvedMethod->isStatic()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  void* ref = frame->getOperandStack().getRefFromTop(resolvedMethod->getArgSlotCount() -1);
  if (ref == nullptr) {
    LOG(FATAL) << "java.lang.NullPointerException";
  }
  rtda::Object* refObj = static_cast<rtda::Object*>(ref);
  if (resolvedMethod->isProtected() 
      && rtda::Class::isSuperClassOf(resolvedMethod->getClass(), methodPtr->getClass()) 
      && resolvedMethod->getClass()->getPackageName() != methodPtr->getClass()->getPackageName() 
      && refObj->getClass() != methodPtr->getClass() 
      && !rtda::Class::isSubClassOf(refObj->getClass(), methodPtr->getClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  std::shared_ptr<rtda::Method> methodToBeInvoked = resolvedMethod;
  if (methodPtr->getClass()->isSuper() 
      && rtda::Class::isSuperClassOf(resolvedClass, methodPtr->getClass()) 
      && resolvedMethod->getName() != "<init>") {
    std::shared_ptr<rtda::Class> superClassPtr = methodPtr->getClass()->getSuperClass();
    methodToBeInvoked = superClassPtr->lookupMethodInClass(methodRefInfo->name(), methodRefInfo->descriptor());
  }
  if (methodToBeInvoked == nullptr || methodToBeInvoked->isAbstract()) {
    LOG(FATAL) << "java.lang.AbstractMethodError";
  }
  LOG_IF(INFO, INST_DEBUG) << "INVOKE_SPECIAL " << methodToBeInvoked->getName() 
                           << " " << methodToBeInvoked->getDescriptor() 
                           << " " << methodToBeInvoked->getClass()->getName();
  invokeMethod(frame, methodToBeInvoked);
}

void INVOKE_VIRTUAL::execute(std::shared_ptr<rtda::Frame> frame) {
  std::shared_ptr<rtda::Method> methodPtr = frame->getMethod();
  std::shared_ptr<rtda::ConstantPool> cp = methodPtr->getClass()->getConstantPool();
  std::shared_ptr<rtda::Constant> constant = cp->getConstant(index);
  std::shared_ptr<rtda::MethodRefConstant> methodRefInfo = std::dynamic_pointer_cast<rtda::MethodRefConstant>(constant);
  std::shared_ptr<rtda::Method> resolvedMethod = methodRefInfo->resolveMethod();
  if (resolvedMethod->isStatic()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  if (resolvedMethod->getName() == "println") {
    //LOG_IF(INFO, INST_DEBUG) << "hack println";
    //LOG(WARNING) << "hack println "<< frame->getOperandStack().popInt();
    hackPrintln(resolvedMethod, frame);
    return;
  }
  void* ref = frame->getOperandStack().getRefFromTop(resolvedMethod->getArgSlotCount() -1);
  if (ref == nullptr) {
    
    LOG(FATAL) << "java.lang.NullPointerException";
  }
  rtda::Object* refObj = static_cast<rtda::Object*>(ref);
  if (resolvedMethod->isProtected() 
      && rtda::Class::isSuperClassOf(resolvedMethod->getClass(), methodPtr->getClass()) 
      && resolvedMethod->getClass()->getPackageName() != methodPtr->getClass()->getPackageName() 
      && refObj->getClass() != methodPtr->getClass() 
      && !rtda::Class::isSubClassOf(refObj->getClass(), methodPtr->getClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  std::shared_ptr<rtda::Method> methodToBeInvoked = refObj->getClass()->lookupMethodInClass(
    methodRefInfo->name(), methodRefInfo->descriptor());
  if (methodToBeInvoked == nullptr || methodToBeInvoked->isAbstract()) {
    LOG(FATAL) << "java.lang.AbstractMethodError";
  }
  LOG_IF(INFO, INST_DEBUG) << "INVOKE_VIRTUAL " << methodToBeInvoked->getName() 
                           << " " << methodToBeInvoked->getDescriptor() 
                           << " " << methodToBeInvoked->getClass()->getName();
  invokeMethod(frame, methodToBeInvoked);
}

void INVOKE_INTERFACE::fetchOperands(std::shared_ptr<BytecodeReader> reader) {
  index = uint32_t(reader->readUInt16());
  reader->readUInt8();
  reader->readUInt8();
}

void INVOKE_INTERFACE::execute(std::shared_ptr<rtda::Frame> frame) {
  std::shared_ptr<rtda::Method> methodPtr = frame->getMethod();
  std::shared_ptr<rtda::ConstantPool> cp = methodPtr->getClass()->getConstantPool();
  std::shared_ptr<rtda::Constant> constant = cp->getConstant(index);
  std::shared_ptr<rtda::InterfaceMethodRefConstant> methodRefInfo = std::dynamic_pointer_cast<rtda::InterfaceMethodRefConstant>(constant);
  std::shared_ptr<rtda::Method> resolvedMethod = methodRefInfo->resolveInterfaceMethod();
  if (resolvedMethod->isStatic() || resolvedMethod->isPrivate()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  void* ref = frame->getOperandStack().getRefFromTop(resolvedMethod->getArgSlotCount() -1);
  if (ref == nullptr) {
    LOG(FATAL) << "java.lang.NullPointerException";
  }
  rtda::Object* refObj = static_cast<rtda::Object*>(ref);
  if (!rtda::Class::isImplements(refObj->getClass(), methodRefInfo->resolveClass())) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  std::shared_ptr<rtda::Method> methodToBeInvoked = refObj->getClass()->lookupMethodInClass(
    methodRefInfo->name(), methodRefInfo->descriptor());
  if (methodToBeInvoked == nullptr || methodToBeInvoked->isAbstract()) {
    LOG(FATAL) << "java.lang.AbstractMethodError";
  }
  if (!methodToBeInvoked->isPublic()) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  LOG_IF(INFO, INST_DEBUG) << "INVOKE_INTERFACE " << methodToBeInvoked->getName() 
                           << " " << methodToBeInvoked->getDescriptor() 
                           << " " << methodToBeInvoked->getClass()->getName();
  invokeMethod(frame, methodToBeInvoked);
}

void INVOKE_NATIVE::execute(std::shared_ptr<rtda::Frame> frame) {
  std::shared_ptr<rtda::Method> methodPtr = frame->getMethod();
  std::string className = methodPtr->getClass()->getName();
  std::string methodName = methodPtr->getName();
  std::string methodDescriptor = methodPtr->getDescriptor();
  auto nativeMethod = native::findNativeMethod(className, methodName, methodDescriptor);
  if (nativeMethod == nullptr) {
    std::string methodInfo = className + "." + methodName + methodDescriptor;
    LOG(FATAL) << "java.lang.UnsatisfiedLinkError: " << methodInfo;
  }
  LOG_IF(INFO, INST_DEBUG) << "INVOKE_NATIVE " << methodPtr->getName() 
                           << " " << methodPtr->getDescriptor() 
                           << " " << methodPtr->getClass()->getName();
  nativeMethod(frame);
}

void hackPrintln(std::shared_ptr<rtda::Method> resolvedMethod, std::shared_ptr<rtda::Frame> frame) {
  auto& opStack = frame->getOperandStack();
  auto descriptor = resolvedMethod->getDescriptor();
  if (descriptor == "(Ljava/lang/String;)V") {
    auto jStr = opStack.popRef();
    auto charArr = jStr->getRefVar("value", "[C");
    const char16_t* u16Chars = charArr->getArray<char16_t>();
    
    LOG(WARNING) << "hack println "<< rtda::StringConstant::utf16ToUtf8(u16Chars);
    return;
  }
  switch (descriptor[1]) {
    case 'Z':
    case 'B':
    case 'C':
    case 'S':
    case 'I': {
      LOG(WARNING) << "hack println "<< opStack.popInt();
      break;
    }
    case 'F': {
      LOG(WARNING) << "hack println "<< opStack.popFloat();
      break;
    }
    case 'J': {
      LOG(WARNING) << "hack println "<< opStack.popLong();
      break;
    }
    case 'D': {
      LOG(WARNING) << "hack println "<< opStack.popDouble();
      break;
    }
    case 'L':
    case '[': {
      LOG(WARNING) << "hack println "<< opStack.popRef();
      break;
    }
    default: {
      LOG(FATAL) << "hack println "<< descriptor;
    }
    
  }
}

} // namespace instructions