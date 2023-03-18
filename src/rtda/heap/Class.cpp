#include "Class.h"
#include <memory>

namespace rtda {

Class::Class(std::shared_ptr<ClassFile> classfile) : mAccessFlags(classfile->accessFlags) {
  std::shared_ptr<classfile::ConstantPool> constantPool = classfile->constantPool;
  mName = classfile->getClassName();
  mSuperClassName = classfile->getSuperClassName();
  classfile->getInterfaceNames(mInterfaceNames);
  //TODO: init fileds
  //TODO: init constant pool
  //TODO: init methods
}
}