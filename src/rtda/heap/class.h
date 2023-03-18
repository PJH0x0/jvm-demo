#pragma once

#include <memory>
#include <string>
#include <vector>
#include <classfile/ClassParser.h>
using classfile::ClassFile;

namespace rtda {
class ClassLoader;
class ConstantPool;
class Field;
class Method;
class Slot;
struct Class {
  uint16_t accessFlags;
  std::string name;
  std::string superClassName;
  std::vector<std::string> interfaceNames;
  std::shared_ptr<ConstantPool> constantPool;
  std::vector<std::shared_ptr<Field>> fields;
  std::vector<std::shared_ptr<Method>> methods;
  std::shared_ptr<ClassLoader> loader;
  std::shared_ptr<Class> superClass;
  std::vector<std::shared_ptr<Class>> interfaces;
  uint32_t instanceSlotCount;
  uint32_t staticSlotCount;
  std::shared_ptr<Slot> staticVars;
  Class(std::shared_ptr<ClassFile> classfile);
};
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
}