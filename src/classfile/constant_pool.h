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
class ConstantUtf8Info : public ConstantInfo {
public:
  ConstantUtf8Info() : ConstantInfo(kConstantUtf8) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    u2 length = 0;
    ParseUnsignedInt(classData, pos, length);
    u1* tmp = ParseBytes(classData, pos, length);
    //value = DecodeMutf8(tmp, length);
    value_ = std::string((char*)tmp, length);
  }
  const string& GetValue() const {
    return value_;
  }
private:
  string value_;
};
class ConstantIntegerInfo : public ConstantInfo {
public:
  ConstantIntegerInfo() : ConstantInfo(kConstantInteger), value_(0) {}
  int32_t GetValue() const {
    return value_;
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    u4 value = 0;
    ParseUnsignedInt(classData, pos, value);
    value_ = value;
  }
private:
  int32_t value_;
};
class ConstantFloatInfo : public ConstantInfo {
public:
  ConstantFloatInfo() : ConstantInfo(kConstantFloat), value_(0.0f) {}
  float GetValue() const {
    return value_;
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    u4 value = 0;
    ParseUnsignedInt(classData, pos, value);
    memcpy(&value_, &value, sizeof(value));
  }
private:
  float value_;
};
class ConstantLongInfo : public ConstantInfo {
public:
  ConstantLongInfo() : ConstantInfo(kConstantLong), value_(0L) {}
  int64_t GetValue() const {
    return value_;
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> class_data, int& pos) override {
    u8 value = 0;
    ParseUnsignedInt(class_data, pos, value);
    //memcpy(&value, value, sizeof(value))
    value_ = value;
  }
private:
  int64_t value_;
};
class ConstantDoubleInfo : public ConstantInfo {
public:
  ConstantDoubleInfo() : ConstantInfo(kConstantDouble), value_(0.0) {}
  double GetValue() const {
    return value_;
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> class_data, int& pos) override {
    u8 value = 0;
    ParseUnsignedInt(class_data, pos, value);
    memcpy(&value_, &value, sizeof(value_));
  }
private:
  double value_;
};
class ConstantClassInfo : public ConstantInfo {
public:
  ConstantClassInfo() : ConstantInfo(kConstantClass), name_index_(0) {}
  u2 GetNameIndex() const {
    return name_index_;
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> class_data, int& pos) override {
    ParseUnsignedInt(class_data, pos, name_index_);
  }
private:
  u2 name_index_;
};
class ConstantStringInfo : public ConstantInfo {
public:
  ConstantStringInfo() : ConstantInfo(kConstantString), string_index_(0) {}
  u2 GetStringIndex() const {
    return string_index_;
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> class_data, int& pos) override {
    ParseUnsignedInt(class_data, pos, string_index_);
  }
private:
  u2 string_index_;
};
class ConstantMemberRefInfo : public ConstantInfo {
public:
  explicit ConstantMemberRefInfo(u1 tag) : ConstantInfo(tag),
                                           class_index_(0),
                                           name_and_type_index_(0) {}
  u2 GetClassIndex() const {
    return class_index_;
  }
  u2 GetNameAndTypeIndex() const {
    return name_and_type_index_;
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> class_data, int& pos) override {
    ParseUnsignedInt(class_data, pos, class_index_);
    ParseUnsignedInt(class_data, pos, name_and_type_index_);
  }
private:
  u2 class_index_;
  u2 name_and_type_index_;
};
class ConstantFieldRefInfo : public ConstantMemberRefInfo {
public:
  ConstantFieldRefInfo() : ConstantMemberRefInfo(kConstantFieldRef) {}
};
class ConstantMethodRefInfo : public ConstantMemberRefInfo {
public:
  ConstantMethodRefInfo() : ConstantMemberRefInfo(kConstantMethodRef) {
  }
};
class ConstantInterfaceMethodRefInfo : public ConstantMemberRefInfo {
public:
  ConstantInterfaceMethodRefInfo() : ConstantMemberRefInfo(kConstantInterfaceMethodRef) {
  }
};
class ConstantNameAndTypeInfo : public ConstantInfo {
public:
  ConstantNameAndTypeInfo() : ConstantInfo(kConstantNameAndType),
                              name_index_(0),
                              descriptor_index_(0) {}
  u2 GetNameIndex() const {
    return name_index_;
  }
  u2 GetDescriptorIndex() const {
    return descriptor_index_;
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> class_data, int& pos) override {
    ParseUnsignedInt(class_data, pos, name_index_);
    ParseUnsignedInt(class_data, pos, descriptor_index_);
  }
private:
  u2 name_index_;
  u2 descriptor_index_;
};
class ConstantMethodHandleInfo : public ConstantInfo {
public:
  ConstantMethodHandleInfo() : ConstantInfo(kConstantMethodHandle),
                               reference_kind_(0),
                               reference_index_(0) {}
  void ParseConstantInfo(std::shared_ptr<ClassData> class_data, int& pos) override {
    ParseUnsignedInt(class_data, pos, reference_kind_);
    ParseUnsignedInt(class_data, pos, reference_index_);
  }
private:
  u1 reference_kind_;
  u2 reference_index_;
};
class ConstantMethodTypeInfo : public ConstantInfo {
public:
  ConstantMethodTypeInfo() : ConstantInfo(kConstantMethodType), descriptor_index_(0) {}
  void ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    ParseUnsignedInt(classData, pos, descriptor_index_);
  }
