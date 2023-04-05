#include "member_info.h"
#include <memory>
#include <string>
#include <typeinfo>
#include "attr_info.h"
#include "constant_pool.h"
#include <glog/logging.h>
namespace classfile {

const std::shared_ptr<CodeAttributeInfo> MemberInfo::getCodeAttribute() const {
  LOG(INFO) << "MemberInfo::getCodeAttribute() " << getName() << " codesize = " << attributes.size();
  for (auto attribute : attributes) {
    std::shared_ptr<CodeAttributeInfo> codeAttr = std::dynamic_pointer_cast<CodeAttributeInfo>(attribute);
    if (codeAttr != nullptr) {
      LOG(INFO) << "Found code attribute";
      return codeAttr;
    } else {
      LOG(INFO) << "attribute = " << typeid(*attribute).name();
    }
  }
  LOG(ERROR) << "Not found code attribute";
  return nullptr;
}
const std::shared_ptr<ConstantValueAttributeInfo> MemberInfo::getConstantAttribute() const {
  for (auto attribute : attributes) {
    std::shared_ptr<ConstantValueAttributeInfo> constantAttr = std::dynamic_pointer_cast<ConstantValueAttributeInfo>(attribute);
    if (constantAttr != nullptr) {
      return constantAttr;
    }
  }
  LOG(ERROR) << "Not found constant value attribute";
  return nullptr;
}
const std::string MemberInfo::getName() const {
  return cp->getUtf8(nameIndex);
}
const std::string MemberInfo::getDescriptor() const {
  return cp->getUtf8(descriptorIndex);
}
}