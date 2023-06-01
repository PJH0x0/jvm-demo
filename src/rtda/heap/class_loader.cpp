#include "class_loader.h"
#include <classfile/class_parser.h>
#include "field.h"
#include "rtda/heap/constant_pool.h"
#include "object.h"
#include <memory>
#include <rtda/slots.h>
#include <stdint.h>
#include <glog/logging.h>
#include <string>
#include <vector>
#include <mutex>


namespace rtda {
std::unordered_map<std::string, std::shared_ptr<ClassLoader>> ClassLoader::mLoaders;
std::shared_ptr<ClassLoader> ClassLoader::mBootClassLoader;
std::once_flag sClassLoaderFlag;
std::shared_ptr<Class> ClassLoader::loadClass(std::string name) {
  //LOG(INFO) << "load class " << name;
  if (mLoadedClasses.find(name) != mLoadedClasses.end()) {
    return mLoadedClasses[name];
  }
  std::shared_ptr<Class> clssPtr = nullptr;
  if (name[0] == '[') {
    clssPtr = loadArrayClass(name);
  } else {
    clssPtr = loadNonArrayClass(name);
  }
  auto jlClassClass = mLoadedClasses["java/lang/Class"];
  if (jlClassClass != nullptr) {
    //class object's class is java/lang/Class
    clssPtr->setJClass(jlClassClass->newObject());
    //Object's class object's extra is point to Object's class
    clssPtr->getJClass()->setExtra(clssPtr.get());
  }
  return clssPtr;
}

std::shared_ptr<Class> ClassLoader::loadArrayClass(std::string name) {
  std::shared_ptr<Class> clssPtr = std::make_shared<Class>(name);
  //clssPtr->startInit(this);
  clssPtr->startLoadArrayClass();
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
  classPtr->startLoad();
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
  classPtr->setStaticVars(staticVars);
  for (auto field : classPtr->getFields()) {
    if (field->isStatic() && field->isFinal()) {
      initStaticFinalVar(classPtr, field);
    }
  }
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
      std::string name = field->getName();
      if (name == "$assertionsDisabled") {
        LOG(INFO) << "Skip $assertionsDisabled field";
        return;
      }
      int32_t value = std::static_pointer_cast<IntegerConstant>(constant)->value();
      classPtr->getStaticVars()->setInt(field->getSlotId(), value);
      break;
    }
    case 'F':
      classPtr->getStaticVars()->setFloat(field->getSlotId(), std::static_pointer_cast<FloatConstant>(constant)->value());
      break;
    case 'J':
      classPtr->getStaticVars()->setLong(field->getSlotId(), std::static_pointer_cast<LongConstant>(constant)->value());
      break;
    case 'D':
      classPtr->getStaticVars()->setDouble(field->getSlotId(), std::static_pointer_cast<DoubleConstant>(constant)->value());
      break;
    case 'L':
    case '[':
      //classPtr->mStaticVars[slotId].mRef = std::static_pointer_cast<StringConstant>(constant)->value();
      //classPtr->getStaticVars()->setRef(field->getConstValueIndex(), std::static_pointer_cast<FieldRefConstant>(constant)->value());
      break;
    default:
      break;
  }
  if (descriptor == "L/java/lang/String;") {
    std::string str = std::static_pointer_cast<StringConstant>(constant)->value();
    auto jStr = Class::newJString(str);
    classPtr->getStaticVars()->setRef(field->getSlotId(), jStr);
  }
}
void ClassLoader::loadBasicClass() {
  auto jlClass = loadClass("java/lang/Class");
  for (auto& pair : mLoadedClasses) {
    pair.second->setJClass(jlClass->newObject());
    pair.second->getJClass()->setExtra(pair.second.get());
  }
}

void ClassLoader::loadPrimitiveClasses() {
  for (auto& pair : Class::mPrimitiveTypes) {
    auto classPtr = std::make_shared<Class>();
    classPtr->setAccessFlags(ACCESS_FLAG::ACC_PUBLIC);
    classPtr->setName(pair.first);
    classPtr->setClassLoader(getBootClassLoader(nullptr));
    //classPtr->startLoad();
    classPtr->setJClass(mLoadedClasses["java/lang/Class"]->newObject());
    classPtr->getJClass()->setExtra(classPtr.get());
    mLoadedClasses[pair.first] = classPtr;
  }
}

std::shared_ptr<ClassLoader> ClassLoader::getBootClassLoader(
      std::shared_ptr<classpath::ClassPathParser> bootClsReader) {
  std::call_once(sClassLoaderFlag, [&]() {
    if (nullptr == bootClsReader) {
      LOG(FATAL) << "boot class path is null";
    }
    mBootClassLoader = std::shared_ptr<ClassLoader>(new ClassLoader(bootClsReader));
  });
  
  return mBootClassLoader;
}

std::shared_ptr<ClassLoader> ClassLoader::getLoader(std::string name) {
  //TODO
  return nullptr;
}

void ClassLoader::registerLoader(std::string name, std::shared_ptr<ClassLoader> loader) {
  //TODO
}

}// namespace rtda