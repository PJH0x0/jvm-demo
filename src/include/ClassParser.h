#pragma once
#include <cstdint>
#include <linux/limits.h>
#include <memory>
#include <vector>
#include "ClassReader.h"
//#include "ConstantPool.h"
#include "ClassDataType.h"

namespace JVM {
class ConstantInfo;
struct ConstantPool {
  u2 constantPoolCount;
  std::vector<std::shared_ptr<ConstantInfo>> constantInfos;
  ConstantPool() : constantPoolCount(0){}
  ~ConstantPool() {constantPoolCount = 0;}
  std::shared_ptr<ConstantInfo> getConstantInfo(u2 index);
  void getNameAndType(u2 index, string& name, string& type);
  string getClassName(u2 index);
  string getUtf8(u2 index);
};
class FiledInfo;
class MethodInfo;
class AttributeInfo;
class ClassFile {
  public:
  u4 magic;
  u2 minorVersion;
  u2 majorVersion;
  // u2 constantPoolCount;
  // ConstantPool* constantPool;
  std::shared_ptr<ConstantPool> constantPool;
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
void parseUint(std::shared_ptr<ClassData> data, int& pos, T& t);
u1* parseBytes(std::shared_ptr<ClassData> data, int& pos, int length);
std::shared_ptr<ClassFile> parse(std::shared_ptr<ClassData> data);
void parseAndCheckMagic(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
void parseAndCheckVersion(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
void parseConstantPool(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
void parseAccessFlags(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
void endianSwap(uint8_t* data, int size);

}