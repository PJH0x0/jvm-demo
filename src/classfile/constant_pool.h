#pragma once
#include "class_data_t.h"
#include "class_parser.h"
#include <vector>
#include <string>
#include <memory>
#include <glog/logging.h>
namespace classfile {
using std::string;

enum {
  CONSTANT_Utf8 = 1,
  CONSTANT_Integer = 3,
  CONSTANT_Float = 4,
  CONSTANT_Long = 5,
  CONSTANT_Double = 6,
  CONSTANT_Class = 7,
  CONSTANT_String = 8,
  CONSTANT_Fieldref = 9,
  CONSTANT_Methodref = 10,
  CONSTANT_InterfaceMethodref = 11,
  CONSTANT_NameAndType = 12,
  CONSTANT_MethodHandle = 15,
  CONSTANT_MethodType = 16,
  CONSTANT_Dynamic = 17,
  CONSTANT_InvokeDynamic = 18,
  CONSTANT_Module = 19,
  CONSTANT_Package = 20,
};
struct ConstantInfo {
  u1 tag_;
  ConstantInfo(u1 tag) : tag_(tag) {}
  virtual void ParseConstantInfo(std::shared_ptr<ClassData> class_data, int& pos) {
    LOG(FATAL) << "UnsupportedOperation parse constant info in base class";
  }
};
struct ConstantUtf8Info : public ConstantInfo {
  string value;
  ConstantUtf8Info() : ConstantInfo(CONSTANT_Utf8) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    u2 length = 0;
      ParseUint(classData, pos, length);
    u1* tmp = ParseBytes(classData, pos, length);
    //value = decodeMUTF8(tmp, length);
    value = std::string((char*)tmp, length);
  }
 
};
struct ConstantIntegerInfo : public ConstantInfo {
  int32_t value_;
  ConstantIntegerInfo() : ConstantInfo(CONSTANT_Integer) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    u4 value = 0;
    ParseUint(classData, pos, value);
    value_ = value;
  }
};
struct ConstantFloatInfo : public ConstantInfo {
  float value;
  ConstantFloatInfo() : ConstantInfo(CONSTANT_Float) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    u4 value = 0;
    ParseUint(classData, pos, value);
    memcpy(&value, &value, sizeof(value));
  }
};
struct ConstantLongInfo : public ConstantInfo {
  int64_t value_;
  ConstantLongInfo() : ConstantInfo(CONSTANT_Long) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    u8 value = 0;
    ParseUint(classData, pos, value);
    //memcpy(&value, value, sizeof(value))
    value_ = value;
  }
};
struct ConstantDoubleInfo : public ConstantInfo {
  double value_;
  ConstantDoubleInfo() : ConstantInfo(CONSTANT_Double) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    u8 _value = 0;
    ParseUint(classData, pos, _value);
    memcpy(&value_, &_value, sizeof(value_));
  }
};
struct ConstantClassInfo : public ConstantInfo {
  u2 name_index_;
  ConstantClassInfo() : ConstantInfo(CONSTANT_Class){
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUint(classData, pos, name_index_);
  }
};
struct ConstantStringInfo : public ConstantInfo {
  u2 string_index_;
  ConstantStringInfo() : ConstantInfo(CONSTANT_String) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUint(classData, pos, string_index_);
  }
};
struct ConstantMemberrefInfo : public ConstantInfo {
  u2 class_index_;
  u2 name_and_type_index_;
  ConstantMemberrefInfo(u1 tag) : ConstantInfo(tag) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUint(classData, pos, class_index_);
    ParseUint(classData, pos, name_and_type_index_);
  }
};
struct ConstantFieldrefInfo : public ConstantMemberrefInfo {
  ConstantFieldrefInfo() : ConstantMemberrefInfo(CONSTANT_Fieldref) {
  }
};
struct ConstantMethodrefInfo : public ConstantMemberrefInfo {
  ConstantMethodrefInfo() : ConstantMemberrefInfo(CONSTANT_Methodref) {
  }
};
struct ConstantInterfaceMethodrefInfo : public ConstantMemberrefInfo {
  ConstantInterfaceMethodrefInfo() : ConstantMemberrefInfo(CONSTANT_InterfaceMethodref) {
  }
};
struct ConstantNameAndTypeInfo : public ConstantInfo {
  u2 name_index_;
  u2 descriptor_index_;
  ConstantNameAndTypeInfo() : ConstantInfo(CONSTANT_NameAndType) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUint(classData, pos, name_index_);
    ParseUint(classData, pos, descriptor_index_);
  }
};
struct ConstantMethodHandleInfo : public ConstantInfo {
  u1 reference_kind_;
  u2 reference_index_;
  ConstantMethodHandleInfo() : ConstantInfo(CONSTANT_MethodHandle) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUint(classData, pos, reference_kind_);
    ParseUint(classData, pos, reference_index_);
  }
};
struct ConstantMethodTypeInfo : public ConstantInfo {
  u2 descriptor_index_;
  ConstantMethodTypeInfo() : ConstantInfo(CONSTANT_MethodType) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUint(classData, pos, descriptor_index_);
  }
};
struct ConstantDynamicInfo : public ConstantInfo {
  u2 bootstrap_method_attr_index_;
  u2 name_and_type_index_;
  ConstantDynamicInfo() : ConstantInfo(CONSTANT_Dynamic){
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUint(classData, pos, bootstrap_method_attr_index_);
    ParseUint(classData, pos, name_and_type_index_);
  }
};
struct ConstantInvokeDynamicInfo : public ConstantInfo {
  u2 bootstrap_method_attr_index_;
  u2 name_and_type_index_;
  ConstantInvokeDynamicInfo() : ConstantInfo(CONSTANT_InvokeDynamic) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
      ParseUint(classData, pos, bootstrap_method_attr_index_);
      ParseUint(classData, pos, name_and_type_index_);
  }
};
struct ConstantModuleInfo : public ConstantInfo {
  u2 name_index_;
  ConstantModuleInfo() : ConstantInfo(CONSTANT_Module) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
      ParseUint(classData, pos, name_index_);
  }
};
struct ConstantPackageInfo : public ConstantInfo {
  u2 name_index_;
  ConstantPackageInfo() : ConstantInfo(CONSTANT_Package) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
      ParseUint(classData, pos, name_index_);
  }
};
struct ConstantPool {
  u2 constant_pool_count_;
  std::vector<std::shared_ptr<ConstantInfo>> constant_infos_;
  ConstantPool() : constant_pool_count_(0){}
  ~ConstantPool() { constant_pool_count_ = 0;}
  std::shared_ptr<ConstantInfo> GetConstantInfo(u2 index) {
    if (index < 1 || index >= constant_pool_count_) {
      LOG(FATAL) << "Invalid constant pool index";
      return nullptr;
    }
    return constant_infos_[index];
  }
  void GetNameAndType(u2 index, string& name, string& type) {
    std::shared_ptr<ConstantNameAndTypeInfo> natInfo = std::dynamic_pointer_cast<ConstantNameAndTypeInfo>(
        GetConstantInfo(index));
    name = GetUtf8(natInfo->name_index_);
    type = GetUtf8(natInfo->descriptor_index_);
  }
  string GetClassName(u2 index) {
    std::shared_ptr<ConstantClassInfo> classInfo = std::dynamic_pointer_cast<ConstantClassInfo>(GetConstantInfo(index));
    return GetUtf8(classInfo->name_index_);
  }
  string GetUtf8(u2 index) {
    std::shared_ptr<ConstantUtf8Info> utf8Info = std::dynamic_pointer_cast<ConstantUtf8Info>(GetConstantInfo(index));
    return utf8Info->value;
  }
};

}
