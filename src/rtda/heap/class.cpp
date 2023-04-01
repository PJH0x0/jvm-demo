#include "class.h"
#include <memory>
#include "class_member.h"
#include "constant_pool.h"
#include "class_member.h"

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
}