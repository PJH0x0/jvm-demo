#include "class.h"
#include <memory>
#include "class_member.h"
#include "constant_pool.h"

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
}