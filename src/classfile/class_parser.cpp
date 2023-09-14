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


void ParseAndCheckMagic(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  u4 target_magic = 0xCAFEBABE;//little endian
  ParseUnsignedInt(data, pos, file->magic_);
  LOG_IF(FATAL, file->magic_ != target_magic) << "Magic number wrong";
}
void ParseAndCheckVersion(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  ParseUnsignedInt(data, pos, file->minor_version_);
  ParseUnsignedInt(data, pos, file->major_version_);
  switch (file->major_version_) {
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
void ParseConstantPool(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  std::shared_ptr<ConstantPool> constant_pool = std::make_shared<ConstantPool>();
  ParseUnsignedInt(data, pos, constant_pool->constant_pool_count_);
  constant_pool->constant_infos_.push_back(nullptr);
  for (u2 i = 1; i < constant_pool->constant_pool_count_; i++) {
    std::shared_ptr<ConstantInfo> constantInfo = ParseConstantInfo(data, pos);
    constant_pool->constant_infos_.push_back(constantInfo);
    switch ((int32_t)constantInfo->tag_) {
      case kConstantDouble:
      case kConstantLong:
        i++;
        constant_pool->constant_infos_.push_back(nullptr);
        break;
    }
  }
  file->constant_pool_ = constant_pool;
}
void ParseAccessFlags(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  ParseUnsignedInt(data, pos, file->access_flags_);
}
void ParseThisClass(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  ParseUnsignedInt(data, pos, file->this_class_);
}
void ParseSuperClass(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  ParseUnsignedInt(data, pos, file->super_class_);
}
void ParseInterfaces(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  u2 count = 0;
  ParseUnsignedInt(data, pos, count);
  //LOG(INFO) << "Interface count = " << static_cast<int>(count);
  u2 interface_index = 0;
  for (u2 i = 0; i < count; i++) {
    ParseUnsignedInt(data, pos, interface_index);
    file->interfaces_.push_back(interface_index);
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
    ParseAttributeInfos(data, cp, member_info->attributes_, pos);
  return member_info;
}
void ParseFieldInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
    ParseMembers(data, file->fields_, file->constant_pool_, pos);
}
void ParseMethodInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
    ParseMembers(data, file->methods_, file->constant_pool_, pos);
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
void ParseAttributeInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int &pos) {
    ParseAttributeInfos(data, file->constant_pool_, file->attributes_, pos);
}
void ParseAttributeInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp, std::vector<std::shared_ptr<AttributeInfo>>& attributes, int& pos) {
  u2 attribute_count = 0;
  ParseUnsignedInt(data, pos, attribute_count);
  for (u2 i = 0; i < attribute_count; i++) {
    attributes.push_back(ParseAttributeInfo(data, cp, pos));
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
  std::shared_ptr<ClassFile> class_file = std::make_shared<ClassFile>();
  int pos = 0;
  ParseAndCheckMagic(data, class_file, pos);
  ParseAndCheckVersion(data, class_file, pos);
  ParseConstantPool(data, class_file, pos);
  ParseAccessFlags(data, class_file, pos);
  ParseThisClass(data, class_file, pos);
  ParseSuperClass(data, class_file, pos);
  ParseInterfaces(data, class_file, pos);
  ParseFieldInfos(data, class_file, pos);
  ParseMethodInfos(data, class_file, pos);
  ParseAttributeInfos(data, class_file, pos);
  return class_file;
}
}

