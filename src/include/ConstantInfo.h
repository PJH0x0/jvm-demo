#pragma once

#include "ClassParser.h"
#include "ClassDataType.h"
#include <clocale>
#include <cstdlib>
#include <cstring>
#include <ios>
#include <iostream>
#include <locale>
#include <memory>
#include <cstdint>
#include <string>
#include <codecvt>
#include <glog/logging.h>

namespace JVM {
  class ConstantPool;
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
    u1 tag;
    ConstantInfo() {

    }
    virtual void parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
      LOG(FATAL) << "UnsupportedOperation parse constant info in base class";
    }
  };
  struct ConstantUtf8Info : public ConstantInfo {
    string value;
    void parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
      u2 length = 0;
      parseUint(classData, pos, length);
      LOG(INFO) << "parse ConstantUtf8Info string length = " << length;
      u1* tmp = parseBytes(classData, pos, length);
      value = decodeMUTF8(tmp, length);
      LOG(INFO) << "value length = " << value.size() << " value = " << value;
    }
    //transfer java Modified Utf8 to unicode
    std::string decodeMUTF8(u1* utf8Str, int len) {
      char* unicodeStr = (char*) malloc(len+1);
      memset(unicodeStr, 0, len+1);
      int32_t c, char2, char3;
      int32_t count = 0;
      int32_t chararr_count = 0;
      while (count < len) {
        c = static_cast<int32_t>(utf8Str[count]) & 0xff;
        if (c > 127) break;
        count++;
        unicodeStr[chararr_count++] = static_cast<char>(c);
      }
      while (count < len) {
        c = static_cast<int32_t>(utf8Str[count]) & 0xff;
        switch (c >> 4) {
          
          case 0:
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
          case 7:
            /* 0xxxxxxx*/
            count++;
            unicodeStr[chararr_count++]=static_cast<char>(c);
            break;
          case 12:
          case 13:
            /* 110x xxxx   10xx xxxx*/
            count += 2;
            if (count > len) {
              LOG(FATAL) << "malformed input: partial character at end";
            }
            char2 = static_cast<int32_t>(utf8Str[count-1]);
            if ((char2 & 0xC0) != 0x80) {
              LOG(FATAL) << "malformed input around byte " << count;
            }
            unicodeStr[chararr_count++] = static_cast<char>(((c & 0x1F) << 6) | (char2 & 0x3F));
            break;
          case 14:
            /* 1110 xxxx  10xx xxxx  10xx xxxx */
            count += 3;
            if (count > len) {
              LOG(FATAL) << "malformed input: partial character at end";
            }
            char2 = utf8Str[count-2];
            char3 = utf8Str[count-1];
            if (((char2 & 0xC0) != 0x80) || ((char3 & 0xC0) != 0x80)) {
              LOG(FATAL) << "malformed input around byte " << count-1;
            }
            unicodeStr[chararr_count++] = static_cast<char>(((c & 0x0F) << 12) |
                                                    ((char2 & 0x3F) << 6)  |
                                                    ((char3 & 0x3F) << 0));
            break;
          default:
            LOG(FATAL) << "malformed input around byte " << count;
        }
      }
      return string(unicodeStr);
    }
  };
  struct ConstantIntegerInfo : public ConstantInfo {
    int32_t value;
    void parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
      u4 _value = 0;
      parseUint(classData, pos, _value);
      value = _value;
    }
  };
  struct ConstantFloatInfo : public ConstantInfo {
    float value;
    void parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
      u4 _value = 0;
      parseUint(classData, pos, _value);
      memcpy(&value, &_value, sizeof(value));
    }
  };
  struct ConstantLongInfo : public ConstantInfo {
    int64_t value;
    void parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
      u8 _value = 0;
      parseUint(classData, pos, _value);
      //memcpy(&value, _value, sizeof(value))
      value = _value;
    }
  };
  struct ConstantDoubleInfo : public ConstantInfo {
    double value;
    void parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
      u8 _value = 0;
      parseUint(classData, pos, _value);
      memcpy(&value, &_value, sizeof(value));
    }
  };
  struct ConstantClassInfo : public ConstantInfo {
    u2 nameIndex;
    void parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
      parseUint(classData, pos, nameIndex);
    }
  };
  struct ConstantStringInfo : public ConstantInfo {
    u2 stringIndex;
    void parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
      parseUint(classData, pos, stringIndex);
    }
  };
  struct ConstantMemberrefInfo : public ConstantInfo {
    u2 classIndex;
    u2 nameAndTypeIndex;
    void parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
      parseUint(classData, pos, classIndex);
      parseUint(classData, pos, nameAndTypeIndex);
    }
  };
  struct ConstantFieldrefInfo : public ConstantMemberrefInfo {};
  struct ConstantMethodrefInfo : public ConstantMemberrefInfo {};
  struct ConstantInterfaceMethodrefInfo : public ConstantMemberrefInfo {};
  struct ConstantNameAndTypeInfo : public ConstantInfo {
    u2 nameIndex;
    u2 descriptorIndex;
    void parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
      parseUint(classData, pos, nameIndex);
      parseUint(classData, pos, descriptorIndex);
    }
  };
  struct ConstantMethodHandleInfo : public ConstantInfo {
    u1 referenceKind;
    u2 referenceIndex; 
    void parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
      parseUint(classData, pos, referenceKind);
      parseUint(classData, pos, referenceIndex);
    }
  };
  struct ConstantMethodTypeInfo : public ConstantInfo {
    u2 descriptorIndex;
    void parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
      parseUint(classData, pos, descriptorIndex);
    }
  };
  struct ConstantDynamicInfo : public ConstantInfo {
    u2 bootstrapMethodAttrIndex;
    u2 nameAndTypeIndex;
    void parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
      parseUint(classData, pos, bootstrapMethodAttrIndex);
      parseUint(classData, pos, nameAndTypeIndex);
    }
  };
  struct ConstantInvokeDynamicInfo : public ConstantInfo {
    u2 bootstrapMethodAttrIndex;
    u2 nameAndTypeIndex;
    void parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
      parseUint(classData, pos, bootstrapMethodAttrIndex);
      parseUint(classData, pos, nameAndTypeIndex);
    }
  };
  struct ConstantModuleInfo : public ConstantInfo {
    u2 nameIndex;
    void parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
      parseUint(classData, pos, nameIndex);
    }
  };
  struct ConstantPackageInfo : public ConstantInfo {
    u2 nameIndex;
    void parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
      parseUint(classData, pos, nameIndex);
    }
  };
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
    LOG(INFO) << "Constant info tag = " << (int)tag;
    std::shared_ptr<ConstantInfo> constantInfo = createConstantInfo(tag);
    constantInfo->tag = tag;
    constantInfo->parseConstantInfo(classData, pos);
    return constantInfo;
  }
  
  
}