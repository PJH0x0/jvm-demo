#include "include/ClassParser.h"
#include "include/ConstantPool.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <glog/logging.h>
#include <ios>
#include <memory>
#include <string>

namespace JVM {
u1* parseBytes(std::shared_ptr<ClassData> data, int& pos, int length) {
  u1* tmp = (u1*)malloc(length+1);
  memset(tmp, 0, length+1);
  for (int i = 0; i < length; i++) {
    tmp[i] = data->data[pos + i];
  }
  pos += length;
  return tmp;
}
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
void parseConstantPool(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {
  std::shared_ptr<ConstantPool> constantPoolPtr = std::make_shared<ConstantPool>();
  parseUint(data, pos, constantPoolPtr->constantPoolCount);
  for (u2 i = 1; i < constantPoolPtr->constantPoolCount; i++) {
    std::shared_ptr<ConstantInfo> constantInfo = parseConstantInfo(data, pos);
    constantPoolPtr->constantInfos.push_back(constantInfo);
    switch (constantInfo->tag) {
      case JVM::CONSTANT_Double:
      case JVM::CONSTANT_Long:
        i++;
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
void parseFieldInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos) {

}
void parseMethodInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
void parseAttributeInfos(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int& pos);
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

  return classFile;
}
}

