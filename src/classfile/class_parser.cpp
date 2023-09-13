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
    std::shared_ptr<SourceFileAttributeInfo> sourceFileAttr = std::dynamic_pointer_cast<SourceFileAttributeInfo>(attr);
    if (sourceFileAttr != nullptr) {
      return constant_pool_->GetUtf8(sourceFileAttr->sourceFileIndex);
    }
  }
  return {};
}


void ParseAndCheckMagic(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  u4 targetMagic = 0xCAFEBABE;//little endian
  ParseUint(data, pos, file->magic_);
  LOG_IF(FATAL, file->magic_ != targetMagic) << "Magic number wrong";
}
void ParseAndCheckVersion(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
    ParseUint(data, pos, file->minor_version_);
    ParseUint(data, pos, file->major_version_);
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
  //return pos;
  LOG(FATAL) << "java.lang.UnsupportedClassVersionError major version " << file->major_version_ << " minor version " << file->minor_version_;
}
std::shared_ptr<ConstantInfo> CreateConstantInfo(u1 tag) {
  switch (tag) {
    case CONSTANT_Utf8: return std::make_shared<ConstantUtf8Info>();
    case CONSTANT_Integer: return std::make_shared<ConstantIntegerInfo>();
    case CONSTANT_Float: return std::make_shared<ConstantFloatInfo>();
    case CONSTANT_Long: return std::make_shared<ConstantLongInfo>();
    case CONSTANT_Double: return std::make_shared<ConstantDoubleInfo>();
    case CONSTANT_Class: return std::make_shared<ConstantClassInfo>();
    case CONSTANT_String: return std::make_shared<ConstantStringInfo>();
    case CONSTANT_Fieldref: return std::make_shared<ConstantFieldrefInfo>();
    case CONSTANT_Methodref: return std::make_shared<ConstantMethodrefInfo>();
    case CONSTANT_InterfaceMethodref: return std::make_shared<ConstantInterfaceMethodrefInfo>();
    case CONSTANT_NameAndType: return std::make_shared<ConstantNameAndTypeInfo>();
    case CONSTANT_MethodHandle: return std::make_shared<ConstantMethodHandleInfo>();
    case CONSTANT_MethodType: return std::make_shared<ConstantMethodTypeInfo>();
    case CONSTANT_Dynamic: return std::make_shared<ConstantDynamicInfo>();
    case CONSTANT_InvokeDynamic: return std::make_shared<ConstantInvokeDynamicInfo>();
    case CONSTANT_Module: return std::make_shared<ConstantModuleInfo>();
    case CONSTANT_Package: return std::make_shared<ConstantPackageInfo>();
    default: break;
  }
  LOG(FATAL) << "java.lang.ClassFormatError: constant pool tag "<< tag;
}
std::shared_ptr<ConstantInfo> ParseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
  u1 tag = 0;
    ParseUint(classData, pos, tag);
  //LOG(INFO) << "Constant info tag = " << (int)tag;
  std::shared_ptr<ConstantInfo> constantInfo = CreateConstantInfo(tag);
  //constantInfo->tag_ = tag;
  constantInfo->ParseConstantInfo(classData, pos);
  return constantInfo;
}
void ParseConstantPool(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  std::shared_ptr<ConstantPool> constantPoolPtr = std::make_shared<ConstantPool>();
    ParseUint(data, pos, constantPoolPtr->constant_pool_count_);
  constantPoolPtr->constant_infos_.push_back(nullptr);
  for (u2 i = 1; i < constantPoolPtr->constant_pool_count_; i++) {
    std::shared_ptr<ConstantInfo> constantInfo = ParseConstantInfo(data, pos);
    constantPoolPtr->constant_infos_.push_back(constantInfo);
    switch ((int32_t)constantInfo->tag_) {
      case CONSTANT_Double:
      case CONSTANT_Long:
        i++;
        constantPoolPtr->constant_infos_.push_back(nullptr);
        break;
    }
  }
  file->constant_pool_ = constantPoolPtr;
}
void ParseAccessFlags(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
    ParseUint(data, pos, file->access_flags_);
}
void ParseThisClass(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
    ParseUint(data, pos, file->this_class_);
}
void ParseSuperClass(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
    ParseUint(data, pos, file->super_class_);
}
void ParseInterfaces(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  u2 count = 0;
    ParseUint(data, pos, count);
  //LOG(INFO) << "Interface count = " << static_cast<int>(count);
  u2 interfaceIndex = 0;
  for (u2 i = 0; i < count; i++) {
      ParseUint(data, pos, interfaceIndex);
    file->interfaces_.push_back(interfaceIndex);
  }
}