private:
  u2 descriptor_index_;
};
class ConstantDynamicInfo : public ConstantInfo {
public:
  ConstantDynamicInfo() : ConstantInfo(kConstantDynamic),
                          bootstrap_method_attr_index_(0),
                          name_and_type_index_(0) {}
  void ParseConstantInfo(std::shared_ptr<ClassData> class_data, int& pos) override {
    ParseUnsignedInt(class_data, pos, bootstrap_method_attr_index_);
    ParseUnsignedInt(class_data, pos, name_and_type_index_);
  }
private:
  u2 bootstrap_method_attr_index_;
  u2 name_and_type_index_;
};
class ConstantInvokeDynamicInfo : public ConstantInfo {
public:
  ConstantInvokeDynamicInfo() : ConstantInfo(kConstantInvokeDynamic),
                                bootstrap_method_attr_index_(0),
                                name_and_type_index_(0){}
  void ParseConstantInfo(std::shared_ptr<ClassData> class_data, int& pos) override {
    ParseUnsignedInt(class_data, pos, bootstrap_method_attr_index_);
    ParseUnsignedInt(class_data, pos, name_and_type_index_);
  }
private:
  u2 bootstrap_method_attr_index_;
  u2 name_and_type_index_;
};
class ConstantModuleInfo : public ConstantInfo {
public:
  ConstantModuleInfo() : ConstantInfo(kConstantModule), name_index_(0) {}
  void ParseConstantInfo(std::shared_ptr<ClassData> class_data, int& pos) override {
    ParseUnsignedInt(class_data, pos, name_index_);
  }
private:
  u2 name_index_;
};
class ConstantPackageInfo : public ConstantInfo {
public:
  ConstantPackageInfo() : ConstantInfo(kConstantPackage), name_index_(0) {
  }
  void ParseConstantInfo(std::shared_ptr<ClassData> class_data, int& pos) override {
    ParseUnsignedInt(class_data, pos, name_index_);
  }
private:
  u2 name_index_;
};
class ConstantPool {
public:
  explicit ConstantPool(u2 constant_pool_count) : constant_pool_count_(constant_pool_count){}
  ~ConstantPool() = default;
  void PutConstantInfo(std::shared_ptr<ConstantInfo> constant_info) {
    constant_infos_.push_back(constant_info);
  }
  std::shared_ptr<ConstantInfo> GetConstantInfo(u2 index) {
    if (index < 1 || index >= constant_pool_count_) {
      LOG(FATAL) << "Invalid constant pool index";
      return nullptr;
    }
    return constant_infos_[index];
  }
  u2 GetConstantCount() const {
    return constant_pool_count_;
  }
  void GetNameAndType(u2 index, string& name, string& type) {
    std::shared_ptr<ConstantNameAndTypeInfo> natInfo = std::dynamic_pointer_cast<ConstantNameAndTypeInfo>(
        GetConstantInfo(index));
    name = GetUtf8(natInfo->GetNameIndex());
    type = GetUtf8(natInfo->GetDescriptorIndex());
  }
  string GetClassName(u2 index) {
    std::shared_ptr<ConstantClassInfo> classInfo = std::dynamic_pointer_cast<ConstantClassInfo>(GetConstantInfo(index));
    return GetUtf8(classInfo->GetNameIndex());
  }
  string GetUtf8(u2 index) {
    std::shared_ptr<ConstantUtf8Info> utf8Info = std::dynamic_pointer_cast<ConstantUtf8Info>(GetConstantInfo(index));
    return utf8Info->GetValue();
  }
private:
  u2 constant_pool_count_;
  std::vector<std::shared_ptr<ConstantInfo>> constant_infos_;
};

}
