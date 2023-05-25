#include "class_loader.h"
#include <classfile/class_parser.h>
#include "field.h"
#include "rtda/heap/constant_pool.h"
#include <memory>
#include <rtda/slot.h>
#include <stdint.h>
#include <glog/logging.h>
#include <string>
#include <vector>


namespace rtda {
std::shared_ptr<Class> ClassLoader::loadClass(std::string name) {
  //LOG(INFO) << "load class " << name;
  if (mLoadedClasses.find(name) != mLoadedClasses.end()) {
    return mLoadedClasses[name];
  }
  if (name[0] == '[') {
    return loadArrayClass(name);
  }
  return loadNonArrayClass(name);
}

std::shared_ptr<Class> ClassLoader::loadArrayClass(std::string name) {
  std::shared_ptr<Class> clssPtr = std::make_shared<Class>(name);
  //clssPtr->startInit(this);
  clssPtr->startInitArrayClass(this);
  mLoadedClasses[name] = clssPtr;
  return clssPtr;
}
std::shared_ptr<Class> ClassLoader::loadNonArrayClass(std::string name) {
  std::shared_ptr<classfile::ClassData> classData = mClsReader->readClass(name);
  std::shared_ptr<Class> clssPtr = defineClass(classData);
  linkClass(clssPtr);
  return clssPtr;
}
std::shared_ptr<Class> ClassLoader::defineClass(std::shared_ptr<classpath::ClassData> data) {
  std::shared_ptr<classfile::ClassFile> classFile = classfile::parse(data);
  if (classFile == nullptr) {
    LOG(ERROR) << "parse class file failed";
  }
  std::shared_ptr<Class> classPtr = std::make_shared<Class>(classFile);
  classPtr->startInit(this);
  mLoadedClasses[classPtr->getName()] = classPtr;
  return classPtr;
}
std::shared_ptr<Class> ClassLoader::resolveSuperClass(Class* classPtr) {
  if (classPtr->getName() != "java/lang/Object") {
    return loadClass(classPtr->getSuperClassName());
  }
  return nullptr;
}
void ClassLoader::resolveInterfaces(Class* classPtr, std::vector<std::shared_ptr<Class>>& interfaces) {
  int interfaceCount = classPtr->getInterfaceNames().size();
  if (interfaceCount > 0) {
    interfaces.resize(interfaceCount);
    for (int i = 0; i < interfaceCount; i++) {
      interfaces[i] = loadClass(classPtr->getInterfaceNames()[i]);
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
  if (classPtr->getSuperClass() != nullptr) {
    slotId = classPtr->getSuperClass()->getInstanceSlotCount();
  }
  for (auto field : classPtr->getFields()) {
    if (!field->isStatic()) {
      //field->getSlotId() = slotId;
      field->setSlotId(slotId);
      slotId++;
      if (field->isLongOrDouble()) {
        slotId++;
      }
    }
  }
  classPtr->setInstanceSlotCount(slotId);
}
void calcStaticFieldSlotIds(std::shared_ptr<Class> classPtr) {
  int slotId = 0;
  for (auto field : classPtr->getFields()) {
    if (field->isStatic()) {
      //field->getSlotId() = slotId;
      field->setSlotId(slotId);
      slotId++;
      if (field->isLongOrDouble()) {
        slotId++;
      }
    }
  }
  classPtr->setStaticSlotCount(slotId);
}
void allocAndInitStaticVars(std::shared_ptr<Class> classPtr) {
  auto staticVars = std::make_shared<Slots>(classPtr->getStaticSlotCount());
  for (auto field : classPtr->getFields()) {
    if (field->isStatic() && field->isFinal()) {
      initStaticFinalVar(classPtr, field);
    }
  }
  classPtr->setStaticVars(staticVars);
}
void initStaticFinalVar(std::shared_ptr<Class> classPtr, std::shared_ptr<Field> field) {
  // todo
  std::shared_ptr<ConstantPool> cp = classPtr->getConstantPool();
  const std::vector<std::shared_ptr<Constant>>& constants = cp->constants();
  std::shared_ptr<Constant> constant = constants[field->getConstValueIndex()];
  int slotId = field->getSlotId();
  std::string descriptor = field->getDescriptor();
  switch (descriptor[0]) {
    case 'Z':
    case 'B':
    case 'C':
    case 'S':
    case 'I': {
      int32_t value = std::static_pointer_cast<IntegerConstant>(constant)->value();
      classPtr->getStaticVars()->setInt(field->getConstValueIndex(), value);
      break;
    }
    case 'F':
      classPtr->getStaticVars()->setFloat(field->getConstValueIndex(), std::static_pointer_cast<FloatConstant>(constant)->value());
      break;
    case 'J':
      classPtr->getStaticVars()->setLong(field->getConstValueIndex(), std::static_pointer_cast<LongConstant>(constant)->value());
      break;
    case 'D':
      classPtr->getStaticVars()->setDouble(field->getConstValueIndex(), std::static_pointer_cast<DoubleConstant>(constant)->value());
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