void ParseMembers(std::shared_ptr<ClassData> data, std::vector<std::shared_ptr<MemberInfo>>& member_infos,
                  std::shared_ptr<ConstantPool> cp, int& pos) {
  u2 count = 0;
    ParseUint(data, pos, count);
  //LOG(INFO) << "Member counts = " << count;
  for (u2 i = 0; i < count; i++) {
    member_infos.push_back(ParseMember(data, cp, pos));
  }
}

std::shared_ptr<MemberInfo> ParseMember(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp, int& pos) {
  std::shared_ptr<MemberInfo> memberInfo = std::make_shared<MemberInfo>(cp);
    ParseUint(data, pos, memberInfo->access_flags_);
    ParseUint(data, pos, memberInfo->name_index_);
  //LOG(INFO) << "name_index_ = " << memberInfo->name_index_ << " name = " << cp->GetUtf8(memberInfo->name_index_);
    ParseUint(data, pos, memberInfo->descriptor_index_);
    ParseAttributeInfos(data, cp, memberInfo->attributes_, pos);
  return memberInfo;
}
void ParseFieldInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
    ParseMembers(data, file->fields_, file->constant_pool_, pos);
}
void ParseMethodInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
    ParseMembers(data, file->methods_, file->constant_pool_, pos);
}


std::shared_ptr<AttributeInfo> CreateAttributeInfo(const string& attrName, u4 attrLen, std::shared_ptr<ConstantPool> cp) {
  std::shared_ptr<AttributeInfo> ptr;
  if (attrName == "Code") {
    return std::make_shared<CodeAttributeInfo>(cp);
  } else if (attrName == "ConstantValue") {
    return std::make_shared<ConstantValueAttributeInfo>();
  } else if (attrName == "DepreCated") {
    return  std::make_shared<DeprecatedAttributeInfo>();
  } else if (attrName == "Exceptions") {
    return std::make_shared<ExceptionsAttributeInfo>();
  } else if (attrName == "LineNumberTable") {
    return std::make_shared<LineNumberTableAttributeInfo>();
  } else if (attrName == "LocalVariableTable") {
    return std::make_shared<LocalVariableTableAttributeInfo>();
  } else if (attrName == "SourceFile") {
    return std::make_shared<SourceFileAttributeInfo>();
  } else if (attrName == "Synthetic") {
    return std::make_shared<SyntheticAttributeInfo>();
  } else {
    return std::make_shared<UnparsedAttributeInfo>(attrName, attrLen);
  }
}
std::shared_ptr<AttributeInfo> ParseAttributeInfo(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp, int& pos) {
  u2 attrNameIndex = 0;
    ParseUint(data, pos, attrNameIndex);
  string attrName = cp->GetUtf8(attrNameIndex);
  u4 attrLen = 0;
    ParseUint(data, pos, attrLen);
  std::shared_ptr<AttributeInfo> attrInfo = CreateAttributeInfo(attrName, attrLen, cp);
    attrInfo->ParseAttrInfo(data, pos);
  return attrInfo;
}
void ParseAttributeInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int &pos) {
    ParseAttributeInfos(data, file->constant_pool_, file->attributes_, pos);
}
void ParseAttributeInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp, std::vector<std::shared_ptr<AttributeInfo>>& attributes, int& pos) {
  u2 attributeCount = 0;
    ParseUint(data, pos, attributeCount);
  for (u2 i = 0; i < attributeCount; i++) {
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
  std::shared_ptr<ClassFile> classFile = std::make_shared<ClassFile>();
  int pos = 0;
    ParseAndCheckMagic(data, classFile, pos);
    ParseAndCheckVersion(data, classFile, pos);
    ParseConstantPool(data, classFile, pos);
    ParseAccessFlags(data, classFile, pos);
    ParseThisClass(data, classFile, pos);
    ParseSuperClass(data, classFile, pos);
    ParseInterfaces(data, classFile, pos);
    ParseFieldInfos(data, classFile, pos);
    ParseMethodInfos(data, classFile, pos);
    ParseAttributeInfos(data, classFile, pos);
  return classFile;
}
}

