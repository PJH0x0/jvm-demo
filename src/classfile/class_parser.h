#pragma once
#include <classpath/class_reader.h>
#include "class_data_t.h"
#include <cstdint>
#include <string>
#ifdef linux
#include <linux/limits.h>
#elif __APPLE__
#include <limits.h>
#endif
#include <memory>
#include <vector>
#include <cstring>


namespace classfile {
class ConstantInfo;
class ConstantPool;
class MemberInfo;
class AttributeInfo;
using classpath::ClassData;
class ClassFile {
public:
  u4 magic_;
  u2 minor_version_;
  u2 major_version_;
  std::shared_ptr<ConstantPool> constant_pool_;
  u2 access_flags_;
  u2 this_class_;
  u2 super_class_;
  std::vector<u2> interfaces_;
  std::vector<std::shared_ptr<MemberInfo>> fields_;
  std::vector<std::shared_ptr<MemberInfo>> methods_;
  std::vector<std::shared_ptr<AttributeInfo>> attributes_;
  std::string GetClassName();
  std::string GetSuperClassName();
  void GetInterfaceNames(std::vector<std::string>& interface_names);
  std::string GetSourceFile();

};
template<typename T>
void ParseUint(std::shared_ptr<ClassData> data, int& pos, T& t) {
  int size = sizeof(t);
  t = 0;
  for (int i = 0; i < size; i++) {
    t |= data->data_[pos + i] << (size - 1 - i) * 8;
  }
  pos += size;
}
template<typename T>
u1* ParseBytes(std::shared_ptr<ClassData> data, int& pos, T length) {
  if (length < 0) return nullptr;
  u1* tmp = (u1*)malloc(length+1);
  memset(tmp, 0, length+1);
  for (T i = 0; i < length; i++) {
    tmp[i] = data->data_[pos + i];
  }
  pos += length;
  return tmp;
}
std::shared_ptr<ClassFile> Parse(std::shared_ptr<ClassData> data);
void ParseAndCheckMagic(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
void ParseAndCheckVersion(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);

void ParseConstantPool(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
std::shared_ptr<ConstantInfo> CreateConstantInfo(u1 tag);
std::shared_ptr<ConstantInfo> ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos);

void ParseAccessFlags(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
void ParseThisClass(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
void ParseSuperClass(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
void ParseInterfaces(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);

void ParseMembers(std::shared_ptr<ClassData> data, std::vector<std::shared_ptr<MemberInfo>>& member_infos,
                  std::shared_ptr<ConstantPool> cp, int& pos);
std::shared_ptr<MemberInfo> ParseMember(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp, int& pos);
void ParseFieldInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
void ParseMethodInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);

std::shared_ptr<AttributeInfo> CreateAttributeInfo(const string& attrName, u4 attrLen, std::shared_ptr<ConstantPool> cp);
std::shared_ptr<AttributeInfo> ParseAttributeInfo(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp, int& pos);
void ParseAttributeInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int &pos);
void ParseAttributeInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp,
                         std::vector<std::shared_ptr<AttributeInfo>>& attributes, int& pos);
void EndianSwap(uint8_t* data, int size);

}