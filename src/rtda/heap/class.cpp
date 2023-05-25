#include "class.h"
#include <memory>
#include "class_member.h"
#include "constant_pool.h"
#include "method.h"
#include "field.h"
#include "object.h"
#include "class_loader.h"
#include "array.h"
#include <glog/logging.h>
#include <stdint.h>
#include <string>

namespace rtda {

Class::Class(std::shared_ptr<classfile::ClassFile> classfile) : mClassfile(classfile) {}
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
std::shared_ptr<Object> Class::newArray(uint32_t count) {
  if (!isArrayClass()) {
    LOG(FATAL) << "Not array class";
  }
  std::shared_ptr<Class> thisPtr = std::shared_ptr<Class>(this);
  switch (mName[1]) {
    case 'Z':
      return std::make_shared<Array<bool>>(thisPtr, count);
    case 'B':
      return std::make_shared<Array<int8_t>>(thisPtr, count);
    case 'C':
      return std::make_shared<Array<uint16_t>>(thisPtr, count);
    case 'S':
      return std::make_shared<Array<int16_t>>(thisPtr, count);
    case 'I':
      return std::make_shared<Array<int32_t>>(thisPtr, count);
    case 'J':
      return std::make_shared<Array<int64_t>>(thisPtr, count);
    case 'F':
      return std::make_shared<Array<float>>(thisPtr, count);
    case 'D':
      return std::make_shared<Array<double>>(thisPtr, count);
    default:
      return std::make_shared<Array<Object*>>(thisPtr, count);
  }
}
}