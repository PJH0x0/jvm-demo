#include "invoke_instructions.h"
#include <rtda/heap/method.h>
#include <rtda/heap/class.h>
#include <rtda/heap/class_member.h>
#include <rtda/heap/constant_pool.h>
#include <rtda/heap/object.h>
#include "base/bytecode_reader.h"

namespace instructions {
void invokeMethod(std::shared_ptr<rtda::Frame> frame, std::shared_ptr<rtda::Method> method) {
  std::shared_ptr<rtda::Thread> thread = frame->getThread();
  std::shared_ptr<rtda::Frame> newFrame = std::make_shared<rtda::Frame>(thread, method->getMaxLocals(), method->getMaxStack(), method);
  thread->pushFrame(newFrame);
  rtda::LocalVars& vars = newFrame->getLocalVars();
  for (int32_t i = method->getArgSlotCount() - 1; i >= 0; i--) {
    vars.setSlot(i, frame->getOperandStack().popSlot());
  }
  LOG_IF(INFO, INST_DEBUG) << "method name = " << method->getName();
  //LOG_IF(INFO, INST_DEBUG) << "method descriptor = " << method->descriptor;
  LOG_IF(INFO, INST_DEBUG) << "method maxLocals = " << method->getMaxLocals();
  LOG_IF(INFO, INST_DEBUG) << "method maxStack = " << method->getMaxStack();
  //LOG_IF(INFO, INST_DEBUG) << "method accessFlags = " << method->accessFlags;
  LOG_IF(INFO, INST_DEBUG) << "method argSlotCount = " << method->getArgSlotCount();
  //LOG_IF(INFO, INST_DEBUG) << "method code length = " << method->codes.size();
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
      && resolvedMethod->getClass()->isSuperClassOf(methodPtr->getClass()) 
      && resolvedMethod->getClass()->getPackageName() != methodPtr->getClass()->getPackageName() 
      && refObj->getClass() != methodPtr->getClass() 
      && !refObj->getClass()->isSubClassOf(methodPtr->getClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  std::shared_ptr<rtda::Method> methodToBeInvoked = resolvedMethod;
  if (methodPtr->getClass()->isSuper() 
      && resolvedClass->isSuperClassOf(methodPtr->getClass()) 
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
    LOG(WARNING) << "hack println "<< frame->getOperandStack().popLong();
    return;
  }
  void* ref = frame->getOperandStack().getRefFromTop(resolvedMethod->getArgSlotCount() -1);
  if (ref == nullptr) {
    
    LOG(FATAL) << "java.lang.NullPointerException";
  }
  rtda::Object* refObj = static_cast<rtda::Object*>(ref);
  if (resolvedMethod->isProtected() 
      && resolvedMethod->getClass()->isSuperClassOf(methodPtr->getClass()) 
      && resolvedMethod->getClass()->getPackageName() != methodPtr->getClass()->getPackageName() 
      && refObj->getClass() != methodPtr->getClass() 
      && !refObj->getClass()->isSubClassOf(methodPtr->getClass())) {
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
  if (!refObj->getClass()->isImplements(methodRefInfo->resolveClass())) {
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

} // namespace instructions