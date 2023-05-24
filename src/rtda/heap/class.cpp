#include "class.h"
#include <memory>
#include "class_member.h"
#include "constant_pool.h"
#include "method.h"
#include "field.h"
#include "object.h"
#include <glog/logging.h>
#include <stdint.h>

namespace rtda {

Class::Class(std::shared_ptr<classfile::ClassFile> classfile) : mClassfile(classfile) {
  
}
void Class::startInit() {
  mAccessFlags = mClassfile->accessFlags;
  std::shared_ptr<classfile::ConstantPool> constantPool = mClassfile->constantPool;
  mName = mClassfile->getClassName();
  mSuperClassName = mClassfile->getSuperClassName();
  mClassfile->getInterfaceNames(mInterfaceNames);
  std::shared_ptr<Class> thisptr = std::shared_ptr<Class>(this);
  //TODO: init fileds
  createFields(thisptr, mClassfile->fields, mFields);
  //TODO: init constant pool
  mConstantPool = std::make_shared<ConstantPool>(thisptr, constantPool);
  //TODO: init methods
  createMethods(thisptr, mClassfile->methods, mMethods);
  mInited = true;
}
std::shared_ptr<Field> Class::lookupField(std::string name, std::string descriptor) {
  for (auto field : mFields) {
    if (field->mName == name && field->mDescriptor == descriptor) {
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
    if (method->mName == name && method->mDescriptor == descriptor) {
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
    if (method->mName == name && method->mDescriptor == descriptor && method->isStatic()) {
      return method;
    }
  }
  return nullptr;
}
}