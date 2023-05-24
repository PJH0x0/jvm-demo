#include "constant_pool.h"
#include "classfile/class_parser.h"
#include "classfile/constant_pool.h"
#include "class_loader.h"
#include "class.h"
#include "field.h"
#include "method.h"
#include <glog/logging.h>
#include <memory>
#include <string>
#include <stdint.h>

namespace rtda {
std::shared_ptr<Constant> ConstantPool::getConstant(uint32_t index) {
  if (index >= 0 && index < mConstants.size()) {
    return mConstants[index];
  }
  LOG(ERROR) << "Not found contant in " << index;
  return nullptr;
}

ConstantPool::ConstantPool(std::shared_ptr<Class> clsPtr, std::shared_ptr<classfile::ConstantPool> cfConstantPool) : mClsPtr(clsPtr) {
  mConstants.resize(cfConstantPool->constantPoolCount);
  for (int i = 1; i < cfConstantPool->constantPoolCount; i++) {
    auto cfConstant = cfConstantPool->getConstantInfo(i);
    int32_t tag = (int32_t)cfConstant->mTag;
    switch (tag) {
      case classfile::CONSTANT_Integer:
        mConstants[i] = std::make_shared<IntegerConstant>(std::dynamic_pointer_cast<classfile::ConstantIntegerInfo>(cfConstant));
        break;
      case classfile::CONSTANT_Float:
        mConstants[i] = std::make_shared<FloatConstant>(std::dynamic_pointer_cast<classfile::ConstantFloatInfo>(cfConstant));
        break;
      case classfile::CONSTANT_Long:
        mConstants[i] = std::make_shared<LongConstant>(std::dynamic_pointer_cast<classfile::ConstantLongInfo>(cfConstant));
        mConstants[++i] = nullptr;
        break;
      case classfile::CONSTANT_Double:
        mConstants[i] = std::make_shared<DoubleConstant>(std::dynamic_pointer_cast<classfile::ConstantDoubleInfo>(cfConstant));
        mConstants[++i] = nullptr;
        break;
      case classfile::CONSTANT_String: {
        std::shared_ptr<classfile::ConstantStringInfo> cfStringInfo = std::dynamic_pointer_cast<classfile::ConstantStringInfo>(cfConstant);
        mConstants[i] = std::make_shared<StringConstant>(cfConstantPool->getUtf8(cfStringInfo->stringIndex));
        break;
      }
      case classfile::CONSTANT_Class: {
        std::shared_ptr<classfile::ConstantClassInfo> cfClassInfo = std::dynamic_pointer_cast<classfile::ConstantClassInfo>(cfConstant);
        mConstants[i] = std::make_shared<ClassRefConstant>(std::shared_ptr<ConstantPool>(this), cfConstantPool->getUtf8(cfClassInfo->nameIndex));
        break;
      }
      case classfile::CONSTANT_Fieldref: {
        std::shared_ptr<classfile::ConstantFieldrefInfo> cfFieldrefInfo = std::dynamic_pointer_cast<classfile::ConstantFieldrefInfo>(cfConstant);
        std::string className = cfConstantPool->getClassName(cfFieldrefInfo->classIndex);
        std::string fieldName;
        std::string fieldDescriptor;
        cfConstantPool->getNameAndType(cfFieldrefInfo->nameAndTypeIndex, fieldName, fieldDescriptor);
        mConstants[i] = std::make_shared<FieldRefConstant>(std::shared_ptr<ConstantPool>(this), className, fieldName, fieldDescriptor);
        break;
      }
      case classfile::CONSTANT_Methodref: {
        std::shared_ptr<classfile::ConstantMethodrefInfo> cfMethodrefInfo = std::dynamic_pointer_cast<classfile::ConstantMethodrefInfo>(cfConstant);
        std::string className = cfConstantPool->getClassName(cfMethodrefInfo->classIndex);
        std::string methodName;
        std::string methodDescriptor;
        cfConstantPool->getNameAndType(cfMethodrefInfo->nameAndTypeIndex, methodName, methodDescriptor);
        mConstants[i] = std::make_shared<MethodRefConstant>(std::shared_ptr<ConstantPool>(this), className, methodName, methodDescriptor);
        break;
      }
      case classfile::CONSTANT_InterfaceMethodref: {
        std::shared_ptr<classfile::ConstantInterfaceMethodrefInfo> cfInterfaceMethodrefInfo = std::dynamic_pointer_cast<classfile::ConstantInterfaceMethodrefInfo>(cfConstant);
        std::string className = cfConstantPool->getClassName(cfInterfaceMethodrefInfo->classIndex);
        std::string methodName;
        std::string methodDescriptor;
        cfConstantPool->getNameAndType(cfInterfaceMethodrefInfo->nameAndTypeIndex, methodName, methodDescriptor);
        mConstants[i] = std::make_shared<InterfaceMethodRefConstant>(std::shared_ptr<ConstantPool>(this), className, methodName, methodDescriptor);
        break;
      }
      case classfile::CONSTANT_NameAndType:
        //mConstants[i] = std::make_shared<NameAndTypeConstant>(std::dynamic_pointer_cast<classfile::ConstantNameAndTypeInfo>(cfConstant));
        break;
      case classfile::CONSTANT_MethodHandle:
        //mConstants[i] = std::make_shared<MethodHandleConstant>(std::dynamic_pointer_cast<classfile::ConstantMethodHandleInfo>(cfConstant));
        break;
      case classfile::CONSTANT_MethodType:
        //mConstants[i] = std::make_shared<MethodTypeConstant>(std::dynamic_pointer_cast<classfile::ConstantMethodTypeInfo>(cfConstant));
        break;
      case classfile::CONSTANT_InvokeDynamic:
        //mConstants[i] = std::make_shared<InvokeDynamicConstant>(std::dynamic_pointer_cast<classfile::ConstantInvokeDynamicInfo>(cfConstant));
        break;
      default:
        //LOG(ERROR) << "Not found constant type " << cfConstant->mTag;
        break;
    }
  }
}
std::shared_ptr<Class> SymRefConstant::resolveClass() {
  if (mClsPtr == nullptr) {
    mClsPtr = mConstantPool->getClass()->getClassLoader()->loadClass(mClassName);
  }
  if (!mClsPtr->isAccessibleTo(mConstantPool->getClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  return mClsPtr;
}
std::shared_ptr<Field> FieldRefConstant::resolveField() {
  if (mFieldPtr == nullptr) {
    std::shared_ptr<Class> d = resolveClass();
    mFieldPtr = d->lookupField(name(), descriptor());
    if (mFieldPtr == nullptr) {
      LOG(FATAL) << "java.lang.NoSuchFieldError";
    }
  }
  if (!mFieldPtr->isAccessibleTo(constantPool()->getClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  return mFieldPtr;
}
std::shared_ptr<Method> MethodRefConstant::resolveMethod() {
  if (nullptr != mMethodPtr) {
    return mMethodPtr;
  }
  std::shared_ptr<Class> d = resolveClass();
  if (d->isInterface()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  std::shared_ptr<Method> method = d->lookupMethod(name(), descriptor());
  if (nullptr == method) {
    LOG(FATAL) << "java.lang.NoSuchMethodError";
  }
  if (!method->isAccessibleTo(constantPool()->getClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  mMethodPtr = method;
  return mMethodPtr;
}
std::shared_ptr<Method> InterfaceMethodRefConstant::resolveInterfaceMethod() {
  if (nullptr != mMethodPtr) {
    return mMethodPtr;
  }
  std::shared_ptr<Class> d = resolveClass();
  if (!d->isInterface()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  std::shared_ptr<Method> method = d->lookupMethod(name(), descriptor());
  if (nullptr == method) {
    LOG(FATAL) << "java.lang.NoSuchMethodError";
  }
  if (!method->isAccessibleTo(constantPool()->getClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  mMethodPtr = method;
  return mMethodPtr;
}
}