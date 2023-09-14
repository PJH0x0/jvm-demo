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
  explicit ClassFile(std::shared_ptr<ClassData> data) : data_(data), pos_(0), magic_(0), minor_version_(0),
                                                        major_version_(0), access_flags_(0), this_class_(0), super_class_(0) {}
  u4 GetMagic() const;

  u2 GetMinorVersion() const;

  u2 GetMajorVersion() const;

  const std::shared_ptr<ConstantPool>& GetConstantPool() const;

  u2 GetAccessFlags() const;

  u2 GetThisClass() const;

  u2 GetSuperClass() const;

  const std::vector<u2>& GetInterfaces() const;

  const std::vector<std::shared_ptr<MemberInfo>>& GetFields() const;

  const std::vector<std::shared_ptr<MemberInfo>>& GetMethods() const;

  const std::vector<std::shared_ptr<AttributeInfo>>& GetAttributes() const;

  std::string GetClassName();
  std::string GetSuperClassName();
  void GetInterfaceNames(std::vector<std::string>& interface_names);
  std::string GetSourceFile();
  void ParseAndCheckMagic();
  void ParseAndCheckVersion();

  void ParseConstantPool();
  void ParseAccessFlags();
  void ParseThisClass();
  void ParseSuperClass();
  void ParseInterfaces();
  void ParseFieldInfos();
  void ParseMethodInfos();
  void ParseAttributeInfos();

private:
  int32_t pos_;
  std::shared_ptr<ClassData> data_;
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


};
template<typename T>
void ParseUnsignedInt(std::shared_ptr<ClassData> data, int& pos, T& t) {
  int size = sizeof(t);
  t = 0;
  for (int i = 0; i < size; i++) {
    t |= data->GetData()[pos + i] << (size - 1 - i) * 8;
  }
  pos += size;
}
template<typename T>
u1* ParseBytes(std::shared_ptr<ClassData> data, int& pos, T length) {
  if (length < 0) return nullptr;
  u1* tmp = (u1*)malloc(length+1);
  memset(tmp, 0, length+1);
  for (T i = 0; i < length; i++) {
    tmp[i] = data->GetData()[pos + i];
  }
  pos += length;
  return tmp;
}
std::shared_ptr<ClassFile> Parse(std::shared_ptr<ClassData> data);

std::shared_ptr<ConstantInfo> CreateConstantInfo(u1 tag);
std::shared_ptr<ConstantInfo> ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos);

void ParseMembers(std::shared_ptr<ClassData> data, std::vector<std::shared_ptr<MemberInfo>>& member_infos,
                  std::shared_ptr<ConstantPool> cp, int& pos);
std::shared_ptr<MemberInfo> ParseMember(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp, int& pos);


std::shared_ptr<AttributeInfo> CreateAttributeInfo(const string& attr_name, u4 attr_len, std::shared_ptr<ConstantPool> cp);
std::shared_ptr<AttributeInfo> ParseAttributeInfo(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp, int& pos);

void EndianSwap(uint8_t* data, int size);

}