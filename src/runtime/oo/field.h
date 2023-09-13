#pragma once
#include "class_member.h"
#include <classfile/attr_info.h>
#include <classfile/member_info.h>
#include <memory>
#include <vector>
#include <string>

namespace runtime {
class Field : public ClassMember {
  private:
  uint32_t mSlotId;
  uint32_t  mConstValueIndex;
  public:
  Field(std::shared_ptr<classfile::MemberInfo> cfField, Class* classPtr) 
    : ClassMember(cfField, classPtr), mSlotId(0), mConstValueIndex(0) {
    std::shared_ptr<classfile::ConstantValueAttributeInfo> constantValueAttribute = cfField->GetConstantAttribute();
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
    return (access_flags_ & ACC_TRANSIENT) != 0;
  }
  bool isSynthetic() {
    return (access_flags_ & ACC_SYNTHETIC) != 0;
  }
  bool isEnum() {
    return (access_flags_ & ACC_ENUM) != 0;
  }
  bool isLongOrDouble() {
    return descriptor_ == "J" || descriptor_ == "D" || descriptor_[0] == '[' || descriptor_[0] == 'L';
  }
};


} // namespace runtime