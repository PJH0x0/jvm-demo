#include "class.h"
#include <memory>
#include "class_member.h"
#include "constant_pool.h"
#include "method.h"
#include "field.h"
#include "object.h"
#include "class_loader.h"
#include <glog/logging.h>
#include <stdint.h>
#include <string>

namespace rtda {
std::unordered_map<std::string, std::string> Class::mPrimitiveTypes = {
  {"void", "V"},
  {"boolean", "Z"},
  {"byte", "B"},
  {"short", "S"},
  {"int", "I"},
  {"long", "J"},
  {"char", "C"},
  {"float", "F"},
  {"double", "D"}
};
Class::Class(std::shared_ptr<classfile::ClassFile> classfile) 
  : mClassfile(classfile),
    mAccessFlags(0), 
    mInited(false), 
    mLoader(nullptr), 
    mInstanceSlotCount(0),
    mStaticSlotCount(0){}
Class::Class(std::string name) : mName(name) {}
void Class::startInit(ClassLoader* classLoader) {
  mLoader = std::shared_ptr<ClassLoader>(classLoader);
  mAccessFlags = mClassfile->accessFlags;
  std::shared_ptr<classfile::ConstantPool> constantPool = mClassfile->constantPool;
  mName = mClassfile->getClassName();

  mSuperClassName = mClassfile->getSuperClassName();
  mSuperClass = mLoader->resolveSuperClass(this);

  mClassfile->getInterfaceNames(mInterfaceNames);
  mLoader->resolveInterfaces(this, mInterfaces);
  
  std::shared_ptr<Class> thisptr = std::shared_ptr<Class>(this);
  //TODO: init fileds
  createFields(thisptr, mClassfile->fields, mFields);
  //TODO: init constant pool
  mConstantPool = std::make_shared<ConstantPool>(thisptr, constantPool);
  //TODO: init methods
  createMethods(thisptr, mClassfile->methods, mMethods);
  mInited = true;
}
void Class::startInitArrayClass(ClassLoader* classLoader) {
  mLoader = std::shared_ptr<ClassLoader>(classLoader);
  mAccessFlags = ACC_PUBLIC;
  mSuperClassName = "java/lang/Object";
  mSuperClass = mLoader->loadClass(mSuperClassName);
  mInterfaces.push_back(mLoader->loadClass("java/lang/Cloneable"));
  mInterfaces.push_back(mLoader->loadClass("java/io/Serializable"));
  mInited = true;
}
std::shared_ptr<Field> Class::lookupField(std::string name, std::string descriptor) {
  for (auto field : mFields) {
    if (field->getName() == name && field->getDescriptor() == descriptor) {
      return field;
    }
  }
  for (auto interface : mInterfaces) {
    std::shared_ptr<Field> field = interface->lookupField(name, descriptor);
    if (field != nullptr) {
      return field;
    }
  }
  if (mSuperClass != nullptr) {
    std::shared_ptr<Field> field = mSuperClass->lookupField(name, descriptor);
    if (field != nullptr) {
      return field;
    }
  }
  return nullptr;
}

std::shared_ptr<Method> Class::lookupMethod(std::string name, std::string descriptor) {
  std::shared_ptr<Method> method = lookupMethodInClass(name, descriptor);
  if (method == nullptr) {
    method = lookupMethodInInterfaces(name, descriptor);
  }
  return method;
}

std::shared_ptr<Method> Class::lookupMethodInClass(std::string name, std::string descriptor) {
  for (auto method : mMethods) {
    if (method->getName() == name && method->getDescriptor() == descriptor) {
      return method;
    }
  }
  if (mSuperClass != nullptr) {
    return mSuperClass->lookupMethodInClass(name, descriptor);
  }
  return nullptr;
}

std::shared_ptr<Method> Class::lookupMethodInInterfaces(std::string name, std::string descriptor) {
  for (auto interface : mInterfaces) {
    std::shared_ptr<Method> method = interface->lookupMethodInClass(name, descriptor);
    if (method != nullptr) {
      return method;
    }
  }
  return nullptr;
}

std::shared_ptr<Object> Class::newObject() {
  return std::make_shared<Object>(std::shared_ptr<Class>(this));
}

bool Class::isSubClassOf(std::shared_ptr<Class> other) {
  std::shared_ptr<Class> c = mSuperClass;
  while (c != nullptr) {
    if (c == other) {
      return true;
    }
    c = c->mSuperClass;
  }
  return false;
}

bool Class::isSuperClassOf(std::shared_ptr<Class> other) {
  std::shared_ptr<Class> c = other->mSuperClass;
  while (c != nullptr) {
    if (c.get() == this) {
      return true;
    }
    c = c->mSuperClass;
  }
  return false;
}

bool Class::isImplements(std::shared_ptr<Class> other) {
  for (auto interface : mInterfaces) {
    if (interface == other || interface->isSubInterfaceOf(other)) {
      return true;
    }
  }
  return false;
}
bool Class::isSubInterfaceOf(std::shared_ptr<Class> other) {
  for (auto interface : mInterfaces) {
    if (interface == other || interface->isSubInterfaceOf(other)) {
      return true;
    }
  }
  return false;
}
bool Class::isAssignableFrom(std::shared_ptr<Class> other) {
  if (other == nullptr) {
    return false;
  }
  if (this == other.get()) {
    return true;
  }
  if (other->isSubClassOf(std::shared_ptr<Class>(this))) {
    return true;
  }
  if (isInterface()) {
    return other->isImplements(std::shared_ptr<Class>(this));
  }
  return false;
}
std::shared_ptr<Method> Class::getMainMethod() {
  return getStaticMethod("main", "([Ljava/lang/String;)V");
}
std::shared_ptr<Method> Class::getStaticMethod(std::string name, std::string descriptor) {
  for (auto method : mMethods) {
    if (method->getName() == name && method->getDescriptor() == descriptor && method->isStatic()) {
      return method;
    }
  }
  return nullptr;
}
Object* Class::newArray(uint32_t count) {
  if (!isArrayClass()) {
    LOG(FATAL) << "Not array class";
  }
  std::shared_ptr<Class> thisPtr = std::shared_ptr<Class>(this);
  switch (mName[1]) {
    case 'Z':
      return new Object(thisPtr, count, AT_BOOLEAN);
    case 'B':
      return new Object(thisPtr, count, AT_BYTE);
    case 'C':
      return new Object(thisPtr, count, AT_CHAR);
    case 'S':
      return new Object(thisPtr, count, AT_SHORT);
    case 'I':
      return new Object(thisPtr, count, AT_INT);
    case 'J':
      return new Object(thisPtr, count, AT_LONG);
    case 'F':
      return new Object(thisPtr, count, AT_FLOAT);
    case 'D':
      return new Object(thisPtr, count, AT_DOUBLE);
    default:
      return new Object(thisPtr, count, AT_OBJECT);
  }
}
std::shared_ptr<Class> Class::getPrimitiveArrayClass(std::shared_ptr<ClassLoader> classLoader,
                                                     uint8_t atype) {
  switch (atype) {
    case AT_BOOLEAN:
      return classLoader->loadClass("[Z");
    case AT_BYTE:
      return classLoader->loadClass("[B");
    case AT_CHAR:
      return classLoader->loadClass("[C");
    case AT_SHORT:
      return classLoader->loadClass("[S");
    case AT_INT:
      return classLoader->loadClass("[I");
    case AT_LONG:
      return classLoader->loadClass("[J");
    case AT_FLOAT:
      return classLoader->loadClass("[F");
    case AT_DOUBLE:
      return classLoader->loadClass("[D");
    default:
      LOG(FATAL) << "Invalid atype: " << atype;
  }
}

std::shared_ptr<Class> Class::getArrayClass() {
  std::string arrayClassName = getArrayClassName(mName);
  return mLoader->loadClass(arrayClassName);
}
std::string Class::toDescriptor(std::string className) {
  LOG(INFO) << "toDescriptor: " << className;
  if (className[0] == '[') {
    return className;
  }
  for (auto& pair : mPrimitiveTypes) {
    if (pair.first == className) {
      return pair.second;
    }
  }
  return "L" + className + ";";
  LOG(FATAL) << "Invalid class name: " << className;
}
std::string Class::getArrayClassName(std::string className) {
  return "[" + toDescriptor(className);
}
std::string Class::toClassName(std::string descriptor) {
  if (descriptor[0] == '[') {
    return descriptor;
  }
  if (descriptor[0] == 'L') {
    return descriptor.substr(1, descriptor.size() - 2);
  }
  for (auto& pair : mPrimitiveTypes) {
    if (pair.second == descriptor) {
      return pair.first;
    }
  }
  LOG(FATAL) << "Invalid descriptor: " << descriptor;
}
std::string Class::getComponentClassName(std::string className) {
  if (className[0] == '[') {
    std::string componentTypeDescriptor = className.substr(1, className.size() - 1);
    return toClassName(componentTypeDescriptor);
  }
  LOG(FATAL) << "Not array class: " << className;
}
std::shared_ptr<Class> Class::getComponentClass() {
  std::string componentClassName = getComponentClassName(mName);
  return mLoader->loadClass(componentClassName);
}

                                                    
}