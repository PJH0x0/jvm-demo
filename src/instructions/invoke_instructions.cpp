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
  std::shared_ptr<rtda::Frame> newFrame = std::make_shared<rtda::Frame>(thread, method->maxLocals, method->maxStack, method);
  thread->pushFrame(newFrame);
  rtda::LocalVars& vars = newFrame->getLocalVars();
  for (int32_t i = method->mArgSlotCount - 1; i >= 0; i--) {
    vars.setSlot(i, frame->getOperandStack().popSlot());
  }
  LOG_IF(INFO, INST_DEBUG) << "method name = " << method->mName;
  //LOG_IF(INFO, INST_DEBUG) << "method descriptor = " << method->descriptor;
  LOG_IF(INFO, INST_DEBUG) << "method maxLocals = " << method->maxLocals;
  LOG_IF(INFO, INST_DEBUG) << "method maxStack = " << method->maxStack;
  //LOG_IF(INFO, INST_DEBUG) << "method accessFlags = " << method->accessFlags;
  LOG_IF(INFO, INST_DEBUG) << "method argSlotCount = " << method->mArgSlotCount;
  //LOG_IF(INFO, INST_DEBUG) << "method code length = " << method->codes.size();
}
void INVOKE_STATIC::execute(std::shared_ptr<rtda::Frame> frame) {
  
  std::shared_ptr<rtda::Method> methodPtr = frame->getMethod();
  std::shared_ptr<rtda::ConstantPool> cp = methodPtr->mClassPtr->mConstantPool;
  std::shared_ptr<rtda::Constant> constant = cp->getConstant(index);
  std::shared_ptr<rtda::MethodRefConstant> methodRefInfo = std::dynamic_pointer_cast<rtda::MethodRefConstant>(constant);
  
  std::shared_ptr<rtda::Method> resolvedMethod = methodRefInfo->resolveMethod();
  if (!resolvedMethod->isStatic()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  LOG_IF(INFO, INST_DEBUG) << "INVOKE_STATIC " << resolvedMethod->mName << " " << resolvedMethod->mDescriptor << " " << resolvedMethod->mClassPtr->mName;
  invokeMethod(frame, resolvedMethod);
}
void INVOKE_SPECIAL::execute(std::shared_ptr<rtda::Frame> frame) {
  std::shared_ptr<rtda::Method> methodPtr = frame->getMethod();
  std::shared_ptr<rtda::ConstantPool> cp = methodPtr->mClassPtr->mConstantPool;
  std::shared_ptr<rtda::Constant> constant = cp->getConstant(index);
  std::shared_ptr<rtda::MethodRefConstant> methodRefInfo = std::dynamic_pointer_cast<rtda::MethodRefConstant>(constant);
  std::shared_ptr<rtda::Method> resolvedMethod = methodRefInfo->resolveMethod();
  std::shared_ptr<rtda::Class> resolvedClass = methodRefInfo->resolveClass();
  if (resolvedMethod->mName == "<init>" && resolvedMethod->mClassPtr != resolvedClass) {
    LOG(FATAL) << "java.lang.NoSuchMethodError";
  }
  if (resolvedMethod->isStatic()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  void* ref = frame->getOperandStack().getRefFromTop(resolvedMethod->mArgSlotCount -1);
  if (ref == nullptr) {
    LOG(FATAL) << "java.lang.NullPointerException";
  }
  rtda::Object* refObj = static_cast<rtda::Object*>(ref);
  if (resolvedMethod->isProtected() 
      && resolvedMethod->mClassPtr->isSuperClassOf(methodPtr->mClassPtr) 
      && resolvedMethod->mClassPtr->getPackageName() != methodPtr->mClassPtr->getPackageName() 
      && refObj->mClass != methodPtr->mClassPtr 
      && !refObj->mClass->isSubClassOf(methodPtr->mClassPtr)) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  std::shared_ptr<rtda::Method> methodToBeInvoked = resolvedMethod;
  if (methodPtr->mClassPtr->isSuper() 
      && resolvedClass->isSuperClassOf(methodPtr->mClassPtr) 
      && resolvedMethod->mName != "<init>") {
    std::shared_ptr<rtda::Class> superClassPtr = methodPtr->mClassPtr->mSuperClass;
    methodToBeInvoked = superClassPtr->lookupMethodInClass(methodRefInfo->mName, methodRefInfo->mDescriptor);
  }
  if (methodToBeInvoked == nullptr || methodToBeInvoked->isAbstract()) {
    LOG(FATAL) << "java.lang.AbstractMethodError";
  }
  LOG_IF(INFO, INST_DEBUG) << "INVOKE_SPECIAL " << methodToBeInvoked->mName << " " << methodToBeInvoked->mDescriptor << " " << methodToBeInvoked->mClassPtr->mName;
  invokeMethod(frame, methodToBeInvoked);
}

void INVOKE_VIRTUAL::execute(std::shared_ptr<rtda::Frame> frame) {
  std::shared_ptr<rtda::Method> methodPtr = frame->getMethod();
  std::shared_ptr<rtda::ConstantPool> cp = methodPtr->mClassPtr->mConstantPool;
  std::shared_ptr<rtda::Constant> constant = cp->getConstant(index);
  std::shared_ptr<rtda::MethodRefConstant> methodRefInfo = std::dynamic_pointer_cast<rtda::MethodRefConstant>(constant);
  std::shared_ptr<rtda::Method> resolvedMethod = methodRefInfo->resolveMethod();
  if (resolvedMethod->isStatic()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  if (resolvedMethod->mName == "println") {
    //LOG_IF(INFO, INST_DEBUG) << "hack println";
    LOG(WARNING) << "hack println "<< frame->getOperandStack().popLong();
    return;
  }
  void* ref = frame->getOperandStack().getRefFromTop(resolvedMethod->mArgSlotCount -1);
  if (ref == nullptr) {
    
    LOG(FATAL) << "java.lang.NullPointerException";
  }
  rtda::Object* refObj = static_cast<rtda::Object*>(ref);
  if (resolvedMethod->isProtected() 
      && resolvedMethod->mClassPtr->isSuperClassOf(methodPtr->mClassPtr) 
      && resolvedMethod->mClassPtr->getPackageName() != methodPtr->mClassPtr->getPackageName() 
      && refObj->mClass != methodPtr->mClassPtr 
      && !refObj->mClass->isSubClassOf(methodPtr->mClassPtr)) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  std::shared_ptr<rtda::Method> methodToBeInvoked = refObj->mClass->lookupMethodInClass(methodRefInfo->mName, methodRefInfo->mDescriptor);
  if (methodToBeInvoked == nullptr || methodToBeInvoked->isAbstract()) {
    LOG(FATAL) << "java.lang.AbstractMethodError";
  }
  LOG_IF(INFO, INST_DEBUG) << "INVOKE_VIRTUAL " << methodToBeInvoked->mName << " " << methodToBeInvoked->mDescriptor << " " << methodToBeInvoked->mClassPtr->mName;
  invokeMethod(frame, methodToBeInvoked);
}

void INVOKE_INTERFACE::fetchOperands(std::shared_ptr<BytecodeReader> reader) {
  index = uint32_t(reader->readUInt16());
  reader->readUInt8();
  reader->readUInt8();
}

void INVOKE_INTERFACE::execute(std::shared_ptr<rtda::Frame> frame) {
  std::shared_ptr<rtda::Method> methodPtr = frame->getMethod();
  std::shared_ptr<rtda::ConstantPool> cp = methodPtr->mClassPtr->mConstantPool;
  std::shared_ptr<rtda::Constant> constant = cp->getConstant(index);
  std::shared_ptr<rtda::InterfaceMethodRefConstant> methodRefInfo = std::dynamic_pointer_cast<rtda::InterfaceMethodRefConstant>(constant);
  std::shared_ptr<rtda::Method> resolvedMethod = methodRefInfo->resolveInterfaceMethod();
  if (resolvedMethod->isStatic() || resolvedMethod->isPrivate()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  void* ref = frame->getOperandStack().getRefFromTop(resolvedMethod->mArgSlotCount -1);
  if (ref == nullptr) {
    LOG(FATAL) << "java.lang.NullPointerException";
  }
  rtda::Object* refObj = static_cast<rtda::Object*>(ref);
  if (!refObj->mClass->isImplements(methodRefInfo->resolveClass())) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  std::shared_ptr<rtda::Method> methodToBeInvoked = refObj->mClass->lookupMethodInClass(methodRefInfo->mName, methodRefInfo->mDescriptor);
  if (methodToBeInvoked == nullptr || methodToBeInvoked->isAbstract()) {
    LOG(FATAL) << "java.lang.AbstractMethodError";
  }
  if (!methodToBeInvoked->isPublic()) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  LOG_IF(INFO, INST_DEBUG) << "INVOKE_INTERFACE " << methodToBeInvoked->mName << " " << methodToBeInvoked->mDescriptor << " " << methodToBeInvoked->mClassPtr->mName;
  invokeMethod(frame, methodToBeInvoked);
}

} // namespace instructions