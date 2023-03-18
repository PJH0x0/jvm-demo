#include "class.h"
#include <memory>

namespace rtda {

Class::Class(std::shared_ptr<ClassFile> classfile) : accessFlags(classfile->accessFlags) {
  std::shared_ptr<classfile::ConstantPool> constantPool = classfile->constantPool;
  name = classfile->getClassName();
  superClassName = classfile->getSuperClassName();
  classfile->getInterfaceNames(interfaceNames);
  //TODO: init fileds
  //TODO: init constant pool
  //TODO: init methods

}
}