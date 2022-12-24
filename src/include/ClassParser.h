#pragma once
#include <cstdint>
#include <linux/limits.h>
#include <memory>
#include "ClassReader.h"

namespace JVM {

typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;
class ConstantPool;
class FiledInfo;
class MethodInfo;
class AttributeInfo;
class ClassFile {
  public:
  u4 magic;
  u2 minorVersion;
  u2 majorVersion;
  u2 constantPoolCount;
  ConstantPool* constantPool;
  u2 accessFlags;
  u2 thisClass;
  u2 superClass;
  u2 interfacesCount;
  u2* interfaces;
  u2 fieldsCount;
  FiledInfo* fields;
  u2 methodsCount;
  MethodInfo* methods;
  u2 attributesCount;
  AttributeInfo* attributes;
};
template<typename T>
int read(std::shared_ptr<ClassData> data, int pos, T& t);
std::shared_ptr<ClassFile> parse(std::shared_ptr<ClassData> data);
int parseAndCheckMagic(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int pos);
int parseAndCheckVersion(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int pos);
void endianSwap(uint8_t* data, int size);

}