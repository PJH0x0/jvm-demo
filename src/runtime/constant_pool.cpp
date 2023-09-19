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
Constant* ConstantPool::GetConstant(uint32_t index) const {
  if (index < constants_.size()) {
    return constants_[index];
  }
  LOG(ERROR) << "Not found contant in " << index;
  return nullptr;
}

ConstantPool::ConstantPool(Class* class_ptr, const std::shared_ptr<classfile::ConstantPool>& cf_constant_pool) : class_ptr_(class_ptr) {
  constants_.resize(cf_constant_pool->GetConstantCount());
  for (int i = 1; i < cf_constant_pool->GetConstantCount(); i++) {
    auto cf_constant = cf_constant_pool->GetConstantInfo(i);
    ConstantType type = cf_constant->GetConstantType();
    switch (type) {
      case classfile::kConstantInteger: {
        auto cf_integer_constant = std::dynamic_pointer_cast<classfile::ConstantIntegerInfo>(cf_constant);
        constants_[i] = new IntegerConstant(cf_integer_constant->GetValue());
        break;
      }
      case classfile::kConstantFloat: {
        constants_[i] = new FloatConstant(std::dynamic_pointer_cast<classfile::ConstantFloatInfo>(cf_constant)->GetValue());
        break;
      }

      case classfile::kConstantLong:
        constants_[i] = new LongConstant(std::dynamic_pointer_cast<classfile::ConstantLongInfo>(cf_constant)->GetValue());
        constants_[++i] = nullptr;
        break;
      case classfile::kConstantDouble:
        constants_[i] = new DoubleConstant(std::dynamic_pointer_cast<classfile::ConstantDoubleInfo>(cf_constant)->GetValue());
        constants_[++i] = nullptr;
        break;
      case classfile::kConstantString: {
        std::shared_ptr<classfile::ConstantStringInfo> cf_string_info = std::dynamic_pointer_cast<classfile::ConstantStringInfo>(cf_constant);
        constants_[i] = new StringConstant(cf_constant_pool->GetUtf8(cf_string_info->GetStringIndex()));
        break;
      }
      case classfile::kConstantClass: {
        std::shared_ptr<classfile::ConstantClassInfo> cf_class_info = std::dynamic_pointer_cast<classfile::ConstantClassInfo>(cf_constant);
        constants_[i] = new ClassRefConstant(this,cf_constant_pool->GetUtf8(cf_class_info->GetNameIndex()));
        break;
      }
      case classfile::kConstantFieldRef: {
        std::shared_ptr<classfile::ConstantFieldRefInfo> cf_field_ref_info = std::dynamic_pointer_cast<classfile::ConstantFieldRefInfo>(cf_constant);
        std::string class_name = cf_constant_pool->GetClassName(cf_field_ref_info->GetClassIndex());
        std::string field_name;
        std::string field_descriptor;
        cf_constant_pool->GetNameAndType(cf_field_ref_info->GetNameAndTypeIndex(), field_name, field_descriptor);
        constants_[i] = new FieldRefConstant(this, class_name, field_name, field_descriptor);
        break;
      }
      case classfile::kConstantMethodRef: {
        std::shared_ptr<classfile::ConstantMethodRefInfo> cf_method_ref_info = std::dynamic_pointer_cast<classfile::ConstantMethodRefInfo>(cf_constant);
        std::string class_name = cf_constant_pool->GetClassName(cf_method_ref_info->GetClassIndex());
        std::string method_name;
        std::string method_descriptor;
        cf_constant_pool->GetNameAndType(cf_method_ref_info->GetNameAndTypeIndex(), method_name, method_descriptor);
        constants_[i] = new MethodRefConstant(this, class_name, method_name, method_descriptor);
        break;
      }
      case classfile::kConstantInterfaceMethodRef: {
        std::shared_ptr<classfile::ConstantInterfaceMethodRefInfo> cf_interface_method_ref_info = std::dynamic_pointer_cast<classfile::ConstantInterfaceMethodRefInfo>(cf_constant);
        std::string class_name = cf_constant_pool->GetClassName(cf_interface_method_ref_info->GetClassIndex());
        std::string method_name;
        std::string method_descriptor;
        cf_constant_pool->GetNameAndType(cf_interface_method_ref_info->GetNameAndTypeIndex(), method_name, method_descriptor);
        constants_[i] = new InterfaceMethodRefConstant(this, class_name, method_name, method_descriptor);
        break;
      }
      case classfile::kConstantNameAndType:
        //constants_[i] = std::make_shared<NameAndTypeConstant>(std::dynamic_pointer_cast<classfile::ConstantNameAndTypeInfo>(cf_constant));
        break;
      case classfile::kConstantMethodHandle:
        //constants_[i] = std::make_shared<MethodHandleConstant>(std::dynamic_pointer_cast<classfile::ConstantMethodHandleInfo>(cf_constant));
        break;
      case classfile::kConstantMethodType:
        //constants_[i] = std::make_shared<MethodTypeConstant>(std::dynamic_pointer_cast<classfile::ConstantMethodTypeInfo>(cf_constant));
        break;
      case classfile::kConstantInvokeDynamic:
        //constants_[i] = std::make_shared<InvokeDynamicConstant>(std::dynamic_pointer_cast<classfile::ConstantInvokeDynamicInfo>(cf_constant));
        break;
      default:
        //LOG(ERROR) << "Not found constant type " << cf_constant->type_;
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
const Field* FieldRefConstant::ResolveField() {
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
const Method* MethodRefConstant::ResolveMethod() {
  if (nullptr != method_) {
    return method_;
  }
  Class* d = ResolveClass();
  if (d->IsInterface()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  const Method* method = d->LookupMethod(GetName(), GetDescriptor());
  if (nullptr == method) {
    LOG(FATAL) << "java.lang.NoSuchMethodError";
  }
  if (!method->IsAccessibleTo(GetConstantPool()->GetClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  method_ = method;
  return method_;
}
const Method* InterfaceMethodRefConstant::ResolveInterfaceMethod() {
  if (nullptr != interface_method_) {
    return interface_method_;
  }
  Class* d = ResolveClass();
  if (!d->IsInterface()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  const Method* method = d->LookupMethod(GetName(), GetDescriptor());
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
std::u16string StringConstant::DecodeMutf8(const char* utf8Str, int len) {
  std::vector<char16_t> unicode_str;
  int32_t c, char2, char3;
  int32_t count = 0;
  while (count < len) {
    c = static_cast<int32_t>(utf8Str[count]) & 0xff;
    if (c > 127) break;
    count++;
    unicode_str.push_back(static_cast<char16_t>(c));
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
        unicode_str.push_back(static_cast<char16_t>(c));
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
        unicode_str.push_back(static_cast<char16_t>(((c & 0x1F) << 6) | (char2 & 0x3F)));
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
        unicode_str.push_back(static_cast<char16_t>(((c & 0x0F) << 12) |
                                                    ((char2 & 0x3F) << 6) |
                                                    ((char3 & 0x3F) << 0)));
        break;
      default:
        LOG(FATAL) << "malformed input around byte " << count;
    }
  }
  unicode_str.push_back(u'\0');
  char16_t* unicode_str_ptr = unicode_str.data();
  //LOG(WARNING) << "utf16 size = " << unicode_str.size();
  return std::u16string(unicode_str_ptr, unicode_str.size() - 1);
}
}