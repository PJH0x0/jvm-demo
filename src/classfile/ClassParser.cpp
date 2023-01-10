#include "ClassParser.h"
#include "ConstantPool.h"
#include "MemberInfo.h"
#include "AttributeInfo.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <glog/logging.h>
#include <ios>
#include <memory>
#include <string>
#include <vector>


namespace classfile {

void parseAndCheckMagic(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  u4 targetMagic = 0xCAFEBABE;//little endian
  parseUint(data, pos, file->magic);
  LOG(INFO) << "magic " << std::hex << file->magic;
  LOG_IF(FATAL, file->magic != targetMagic) << "Magic number wrong";
}
void parseAndCheckVersion(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  parseUint(data, pos, file->minorVersion);
  parseUint(data, pos, file->majorVersion);
  switch (file->majorVersion) {
    case 45:
      return;
    case 46:
    case 47:
    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
    if (file->minorVersion == 0) {
      return;
    }
  }
  //return pos;
  LOG(FATAL) << "java.lang.UnsupportedClassVersionError major version " << file->majorVersion << " minor version " << file->minorVersion;
}
std::shared_ptr<ConstantInfo> createConstantInfo(u1 tag) {
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
std::shared_ptr<ConstantInfo> parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
  u1 tag = 0;
  parseUint(classData, pos, tag);
  //LOG(INFO) << "Constant info tag = " << (int)tag;
  std::shared_ptr<ConstantInfo> constantInfo = createConstantInfo(tag);
  constantInfo->tag = tag;
  constantInfo->parseConstantInfo(classData, pos);
  return constantInfo;
}
void parseConstantPool(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  std::shared_ptr<ConstantPool> constantPoolPtr = std::make_shared<ConstantPool>();
  parseUint(data, pos, constantPoolPtr->constantPoolCount);
  constantPoolPtr->constantInfos.push_back(nullptr);
  for (u2 i = 1; i < constantPoolPtr->constantPoolCount; i++) {
    std::shared_ptr<ConstantInfo> constantInfo = parseConstantInfo(data, pos);
    constantPoolPtr->constantInfos.push_back(constantInfo);
    switch (constantInfo->tag) {
      case CONSTANT_Double:
      case CONSTANT_Long:
        i++;
        constantPoolPtr->constantInfos.push_back(nullptr);
        break;
    }
  }
  file->constantPool = constantPoolPtr;
}
void parseAccessFlags(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  parseUint(data, pos, file->accessFlags);
}
void parseThisClass(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  parseUint(data, pos, file->thisClass);
}
void parseSuperClass(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  parseUint(data, pos, file->superClass);
}
void parseInterfaces(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  u2 count = 0;
  parseUint(data, pos, count);
  LOG(INFO) << "Interface count = " << static_cast<int>(count);
  u2 interfaceIndex = 0;
  for (u2 i = 0; i < count; i++) {
    parseUint(data, pos, interfaceIndex);
    file->interfaces.push_back(interfaceIndex);
  }
}

void parseMembers(std::shared_ptr<ClassData> data, std::vector<std::shared_ptr<MemberInfo>>& memberInfos, 
                  std::shared_ptr<ConstantPool> cp, int& pos) {
  u2 count = 0;
  parseUint(data, pos, count);
  LOG(INFO) << "Member counts = " << count;
  for (u2 i = 0; i < count; i++) {
    memberInfos.push_back(parseMember(data, cp, pos));
  }
}

std::shared_ptr<MemberInfo> parseMember(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp, int& pos) {
  std::shared_ptr<MemberInfo> memberInfo = std::make_shared<MemberInfo>();
  parseUint(data, pos, memberInfo->accessFlags);
  parseUint(data, pos, memberInfo->nameIndex);
  LOG(INFO) << "nameIndex = " << memberInfo->nameIndex << " name = " << cp->getUtf8(memberInfo->nameIndex);
  parseUint(data, pos, memberInfo->descriptorIndex);
  parseAttributeInfos(data, cp, memberInfo->attributes, pos);
  return memberInfo;
}
void parseFieldInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  parseMembers(data, file->fields, file->constantPool, pos);
}
void parseMethodInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  parseMembers(data, file->methods, file->constantPool, pos);
}


std::shared_ptr<AttributeInfo> createAttributeInfo(const string& attrName, u4 attrLen, std::shared_ptr<ConstantPool> cp) {
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
std::shared_ptr<AttributeInfo> parseAttributeInfo(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp, int& pos) {
  u2 attrNameIndex = 0;
  parseUint(data, pos, attrNameIndex);
  LOG(INFO) << "attrNameIndex = " << attrNameIndex;
  string attrName = cp->getUtf8(attrNameIndex);
  u4 attrLen = 0;
  parseUint(data, pos, attrLen);
  LOG(INFO) << "Attribute name = " << attrName << " length = " << attrLen;
  std::shared_ptr<AttributeInfo> attrInfo = createAttributeInfo(attrName, attrLen, cp);
  attrInfo->parseAttrInfo(data, pos);
  return attrInfo;
}
void parseAttributeInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int &pos) {
  parseAttributeInfos(data, file->constantPool, file->attributes, pos);
}
void parseAttributeInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ConstantPool> cp, std::vector<std::shared_ptr<AttributeInfo>>& attributes, int& pos) {
  u2 attributeCount = 0;
  parseUint(data, pos, attributeCount);
  for (u2 i = 0; i < attributeCount; i++) {
    attributes.push_back(parseAttributeInfo(data, cp, pos));
  }
}
void endianSwap(uint8_t* data, int size) {
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
std::shared_ptr<ClassFile> parse(std::shared_ptr<ClassData> data) {
  std::shared_ptr<ClassFile> classFile = std::make_shared<ClassFile>();
  int pos = 0;
  parseAndCheckMagic(data, classFile, pos);
  parseAndCheckVersion(data, classFile, pos);
  parseConstantPool(data, classFile, pos);
  parseAccessFlags(data, classFile, pos);
  parseThisClass(data, classFile, pos);
  parseSuperClass(data, classFile, pos);
  parseInterfaces(data, classFile, pos);
  parseFieldInfos(data, classFile, pos);
  parseMethodInfos(data, classFile, pos);
  parseAttributeInfos(data, classFile, pos);
  return classFile;
}
}

