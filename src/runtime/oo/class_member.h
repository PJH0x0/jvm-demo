#pragma once

#include "class.h"
#include <classfile/attr_info.h>
#include <classfile/member_info.h>
#include <stdint.h>
#include <memory>
#include <string>
#include <vector>
namespace runtime {
class ClassMember {
public:
  ClassMember(const std::shared_ptr<classfile::MemberInfo>&, Class*);
  uint16_t GetAccessFlags() const {
    return access_flags_;
  }
  std::string GetName() const {
    return name_;
  }
  std::string GetDescriptor() const {
    return descriptor_;
  }
  Class* GetClass() const {
    return class_ptr_;
  }
  bool IsAccessibleTo(Class* class_ptr) const;

  bool IsPublic() const {
    return (access_flags_ & ACC_PUBLIC) != 0;
  }
  bool IsPrivate() const {
    return (access_flags_ & ACC_PRIVATE) != 0;
  }
  bool IsProtected() const {
    return (access_flags_ & ACC_PROTECTED) != 0;
  }
  bool IsStatic() const {
    return (access_flags_ & ACC_STATIC) != 0;
  }
  bool IsFinal() const {
    return (access_flags_ & ACC_FINAL) != 0;
  }
protected:
    std::string name_;
    std::string descriptor_;
    Class* class_ptr_;
    uint16_t access_flags_;
};



}
