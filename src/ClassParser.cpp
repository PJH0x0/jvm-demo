#include "include/ClassParser.h"
#include <cstdint>
#include <cstdlib>
#include <glog/logging.h>
#include <ios>
#include <memory>
//#include "include/ClassReader.h"

namespace JVM {
template<typename T>
int read(std::shared_ptr<ClassData> data, int pos, T& t) {
  int size = sizeof(t);
  t = 0;
  for (int i = 0; i < size; i++) {
    t |= data->data[pos+i] << (size - 1 - i) * 8;
  }
  return size;
}
int parseAndCheckMagic(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int pos) {
  u4 targetMagic = 0xCAFEBABE;//little endian
  pos += read(data, pos, file->magic);
  LOG(INFO) << "magic " << std::hex << file->magic;
  LOG_IF(FATAL, file->magic != targetMagic) << "Magic number wrong";
  return pos;
}
int parseAndCheckVersion(std::shared_ptr<ClassData> data, std::shared_ptr<ClassFile> file, int pos) {
  pos += read(data, pos, file->minorVersion);
  pos += read(data, pos, file->majorVersion);
  switch (file->majorVersion) {
    case 45:
      return pos;
    case 46:
    case 47:
    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
    if (file->minorVersion == 0) {
      return pos;
    }
  }
  //return pos;
  LOG(FATAL) << "java.lang.UnsupportedClassVersionError major version " << file->majorVersion << " minor version " << file->minorVersion;
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
  pos = parseAndCheckMagic(data, classFile, pos);
  pos = parseAndCheckVersion(data, classFile, pos);
  return classFile;
}
}

