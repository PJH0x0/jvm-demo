#pragma once
#include "class_member.h"
#include <classfile/attr_info.h>
#include <classfile/member_info.h>
#include <memory>
#include <vector>
#include <string>

namespace rtda {
class Field : public ClassMember {
  private:
  uint32_t mSlotId;
  uint32_t  mConstValueIndex;
  public:
  Field(std::shared_ptr<classfile::MemberInfo> cfField, std::shared_ptr<Class> classPtr) 
    : ClassMember(cfField, classPtr), mSlotId(0), mConstValueIndex(0) {
    std::shared_ptr<classfile::ConstantValueAttributeInfo> constantValueAttribute = cfField->getConstantAttribute();
    if (constantValueAttribute != nullptr) {
      mConstValueIndex = constantValueAttribute->constantValueIndex;
    }
  }

  uint32_t getSlotId() {
    return mSlotId;
  }
  void setSlotId(uint32_t slotId) {
    mSlotId = slotId;
  }
  uint32_t getConstValueIndex() {
    return mConstValueIndex;
  }
  
  bool isVolatile() {
    return (mAccessFlags & ACC_TRANSIENT) != 0;
  }
  bool isSynthetic() {
    return (mAccessFlags & ACC_SYNTHETIC) != 0;
  }
  bool isEnum() {
    return (mAccessFlags & ACC_ENUM) != 0;
  }
  bool isLongOrDouble() {
    return mDescriptor == "J" || mDescriptor == "D";
  }
};
void createFields(std::shared_ptr<Class>, std::vector<std::shared_ptr<classfile::MemberInfo>>&, 
  std::vector<std::shared_ptr<Field>>&);

} // namespace rtda