#include "constant_pool.h"
#include "classfile/class_parser.h"
#include "classfile/constant_pool.h"
#include "class_loader.h"
#include <runtime/oo/object.h>
#include <runtime/oo/field.h>
#include <runtime/oo/method.h>
#include <glog/logging.h>
#include <memory>
#include <string>
#include <stdint.h>
#include <vector>

namespace runtime {
std::shared_ptr<Constant> ConstantPool::getConstant(uint32_t index) {
  if (index >= 0 && index < mConstants.size()) {
    return mConstants[index];
  }
  LOG(ERROR) << "Not found contant in " << index;
  return nullptr;
}

ConstantPool::ConstantPool(Class* clsPtr, std::shared_ptr<classfile::ConstantPool> cfConstantPool) : mClsPtr(clsPtr) {
  mConstants.resize(cfConstantPool->constantPoolCount);
  for (int i = 1; i < cfConstantPool->constantPoolCount; i++) {
    auto cfConstant = cfConstantPool->getConstantInfo(i);
    int32_t tag = (int32_t)cfConstant->mTag;
    switch (tag) {
      case classfile::CONSTANT_Integer:
        mConstants[i] = std::make_shared<IntegerConstant>(std::dynamic_pointer_cast<classfile::ConstantIntegerInfo>(cfConstant));
        break;
      case classfile::CONSTANT_Float:
        mConstants[i] = std::make_shared<FloatConstant>(std::dynamic_pointer_cast<classfile::ConstantFloatInfo>(cfConstant));
        break;
      case classfile::CONSTANT_Long:
        mConstants[i] = std::make_shared<LongConstant>(std::dynamic_pointer_cast<classfile::ConstantLongInfo>(cfConstant));
        mConstants[++i] = nullptr;
        break;
      case classfile::CONSTANT_Double:
        mConstants[i] = std::make_shared<DoubleConstant>(std::dynamic_pointer_cast<classfile::ConstantDoubleInfo>(cfConstant));
        mConstants[++i] = nullptr;
        break;
      case classfile::CONSTANT_String: {
        std::shared_ptr<classfile::ConstantStringInfo> cfStringInfo = std::dynamic_pointer_cast<classfile::ConstantStringInfo>(cfConstant);
        mConstants[i] = std::make_shared<StringConstant>(cfConstantPool->getUtf8(cfStringInfo->stringIndex));
        break;
      }
      case classfile::CONSTANT_Class: {
        std::shared_ptr<classfile::ConstantClassInfo> cfClassInfo = std::dynamic_pointer_cast<classfile::ConstantClassInfo>(cfConstant);
        mConstants[i] = std::make_shared<ClassRefConstant>(std::shared_ptr<ConstantPool>(this), cfConstantPool->getUtf8(cfClassInfo->nameIndex));
        break;
      }
      case classfile::CONSTANT_Fieldref: {
        std::shared_ptr<classfile::ConstantFieldrefInfo> cfFieldrefInfo = std::dynamic_pointer_cast<classfile::ConstantFieldrefInfo>(cfConstant);
        std::string className = cfConstantPool->getClassName(cfFieldrefInfo->classIndex);
        std::string fieldName;
        std::string fieldDescriptor;
        cfConstantPool->getNameAndType(cfFieldrefInfo->nameAndTypeIndex, fieldName, fieldDescriptor);
        mConstants[i] = std::make_shared<FieldRefConstant>(std::shared_ptr<ConstantPool>(this), className, fieldName, fieldDescriptor);
        break;
      }
      case classfile::CONSTANT_Methodref: {
        std::shared_ptr<classfile::ConstantMethodrefInfo> cfMethodrefInfo = std::dynamic_pointer_cast<classfile::ConstantMethodrefInfo>(cfConstant);
        std::string className = cfConstantPool->getClassName(cfMethodrefInfo->classIndex);
        std::string methodName;
        std::string methodDescriptor;
        cfConstantPool->getNameAndType(cfMethodrefInfo->nameAndTypeIndex, methodName, methodDescriptor);
        mConstants[i] = std::make_shared<MethodRefConstant>(std::shared_ptr<ConstantPool>(this), className, methodName, methodDescriptor);
        break;
      }
      case classfile::CONSTANT_InterfaceMethodref: {
        std::shared_ptr<classfile::ConstantInterfaceMethodrefInfo> cfInterfaceMethodrefInfo = std::dynamic_pointer_cast<classfile::ConstantInterfaceMethodrefInfo>(cfConstant);
        std::string className = cfConstantPool->getClassName(cfInterfaceMethodrefInfo->classIndex);
        std::string methodName;
        std::string methodDescriptor;
        cfConstantPool->getNameAndType(cfInterfaceMethodrefInfo->nameAndTypeIndex, methodName, methodDescriptor);
        mConstants[i] = std::make_shared<InterfaceMethodRefConstant>(std::shared_ptr<ConstantPool>(this), className, methodName, methodDescriptor);
        break;
      }
      case classfile::CONSTANT_NameAndType:
        //mConstants[i] = std::make_shared<NameAndTypeConstant>(std::dynamic_pointer_cast<classfile::ConstantNameAndTypeInfo>(cfConstant));
        break;
      case classfile::CONSTANT_MethodHandle:
        //mConstants[i] = std::make_shared<MethodHandleConstant>(std::dynamic_pointer_cast<classfile::ConstantMethodHandleInfo>(cfConstant));
        break;
      case classfile::CONSTANT_MethodType:
        //mConstants[i] = std::make_shared<MethodTypeConstant>(std::dynamic_pointer_cast<classfile::ConstantMethodTypeInfo>(cfConstant));
        break;
      case classfile::CONSTANT_InvokeDynamic:
        //mConstants[i] = std::make_shared<InvokeDynamicConstant>(std::dynamic_pointer_cast<classfile::ConstantInvokeDynamicInfo>(cfConstant));
        break;
      default:
        //LOG(ERROR) << "Not found constant type " << cfConstant->mTag;
        break;
    }
  }
}
Class* SymRefConstant::resolveClass() {
  if (mClsPtr == nullptr) {
    mClsPtr = mConstantPool->getClass()->GetClassLoader()->loadClass(mClassName);
  }
  if (!mClsPtr->IsAccessibleTo(mConstantPool->getClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  return mClsPtr;
}
std::shared_ptr<Field> FieldRefConstant::resolveField() {
  if (mFieldPtr == nullptr) {
    Class* d = resolveClass();
    mFieldPtr = d->LookupField(name(), descriptor());
    if (mFieldPtr == nullptr) {
      LOG(FATAL) << "java.lang.NoSuchFieldError";
    }
  }
  if (!mFieldPtr->IsAccessibleTo(constantPool()->getClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  return mFieldPtr;
}
std::shared_ptr<Method> MethodRefConstant::resolveMethod() {
  if (nullptr != mMethodPtr) {
    return mMethodPtr;
  }
  Class* d = resolveClass();
  if (d->IsInterface()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  std::shared_ptr<Method> method = d->LookupMethod(name(), descriptor());
  if (nullptr == method) {
    LOG(FATAL) << "java.lang.NoSuchMethodError";
  }
  if (!method->IsAccessibleTo(constantPool()->getClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  mMethodPtr = method;
  return mMethodPtr;
}
std::shared_ptr<Method> InterfaceMethodRefConstant::resolveInterfaceMethod() {
  if (nullptr != mMethodPtr) {
    return mMethodPtr;
  }
  Class* d = resolveClass();
  if (!d->IsInterface()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  std::shared_ptr<Method> method = d->LookupMethod(name(), descriptor());
  if (nullptr == method) {
    LOG(FATAL) << "java.lang.NoSuchMethodError";
  }
  if (!method->IsAccessibleTo(constantPool()->getClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  mMethodPtr = method;
  return mMethodPtr;
}
//transfer java Modified Utf8 to utf16
std::u16string StringConstant::decodeMUTF8(const char* utf8Str, int len) {
  std::vector<char16_t> unicodeStr;
  int32_t c, char2, char3;
  int32_t count = 0;
  while (count < len) {
    c = static_cast<int32_t>(utf8Str[count]) & 0xff;
    if (c > 127) break;
    count++;
    unicodeStr.push_back(static_cast<char16_t>(c));
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
        unicodeStr.push_back(static_cast<char16_t>(c));
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
        unicodeStr.push_back(static_cast<char16_t>(((c & 0x1F) << 6) | (char2 & 0x3F)));
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
        unicodeStr.push_back(static_cast<char16_t>(((c & 0x0F) << 12) |
                                            ((char2 & 0x3F) << 6)  |
                                            ((char3 & 0x3F) << 0)));
        break;
      default:
        LOG(FATAL) << "malformed input around byte " << count;
    }
  }
  unicodeStr.push_back(u'\0');
  char16_t* unicodeStrPtr = unicodeStr.data();
  //LOG(WARNING) << "utf16 size = " << unicodeStr.size();
  return std::u16string(unicodeStrPtr, unicodeStr.size()-1);
}
}