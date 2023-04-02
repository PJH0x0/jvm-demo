#include "class.h"
#include <memory>
#include "class_member.h"
#include "constant_pool.h"
#include "class_member.h"
#include "object.h"

namespace rtda {

Class::Class(std::shared_ptr<ClassFile> classfile) : mAccessFlags(classfile->accessFlags) {
  std::shared_ptr<classfile::ConstantPool> constantPool = classfile->constantPool;
  mName = classfile->getClassName();
  mSuperClassName = classfile->getSuperClassName();
  classfile->getInterfaceNames(mInterfaceNames);
  //TODO: init fileds
  createFields(std::shared_ptr<Class>(this), classfile->fields, mFields);
  //TODO: init constant pool
  mConstantPool = std::make_shared<ConstantPool>(std::shared_ptr<Class>(this), constantPool);
  //TODO: init methods
  createMethods(std::shared_ptr<Class>(this), classfile->methods, mMethods);
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