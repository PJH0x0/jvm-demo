#include "member_info.h"
#include <memory>
#include <string>
#include <typeinfo>
#include "attr_info.h"
#include "constant_pool.h"
namespace classfile {

const std::shared_ptr<CodeAttributeInfo> MemberInfo::getCodeAttribute() const {
  for (auto attribute : attributes) {
    if (typeid(*attribute) == typeid(CodeAttributeInfo)) {
      return std::dynamic_pointer_cast<CodeAttributeInfo>(attribute);
    }

  }
  LOG(ERROR) << "Not found code attribute";
  return nullptr;
}
const std::shared_ptr<ConstantValueAttributeInfo> MemberInfo::getConstantAttribute() const {
  for (auto attribute : attributes) {
    if (typeid(*attribute) == typeid(ConstantValueAttributeInfo)) {
      return std::dynamic_pointer_cast<ConstantValueAttributeInfo>(attribute);
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