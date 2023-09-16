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
  uint32_t slot_id_;
  uint32_t  const_value_index_;
  public:
  Field(std::shared_ptr<classfile::MemberInfo> cf_field, Class* class_ptr)
    : ClassMember(cf_field, class_ptr), slot_id_(0), const_value_index_(0) {
    std::shared_ptr<classfile::ConstantValueAttributeInfo> constantValueAttribute = cf_field->GetConstantAttribute();
    if (constantValueAttribute != nullptr) {
      const_value_index_ = constantValueAttribute->GetConstantValueIndex();
    }
  }

  uint32_t GetSlotId() const {
    return slot_id_;
  }
  void SetSlotId(uint32_t slotId) {
    slot_id_ = slotId;
  }
  uint32_t GetConstValueIndex() const {
    return const_value_index_;
  }
  
  bool IsVolatile() const {
    return (access_flags_ & ACC_TRANSIENT) != 0;
  }
  bool IsSynthetic() const {
    return (access_flags_ & ACC_SYNTHETIC) != 0;
  }
  bool IsEnum() {
    return (access_flags_ & ACC_ENUM) != 0;
  }
  bool IsLongOrDouble() {
    return descriptor_ == "J" || descriptor_ == "D" || descriptor_[0] == '[' || descriptor_[0] == 'L';
  }
};


} // namespace runtime