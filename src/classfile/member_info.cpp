#include "member_info.h"
#include <memory>
#include <string>
#include <typeinfo>
#include "attr_info.h"
#include "constant_pool.h"
#include <glog/logging.h>
namespace classfile {

const std::shared_ptr<CodeAttributeInfo> MemberInfo::GetCodeAttribute() const {
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
const std::shared_ptr<ConstantValueAttributeInfo> MemberInfo::GetConstantAttribute() const {
  for (auto attribute : attributes_) {
    std::shared_ptr<ConstantValueAttributeInfo> constant_attr = std::dynamic_pointer_cast<ConstantValueAttributeInfo>(attribute);
    if (constant_attr != nullptr) {
      return constant_attr;
    }
  }
  //LOG(ERROR) << "Not found constant value attribute";
  return nullptr;
}
const std::string MemberInfo::GetName() const {
  return constant_pool_->GetUtf8(name_index_);
}
const std::string MemberInfo::GetDescriptor() const {
  return constant_pool_->GetUtf8(descriptor_index_);
}
}