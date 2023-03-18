#pragma once

#include <memory>
#include <string>
#include <vector>
#include <classfile/class_parser.h>
using classfile::ClassFile;

namespace rtda {
enum ACCESS_FLAG {
  ACC_PUBLIC       = 0x0001, // class field method
	ACC_PRIVATE      = 0x0002, //       field method
	ACC_PROTECTED    = 0x0004, //       field method
	ACC_STATIC       = 0x0008, //       field method
	ACC_FINAL        = 0x0010, // class field method
	ACC_SUPER        = 0x0020, // class
	ACC_SYNCHRONIZED = 0x0020, //             method
	ACC_VOLATILE     = 0x0040, //       field
	ACC_BRIDGE       = 0x0040, //             method
	ACC_TRANSIENT    = 0x0080, //       field
	ACC_VARARGS      = 0x0080, //             method
	ACC_NATIVE       = 0x0100, //             method
	ACC_INTERFACE    = 0x0200, // class
	ACC_ABSTRACT     = 0x0400, // class       method
	ACC_STRICT       = 0x0800, //             method
	ACC_SYNTHETIC    = 0x1000, // class field method
	ACC_ANNOTATION   = 0x2000, // class
	ACC_ENUM         = 0x4000, // class field
};
class ClassLoader;
class ConstantPool;
class Field;
class Method;
class Slot;
struct Class {
  uint16_t mAccessFlags;
  std::string mName;
  std::string mSuperClassName;
  std::vector<std::string> mInterfaceNames;
  std::shared_ptr<ConstantPool> mConstantPool;
  std::vector<std::shared_ptr<Field>> mFields;
  std::vector<std::shared_ptr<Method>> mMethods;
  std::shared_ptr<ClassLoader> mLoader;
  std::shared_ptr<Class> mSuperClass;
  std::vector<std::shared_ptr<Class>> mInterfaces;
  uint32_t mInstanceSlotCount;
  uint32_t mStaticSlotCount;
  std::shared_ptr<Slot> mStaticVars;
  Class(std::shared_ptr<ClassFile> mClassfile);
  bool isPublic() {
    return (mAccessFlags & ACC_PUBLIC) != 0;
  }
  bool isFinal() {
    return (mAccessFlags & ACC_FINAL) != 0;
  }
  bool isSuper() {
    return (mAccessFlags & ACC_SUPER) != 0;
  }
  bool isInterface() {
    return (mAccessFlags & ACC_INTERFACE) != 0;
  }
  bool isAbstract() {
    return (mAccessFlags & ACC_ABSTRACT) != 0;
  }
  bool isSythetic() {
    return (mAccessFlags & ACC_SYNTHETIC) != 0;
  }
  bool isAnnotation() {
    return (mAccessFlags & ACC_ANNOTATION) != 0;
  }
  bool isEnum() {
    return (mAccessFlags & ACC_ENUM) != 0;
  }
};

}