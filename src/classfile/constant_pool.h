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
  kConstantUtf8 = 1,
  kConstantInteger = 3,
  kConstantFloat = 4,
  kConstantLong = 5,
  kConstantDouble = 6,
  kConstantClass = 7,
  kConstantString = 8,
  kConstantFieldRef = 9,
  kConstantMethodRef = 10,
  kConstantInterfaceMethodRef = 11,
  kConstantNameAndType = 12,
  kConstantMethodHandle = 15,
  kConstantMethodType = 16,
  kConstantDynamic = 17,
  kConstantInvokeDynamic = 18,
  kConstantModule = 19,
  kConstantPackage = 20,
};
class ConstantInfo {
public:
  explicit ConstantInfo(u1 tag) : tag_(tag) {}
  u1 GetTag() const {
    return tag_;
  }
  virtual ~ConstantInfo() = default;
  virtual void ParseConstantInfo(std::shared_ptr<ClassData> class_data, int& pos) {
    LOG(FATAL) << "UnsupportedOperation parse constant info in base class";
  }
private:
  u1 tag_;
};
struct ConstantUtf8Info : public ConstantInfo {
  string value_;
  ConstantUtf8Info() : ConstantInfo(kConstantUtf8) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    u2 length = 0;
    ParseUnsignedInt(classData, pos, length);
    u1* tmp = ParseBytes(classData, pos, length);
    //value = decodeMUTF8(tmp, length);
    value_ = std::string((char*)tmp, length);
  }
 
};
struct ConstantIntegerInfo : public ConstantInfo {
  int32_t value_;
  ConstantIntegerInfo() : ConstantInfo(kConstantInteger) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    u4 value = 0;
    ParseUnsignedInt(classData, pos, value);
    value_ = value;
  }
};
struct ConstantFloatInfo : public ConstantInfo {
  float value;
  ConstantFloatInfo() : ConstantInfo(kConstantFloat) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    u4 value = 0;
    ParseUnsignedInt(classData, pos, value);
    memcpy(&value, &value, sizeof(value));
  }
};
struct ConstantLongInfo : public ConstantInfo {
  int64_t value_;
  ConstantLongInfo() : ConstantInfo(kConstantLong) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    u8 value = 0;
    ParseUnsignedInt(classData, pos, value);
    //memcpy(&value, value, sizeof(value))
    value_ = value;
  }
};
struct ConstantDoubleInfo : public ConstantInfo {
  double value_;
  ConstantDoubleInfo() : ConstantInfo(kConstantDouble) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    u8 _value = 0;
    ParseUnsignedInt(classData, pos, _value);
    memcpy(&value_, &_value, sizeof(value_));
  }
};
struct ConstantClassInfo : public ConstantInfo {
  u2 name_index_;
  ConstantClassInfo() : ConstantInfo(kConstantClass){
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUnsignedInt(classData, pos, name_index_);
  }
};
struct ConstantStringInfo : public ConstantInfo {
  u2 string_index_;
  ConstantStringInfo() : ConstantInfo(kConstantString) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUnsignedInt(classData, pos, string_index_);
  }
};
struct ConstantMemberrefInfo : public ConstantInfo {
  u2 class_index_;
  u2 name_and_type_index_;
  ConstantMemberrefInfo(u1 tag) : ConstantInfo(tag) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUnsignedInt(classData, pos, class_index_);
    ParseUnsignedInt(classData, pos, name_and_type_index_);
  }
};
struct ConstantFieldrefInfo : public ConstantMemberrefInfo {
  ConstantFieldrefInfo() : ConstantMemberrefInfo(kConstantFieldRef) {
  }
};
struct ConstantMethodrefInfo : public ConstantMemberrefInfo {
  ConstantMethodrefInfo() : ConstantMemberrefInfo(kConstantMethodRef) {
  }
};
struct ConstantInterfaceMethodrefInfo : public ConstantMemberrefInfo {
  ConstantInterfaceMethodrefInfo() : ConstantMemberrefInfo(kConstantInterfaceMethodRef) {
  }
};
struct ConstantNameAndTypeInfo : public ConstantInfo {
  u2 name_index_;
  u2 descriptor_index_;
  ConstantNameAndTypeInfo() : ConstantInfo(kConstantNameAndType) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUnsignedInt(classData, pos, name_index_);
    ParseUnsignedInt(classData, pos, descriptor_index_);
  }
};
struct ConstantMethodHandleInfo : public ConstantInfo {
  u1 reference_kind_;
  u2 reference_index_;
  ConstantMethodHandleInfo() : ConstantInfo(kConstantMethodHandle) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUnsignedInt(classData, pos, reference_kind_);
    ParseUnsignedInt(classData, pos, reference_index_);
  }
};
struct ConstantMethodTypeInfo : public ConstantInfo {
  u2 descriptor_index_;
  ConstantMethodTypeInfo() : ConstantInfo(kConstantMethodType) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUnsignedInt(classData, pos, descriptor_index_);
  }
};
struct ConstantDynamicInfo : public ConstantInfo {
  u2 bootstrap_method_attr_index_;
  u2 name_and_type_index_;
  ConstantDynamicInfo() : ConstantInfo(kConstantDynamic){
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUnsignedInt(classData, pos, bootstrap_method_attr_index_);
    ParseUnsignedInt(classData, pos, name_and_type_index_);
  }
};
struct ConstantInvokeDynamicInfo : public ConstantInfo {
  u2 bootstrap_method_attr_index_;
  u2 name_and_type_index_;
  ConstantInvokeDynamicInfo() : ConstantInfo(kConstantInvokeDynamic) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUnsignedInt(classData, pos, bootstrap_method_attr_index_);
    ParseUnsignedInt(classData, pos, name_and_type_index_);
  }
};
struct ConstantModuleInfo : public ConstantInfo {
  u2 name_index_;
  ConstantModuleInfo() : ConstantInfo(kConstantModule) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUnsignedInt(classData, pos, name_index_);
  }
};
struct ConstantPackageInfo : public ConstantInfo {
  u2 name_index_;
  ConstantPackageInfo() : ConstantInfo(kConstantPackage) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUnsignedInt(classData, pos, name_index_);
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
    return utf8Info->value_;
  }
};

}
