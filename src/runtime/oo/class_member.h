#pragma once

#include "class.h"
#include <classfile/attr_info.h>
#include <classfile/member_info.h>
#include <stdint.h>
#include <memory>
#include <string>
#include <vector>
namespace runtime {
struct ClassMember {

  public:
  ClassMember(std::shared_ptr<classfile::MemberInfo>, Class*); 
  uint16_t GetAccessFlags() const {
    return access_flags_;
  }
  std::string GetName() {
    return name_;
  }
  std::string GetDescriptor() {
    return descriptor_;
  }
  Class* getClass() {
    return class_ptr_;
  }
  bool IsAccessibleTo(Class* classPtr);
  bool isPublic() {
    return (access_flags_ & ACC_PUBLIC) != 0;
  }
  bool isPrivate() {
    return (access_flags_ & ACC_PRIVATE) != 0;
  }
  bool isProtected() {
    return (access_flags_ & ACC_PROTECTED) != 0;
  }
  bool isStatic() {
    return (access_flags_ & ACC_STATIC) != 0;
  }
  bool isFinal() {
    return (access_flags_ & ACC_FINAL) != 0;
  }
protected:
    std::string name_;
    std::string descriptor_;
    Class* class_ptr_;
    uint16_t access_flags_;
};



}
