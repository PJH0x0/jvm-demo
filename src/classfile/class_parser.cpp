#include "class_parser.h"
#include "constant_pool.h"
#include "member_info.h"
#include "attr_info.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <glog/logging.h>
#include <ios>
#include <memory>
#include <string>
#include <stdint.h>
#include <vector>


namespace classfile {

std::string ClassFile::GetClassName(){
  return constant_pool_->GetClassName(this_class_);
}

std::string ClassFile::GetSuperClassName() {
  if (super_class_ > 0) {
    return constant_pool_->GetClassName(super_class_);
  }
  return {};
}

void ClassFile::GetInterfaceNames(std::vector<std::string>& interface_names) {
  for (auto index : interfaces_) {
    interface_names.push_back(constant_pool_->GetClassName(index));
  }
}

std::string ClassFile::GetSourceFile() {
  for (auto attr : attributes_) {
    std::shared_ptr<SourceFileAttributeInfo> source_file_attr = std::dynamic_pointer_cast<SourceFileAttributeInfo>(attr);
    if (source_file_attr != nullptr) {
      return constant_pool_->GetUtf8(source_file_attr->source_file_index_);
    }
  }
  return {};
}

u4 ClassFile::GetMagic() const {
  return magic_;
}

u2 ClassFile::GetMinorVersion() const {
  return minor_version_;
}

u2 ClassFile::GetMajorVersion() const {
  return major_version_;
}

const std::shared_ptr<ConstantPool>& ClassFile::GetConstantPool() const {
  return constant_pool_;
}

u2 ClassFile::GetAccessFlags() const {
  return access_flags_;
}

u2 ClassFile::GetThisClass() const {
  return this_class_;
}

u2 ClassFile::GetSuperClass() const {
  return super_class_;
}

const std::vector<u2>& ClassFile::GetInterfaces() const {
  return interfaces_;
}

const std::vector<std::shared_ptr<MemberInfo>>& ClassFile::GetFields() const {
  return fields_;
}

const std::vector<std::shared_ptr<MemberInfo>>& ClassFile::GetMethods() const {
  return methods_;
}

const std::vector<std::shared_ptr<AttributeInfo>>& ClassFile::GetAttributes() const {
  return attributes_;
}


void ClassFile::ParseAndCheckMagic() {
  u4 target_magic = 0xCAFEBABE;//little endian
  ParseUnsignedInt(data_, pos_, magic_);
  LOG_IF(FATAL, magic_ != target_magic) << "Magic number wrong";
}
void ClassFile::ParseAndCheckVersion() {
  ParseUnsignedInt(data_, pos_, minor_version_);
  ParseUnsignedInt(data_, pos_, major_version_);
  switch (major_version_) {
    case 45:
      return;
    case 46:
    case 47:
    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
    if (file->minor_version_ == 0) {
      return;
    }
  }
  //return pos_;
  LOG(FATAL) << "java.lang.UnsupportedClassVersionError major version " << file->major_version_ << " minor version " << file->minor_version_;
}
std::shared_ptr<ConstantInfo> CreateConstantInfo(u1 tag) {
  switch (tag) {
    case kConstantUtf8: return std::make_shared<ConstantUtf8Info>();
    case kConstantInteger: return std::make_shared<ConstantIntegerInfo>();
    case kConstantFloat: return std::make_shared<ConstantFloatInfo>();
    case kConstantLong: return std::make_shared<ConstantLongInfo>();
    case kConstantDouble: return std::make_shared<ConstantDoubleInfo>();
    case kConstantClass: return std::make_shared<ConstantClassInfo>();
    case kConstantString: return std::make_shared<ConstantStringInfo>();
    case kConstantFieldRef: return std::make_shared<ConstantFieldrefInfo>();
    case kConstantMethodRef: return std::make_shared<ConstantMethodrefInfo>();
    case kConstantInterfaceMethodRef: return std::make_shared<ConstantInterfaceMethodrefInfo>();
    case kConstantNameAndType: return std::make_shared<ConstantNameAndTypeInfo>();
    case kConstantMethodHandle: return std::make_shared<ConstantMethodHandleInfo>();
    case kConstantMethodType: return std::make_shared<ConstantMethodTypeInfo>();
    case kConstantDynamic: return std::make_shared<ConstantDynamicInfo>();
    case kConstantInvokeDynamic: return std::make_shared<ConstantInvokeDynamicInfo>();
    case kConstantModule: return std::make_shared<ConstantModuleInfo>();
    case kConstantPackage: return std::make_shared<ConstantPackageInfo>();
    default: break;
  }
  LOG(FATAL) << "java.lang.ClassFormatError: constant pool tag "<< tag;
}
std::shared_ptr<ConstantInfo> ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
  u1 tag = 0;
  ParseUnsignedInt(classData, pos, tag);
  //LOG(INFO) << "Constant info tag = " << (int)tag;
  std::shared_ptr<ConstantInfo> constant_info = CreateConstantInfo(tag);
  //constant_info->tag_ = tag;
  constant_info->ParseConstantInfo(classData, pos);
  return constant_info;
}
void ClassFile::ParseConstantPool() {
  std::shared_ptr<ConstantPool> constant_pool = std::make_shared<ConstantPool>();
  ParseUnsignedInt(data_, pos_, constant_pool->constant_pool_count_);
  constant_pool->constant_infos_.push_back(nullptr);
  for (u2 i = 1; i < constant_pool->constant_pool_count_; i++) {
    std::shared_ptr<ConstantInfo> constantInfo = ParseConstantInfo(data_, pos_);
    constant_pool->constant_infos_.push_back(constantInfo);
    switch ((int32_t)constantInfo->tag_) {
      case kConstantDouble:
      case kConstantLong:
        i++;
        constant_pool->constant_infos_.push_back(nullptr);
        break;
    }
  }
  constant_pool_ = constant_pool;
}
void ClassFile::ParseAccessFlags() {
  ParseUnsignedInt(data_, pos_, access_flags_);
}
void ClassFile::ParseThisClass() {
  ParseUnsignedInt(data_, pos_, this_class_);
}
void ClassFile::ParseSuperClass() {
  ParseUnsignedInt(data_, pos_, super_class_);
}
void ClassFile::ParseInterfaces() {
  u2 count = 0;
  ParseUnsignedInt(data_, pos_, count);
  //LOG(INFO) << "Interface count = " << static_cast<int>(count);
  u2 interface_index = 0;
  for (u2 i = 0; i < count; i++) {
    ParseUnsignedInt(data_, pos_, interface_index);
    interfaces_.push_back(interface_index);
  }
}

