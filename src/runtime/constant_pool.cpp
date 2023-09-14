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
std::shared_ptr<Constant> ConstantPool::GetConstant(uint32_t index) {
  if (index >= 0 && index < constants_.size()) {
    return constants_[index];
  }
  LOG(ERROR) << "Not found contant in " << index;
  return nullptr;
}

ConstantPool::ConstantPool(Class* class_ptr, std::shared_ptr<classfile::ConstantPool> cf_constant_pool) : class_ptr_(class_ptr) {
  constants_.resize(cf_constant_pool->constant_pool_count_);
  for (int i = 1; i < cf_constant_pool->constant_pool_count_; i++) {
    auto cfConstant = cf_constant_pool->GetConstantInfo(i);
    int32_t tag = (int32_t)cfConstant->tag_;
    switch (tag) {
      case classfile::kConstantInteger:
        constants_[i] = std::make_shared<IntegerConstant>(std::dynamic_pointer_cast<classfile::ConstantIntegerInfo>(cfConstant));
        break;
      case classfile::kConstantFloat:
        constants_[i] = std::make_shared<FloatConstant>(std::dynamic_pointer_cast<classfile::ConstantFloatInfo>(cfConstant));
        break;
      case classfile::kConstantLong:
        constants_[i] = std::make_shared<LongConstant>(std::dynamic_pointer_cast<classfile::ConstantLongInfo>(cfConstant));
        constants_[++i] = nullptr;
        break;
      case classfile::kConstantDouble:
        constants_[i] = std::make_shared<DoubleConstant>(std::dynamic_pointer_cast<classfile::ConstantDoubleInfo>(cfConstant));
        constants_[++i] = nullptr;
        break;
      case classfile::kConstantString: {
        std::shared_ptr<classfile::ConstantStringInfo> cfStringInfo = std::dynamic_pointer_cast<classfile::ConstantStringInfo>(cfConstant);
        constants_[i] = std::make_shared<StringConstant>(cf_constant_pool->GetUtf8(cfStringInfo->string_index_));
        break;
      }
      case classfile::kConstantClass: {
        std::shared_ptr<classfile::ConstantClassInfo> cfClassInfo = std::dynamic_pointer_cast<classfile::ConstantClassInfo>(cfConstant);
        constants_[i] = std::make_shared<ClassRefConstant>(std::shared_ptr<ConstantPool>(this),
                                                           cf_constant_pool->GetUtf8(cfClassInfo->name_index_));
        break;
      }
      case classfile::kConstantFieldRef: {
        std::shared_ptr<classfile::ConstantFieldrefInfo> cfFieldrefInfo = std::dynamic_pointer_cast<classfile::ConstantFieldrefInfo>(cfConstant);
        std::string className = cf_constant_pool->GetClassName(cfFieldrefInfo->class_index_);
        std::string fieldName;
        std::string fieldDescriptor;
        cf_constant_pool->GetNameAndType(cfFieldrefInfo->name_and_type_index_, fieldName, fieldDescriptor);
        constants_[i] = std::make_shared<FieldRefConstant>(std::shared_ptr<ConstantPool>(this), className, fieldName, fieldDescriptor);
        break;
      }
      case classfile::kConstantMethodRef: {
        std::shared_ptr<classfile::ConstantMethodrefInfo> cfMethodrefInfo = std::dynamic_pointer_cast<classfile::ConstantMethodrefInfo>(cfConstant);
        std::string className = cf_constant_pool->GetClassName(cfMethodrefInfo->class_index_);
        std::string methodName;
        std::string methodDescriptor;
        cf_constant_pool->GetNameAndType(cfMethodrefInfo->name_and_type_index_, methodName, methodDescriptor);
        constants_[i] = std::make_shared<MethodRefConstant>(std::shared_ptr<ConstantPool>(this), className, methodName, methodDescriptor);
        break;
      }
      case classfile::kConstantInterfaceMethodRef: {
        std::shared_ptr<classfile::ConstantInterfaceMethodrefInfo> cfInterfaceMethodrefInfo = std::dynamic_pointer_cast<classfile::ConstantInterfaceMethodrefInfo>(cfConstant);
        std::string className = cf_constant_pool->GetClassName(cfInterfaceMethodrefInfo->class_index_);
        std::string methodName;
        std::string methodDescriptor;
        cf_constant_pool->GetNameAndType(cfInterfaceMethodrefInfo->name_and_type_index_, methodName, methodDescriptor);
        constants_[i] = std::make_shared<InterfaceMethodRefConstant>(std::shared_ptr<ConstantPool>(this), className, methodName, methodDescriptor);
        break;
      }
      case classfile::kConstantNameAndType:
        //constants_[i] = std::make_shared<NameAndTypeConstant>(std::dynamic_pointer_cast<classfile::ConstantNameAndTypeInfo>(cfConstant));
        break;
      case classfile::kConstantMethodHandle:
        //constants_[i] = std::make_shared<MethodHandleConstant>(std::dynamic_pointer_cast<classfile::ConstantMethodHandleInfo>(cfConstant));
        break;
      case classfile::kConstantMethodType:
        //constants_[i] = std::make_shared<MethodTypeConstant>(std::dynamic_pointer_cast<classfile::ConstantMethodTypeInfo>(cfConstant));
        break;
      case classfile::kConstantInvokeDynamic:
        //constants_[i] = std::make_shared<InvokeDynamicConstant>(std::dynamic_pointer_cast<classfile::ConstantInvokeDynamicInfo>(cfConstant));
        break;
      default:
        //LOG(ERROR) << "Not found constant type " << cfConstant->tag_;
        break;
    }
  }
}
Class* SymRefConstant::ResolveClass() {
  if (class_ptr_ == nullptr) {
    class_ptr_ = constant_pool_->GetClass()->GetClassLoader()->LoadClass(class_name_);
  }
  if (!class_ptr_->IsAccessibleTo(constant_pool_->GetClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  return class_ptr_;
}
std::shared_ptr<Field> FieldRefConstant::resolveField() {
  if (field_ == nullptr) {
    Class* d = ResolveClass();
    field_ = d->LookupField(GetName(), GetDescriptor());
    if (field_ == nullptr) {
      LOG(FATAL) << "java.lang.NoSuchFieldError";
    }
  }
  if (!field_->IsAccessibleTo(GetConstantPool()->GetClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  return field_;
}
std::shared_ptr<Method> MethodRefConstant::ResolveMethod() {
  if (nullptr != method_) {
    return method_;
  }
  Class* d = ResolveClass();
  if (d->IsInterface()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  std::shared_ptr<Method> method = d->LookupMethod(GetName(), GetDescriptor());
  if (nullptr == method) {
    LOG(FATAL) << "java.lang.NoSuchMethodError";
  }
  if (!method->IsAccessibleTo(GetConstantPool()->GetClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  method_ = method;
  return method_;
}
std::shared_ptr<Method> InterfaceMethodRefConstant::ResolveInterfaceMethod() {
  if (nullptr != interface_method_) {
    return interface_method_;
  }
  Class* d = ResolveClass();
  if (!d->IsInterface()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  std::shared_ptr<Method> method = d->LookupMethod(GetName(), GetDescriptor());
  if (nullptr == method) {
    LOG(FATAL) << "java.lang.NoSuchMethodError";
  }
  if (!method->IsAccessibleTo(GetConstantPool()->GetClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  interface_method_ = method;
  return interface_method_;
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