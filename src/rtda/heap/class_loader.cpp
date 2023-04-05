#include "class_loader.h"
#include <classfile/class_parser.h>
#include "class_member.h"
#include "rtda/heap/constant_pool.h"
#include <memory>
#include <rtda/slot.h>
#include <sys/_types/_int32_t.h>
#include <glog/logging.h>

namespace rtda {
std::shared_ptr<Class> ClassLoader::loadClass(std::string name) {
  LOG(INFO) << "load class " << name;
  if (mLoadedClasses.find(name) != mLoadedClasses.end()) {
    return mLoadedClasses[name];
  }
  return loadNonArrayClass(name);
}
std::shared_ptr<Class> ClassLoader::loadNonArrayClass(std::string name) {
  std::shared_ptr<classfile::ClassData> classData = mClsReader->readClass(name);
  return defineClass(classData);  
}
std::shared_ptr<Class> ClassLoader::defineClass(std::shared_ptr<classpath::ClassData> data) {
  std::shared_ptr<classfile::ClassFile> classFile = classfile::parse(data);
  if (classFile == nullptr) {
    LOG(ERROR) << "parse class file failed";
  }
  std::shared_ptr<Class> classPtr = std::make_shared<Class>(classFile);
  classPtr->startInit();
  classPtr->mSuperClass = resolveSuperClass(classPtr);
  resolveInterfaces(classPtr);
  mLoadedClasses[classPtr->mName] = classPtr;
  return classPtr;
}
std::shared_ptr<Class> ClassLoader::resolveSuperClass(std::shared_ptr<Class> classPtr) {
  if (classPtr->mName != "java/lang/Object") {
    return loadClass(classPtr->mSuperClassName);
  }
  return nullptr;
}
void ClassLoader::resolveInterfaces(std::shared_ptr<Class> classPtr) {
  int interfaceCount = classPtr->mInterfaceNames.size();
  if (interfaceCount > 0) {
    classPtr->mInterfaces.resize(interfaceCount);
    for (int i = 0; i < interfaceCount; i++) {
      classPtr->mInterfaces[i] = loadClass(classPtr->mInterfaceNames[i]);
    }
  }
}
void linkClass(std::shared_ptr<Class> classPtr) {
  verifyClass(classPtr);
  prepareClass(classPtr);
}
void verifyClass(std::shared_ptr<Class> classPtr) {
  // todo
}
void prepareClass(std::shared_ptr<Class> classPtr) {
  calcInstanceFieldSlotIds(classPtr);
  calcStaticFieldSlotIds(classPtr);
  allocAndInitStaticVars(classPtr);
}
void calcInstanceFieldSlotIds(std::shared_ptr<Class> classPtr) {
  int slotId = 0;
  if (classPtr->mSuperClass != nullptr) {
    slotId = classPtr->mSuperClass->mInstanceSlotCount;
  }
  for (auto field : classPtr->mFields) {
    if (!field->isStatic()) {
      field->mSlotId = slotId;
      slotId++;
      if (field->isLongOrDouble()) {
        slotId++;
      }
    }
  }
  classPtr->mInstanceSlotCount = slotId;
}
void calcStaticFieldSlotIds(std::shared_ptr<Class> classPtr) {
  int slotId = 0;
  for (auto field : classPtr->mFields) {
    if (field->isStatic()) {
      field->mSlotId = slotId;
      slotId++;
      if (field->isLongOrDouble()) {
        slotId++;
      }
    }
  }
  classPtr->mStaticSlotCount = slotId;
}
void allocAndInitStaticVars(std::shared_ptr<Class> classPtr) {
  classPtr->mStaticVars = std::make_shared<Slots>(classPtr->mStaticSlotCount);
  for (auto field : classPtr->mFields) {
    if (field->isStatic() && field->isFinal()) {
      initStaticFinalVar(classPtr, field);
    }
  }
}
void initStaticFinalVar(std::shared_ptr<Class> classPtr, std::shared_ptr<Field> field) {
  // todo
  std::shared_ptr<ConstantPool> cp = classPtr->mConstantPool;
  std::shared_ptr<Constant> constant = cp->mConstants[field->mConstValueIndex];
  int slotId = field->mSlotId;
  switch (field->mDescriptor[0]) {
    case 'Z':
    case 'B':
    case 'C':
    case 'S':
    case 'I': {
      int32_t value = std::static_pointer_cast<IntegerConstant>(constant)->value();
      classPtr->mStaticVars->setInt(field->mConstValueIndex, value);
      break;
    }
    case 'F':
      classPtr->mStaticVars->setFloat(field->mConstValueIndex, std::static_pointer_cast<FloatConstant>(constant)->value());
      break;
    case 'J':
      classPtr->mStaticVars->setLong(field->mConstValueIndex, std::static_pointer_cast<LongConstant>(constant)->value());
      break;
    case 'D':
      classPtr->mStaticVars->setDouble(field->mConstValueIndex, std::static_pointer_cast<DoubleConstant>(constant)->value());
      break;
    case 'L':
    case '[':
      //classPtr->mStaticVars[slotId].mRef = std::static_pointer_cast<StringConstant>(constant)->value();
      break;
    default:
      break;
  }
}

}// namespace rtda