#include "member_info.h"
#include <memory>
#include <string>
#include <typeinfo>
#include "attr_info.h"
#include "constant_pool.h"
#include <glog/logging.h>
namespace classfile {

std::shared_ptr<CodeAttributeInfo> MemberInfo::GetCodeAttribute() const {
  //LOG(INFO) << "MemberInfo::GetCodeAttribute() " << GetName() << " codesize = " << attributes.size();
  for (auto attribute : attributes_) {
    std::shared_ptr<CodeAttributeInfo> code_attr = std::dynamic_pointer_cast<CodeAttributeInfo>(attribute);
    if (code_attr != nullptr) {
      return code_attr;
    }
  }
  //LOG(ERROR) << "Not found code attribute";
  return nullptr;
}
std::shared_ptr<ConstantValueAttributeInfo> MemberInfo::GetConstantAttribute() const {
  for (const auto& attribute : attributes_) {
    std::shared_ptr<ConstantValueAttributeInfo> constant_attr = std::dynamic_pointer_cast<ConstantValueAttributeInfo>(attribute);
    if (constant_attr != nullptr) {
      return constant_attr;
    }
  }
  //LOG(ERROR) << "Not found constant value attribute";
  return nullptr;
}
std::string MemberInfo::GetName() const {
  return constant_pool_->GetUtf8(name_index_);
}
std::string MemberInfo::GetDescriptor() const {
  return constant_pool_->GetUtf8(descriptor_index_);
}

void MemberInfo::PutAttributeInfo(std::shared_ptr<AttributeInfo> attribute_info) {
  attributes_.push_back(attribute_info);
}

MemberInfo::MemberInfo(u2 access_flags, u2 name_index, u2 descriptor_index,
                       const std::shared_ptr<ConstantPool>& constant_pool) : access_flags_(access_flags),
                                                                             name_index_(name_index),
                                                                             descriptor_index_(descriptor_index),
                                                                             constant_pool_(constant_pool) {}

u2 MemberInfo::GetAccessFlags() const {
  return access_flags_;
}

u2 MemberInfo::GetNameIndex() const {
  return name_index_;
}

u2 MemberInfo::GetDescriptorIndex() const {
  return descriptor_index_;
}


}