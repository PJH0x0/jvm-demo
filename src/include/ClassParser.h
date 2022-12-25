#pragma once
#include "ClassReader.h"
#include "ClassDataType.h"
#include <cstdint>
#include <linux/limits.h>
#include <memory>
#include <vector>
#include <cstring>


namespace JVM {
class ConstantInfo;
class ConstantPool;
class MemberInfo;
class AttributeInfo;
class ClassFile {
  public:
  u4 magic;
  u2 minorVersion;
  u2 majorVersion;
  std::shared_ptr<ConstantPool> constantPool;
  u2 accessFlags;
  u2 thisClass;
  u2 superClass;
  std::vector<u2> interfaces;
  std::vector<std::shared_ptr<MemberInfo>> fields;
  std::vector<std::shared_ptr<MemberInfo>> methods;
  std::vector<std::shared_ptr<AttributeInfo>> attributes;
};
template<typename T>
void parseUint(std::shared_ptr<ClassData> data, int& pos, T& t) {
  int size = sizeof(t);
  t = 0;
  for (int i = 0; i < size; i++) {
    t |= data->data[pos+i] << (size - 1 - i) * 8;
  }
  pos += size;
}
template<typename T>
u1* parseBytes(std::shared_ptr<ClassData> data, int& pos, T length) {
  if (length < 0) return nullptr;
  u1* tmp = (u1*)malloc(length+1);
  memset(tmp, 0, length+1);
  for (T i = 0; i < length; i++) {
    tmp[i] = data->data[pos + i];
  }
  pos += length;
  return tmp;
}
std::shared_ptr<ClassFile> parse(std::shared_ptr<ClassData> data);
void parseAndCheckMagic(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
void parseAndCheckVersion(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);

void parseConstantPool(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
std::shared_ptr<ConstantInfo> createConstantInfo(u1 tag);
std::shared_ptr<ConstantInfo> parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos);

void parseAccessFlags(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
void parseThisClass(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
void parseSuperClass(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
void parseInterfaces(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);

void parseMembers(std::shared_ptr<ClassData> data, std::vector<std::shared_ptr<MemberInfo>>& memberInfos, 
                  std::shared_ptr<ConstantPool> cp, int& pos);
std::shared_ptr<MemberInfo> parseMember(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp, int& pos);
void parseFieldInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
void parseMethodInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);

std::shared_ptr<AttributeInfo> createAttributeInfo(const string& attrName, u4 attrLen, std::shared_ptr<ConstantPool> cp);
std::shared_ptr<AttributeInfo> parseAttributeInfo(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp, int& pos);
void parseAttributeInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int &pos);
void parseAttributeInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp,
                         std::vector<std::shared_ptr<AttributeInfo>>& attributes, int& pos);
void endianSwap(uint8_t* data, int size);

}