void ParseMembers(std::shared_ptr<ClassData> data, std::vector<std::shared_ptr<MemberInfo>>& member_infos,
                  std::shared_ptr<ConstantPool> cp, int& pos) {
  u2 count = 0;
  ParseUnsignedInt(data, pos, count);
  //LOG(INFO) << "Member counts = " << count;
  for (u2 i = 0; i < count; i++) {
    member_infos.push_back(ParseMember(data, cp, pos));
  }
}

std::shared_ptr<MemberInfo> ParseMember(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp, int& pos) {
  std::shared_ptr<MemberInfo> member_info = std::make_shared<MemberInfo>(cp);
  ParseUnsignedInt(data, pos, member_info->access_flags_);
  ParseUnsignedInt(data, pos, member_info->name_index_);
  //LOG(INFO) << "name_index_ = " << member_info->name_index_ << " name = " << cp->GetUtf8(member_info->name_index_);
  ParseUnsignedInt(data, pos, member_info->descriptor_index_);
  //ParseAttributeInfos(data, cp, member_info->attributes_, pos);
  u2 attribute_count = 0;
  ParseUnsignedInt(data, pos, attribute_count);
  for (u2 i = 0; i < attribute_count; i++) {
    member_info->attributes_.push_back(ParseAttributeInfo(data, cp, pos));
  }
  return member_info;
}
void ClassFile::ParseFieldInfos() {
    ParseMembers(data_, fields_, constant_pool_, pos_);
}
void ClassFile::ParseMethodInfos() {
    ParseMembers(data_, methods_, constant_pool_, pos_);
}


std::shared_ptr<AttributeInfo> CreateAttributeInfo(const string& attr_name, u4 attr_len, std::shared_ptr<ConstantPool> cp) {
  std::shared_ptr<AttributeInfo> ptr;
  if (attr_name == "Code") {
    return std::make_shared<CodeAttributeInfo>(cp);
  } else if (attr_name == "ConstantValue") {
    return std::make_shared<ConstantValueAttributeInfo>();
  } else if (attr_name == "DepreCated") {
    return  std::make_shared<DeprecatedAttributeInfo>();
  } else if (attr_name == "Exceptions") {
    return std::make_shared<ExceptionsAttributeInfo>();
  } else if (attr_name == "LineNumberTable") {
    return std::make_shared<LineNumberTableAttributeInfo>();
  } else if (attr_name == "LocalVariableTable") {
    return std::make_shared<LocalVariableTableAttributeInfo>();
  } else if (attr_name == "SourceFile") {
    return std::make_shared<SourceFileAttributeInfo>();
  } else if (attr_name == "Synthetic") {
    return std::make_shared<SyntheticAttributeInfo>();
  } else {
    return std::make_shared<UnparsedAttributeInfo>(attr_name, attr_len);
  }
}
std::shared_ptr<AttributeInfo> ParseAttributeInfo(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp, int& pos) {
  u2 attr_name_index = 0;
  ParseUnsignedInt(data, pos, attr_name_index);
  string attr_name = cp->GetUtf8(attr_name_index);
  u4 attr_len = 0;
  ParseUnsignedInt(data, pos, attr_len);
  std::shared_ptr<AttributeInfo> attr_info = CreateAttributeInfo(attr_name, attr_len, cp);
    attr_info->ParseAttrInfo(data, pos);
  return attr_info;
}

void ClassFile::ParseAttributeInfos() {
  u2 attribute_count = 0;
  ParseUnsignedInt(data_, pos_, attribute_count);
  for (u2 i = 0; i < attribute_count; i++) {
    attributes_.push_back(ParseAttributeInfo(data, cp, pos));
  }
}

void EndianSwap(uint8_t* data, int size) {
  int start = 0;
  int end = size - 1;
  uint8_t tmp = 0;
  while (start < end) {
    tmp = data[start];
    data[start] = data[end];
    data[end] = tmp;
    start++;
    end--;
  }
}
std::shared_ptr<ClassFile> Parse(std::shared_ptr<ClassData> data) {
  std::shared_ptr<ClassFile> class_file = std::make_shared<ClassFile>(data);
  int pos = 0;
  class_file->ParseAndCheckMagic();
  class_file->ParseAndCheckVersion();
  class_file->ParseConstantPool();
  class_file->ParseAccessFlags();
  class_file->ParseThisClass();
  class_file->ParseSuperClass();
  class_file->ParseInterfaces();
  class_file->ParseFieldInfos();
  class_file->ParseMethodInfos();
  class_file->ParseAttributeInfos();
  return class_file;
}
}

