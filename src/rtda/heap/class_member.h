#pragma once

#include "class.h"
#include <classfile/attr_info.h>
#include <classfile/member_info.h>
#include <stdint.h>
#include <memory>
#include <string>
#include <vector>
namespace rtda {
struct ClassMember {
  uint16_t mAccessFlags;
  std::string mName;
  std::string mDescriptor;
  std::shared_ptr<Class> mClassPtr;
  ClassMember(std::shared_ptr<classfile::MemberInfo>, std::shared_ptr<Class>); 
  bool isAccessibleTo(std::shared_ptr<Class> classPtr);
  bool isPublic() {
    return (mAccessFlags & ACC_PUBLIC) != 0;
  }
  bool isPrivate() {
    return (mAccessFlags & ACC_PRIVATE) != 0;
  }
  bool isProtected() {
    return (mAccessFlags & ACC_PROTECTED) != 0;
  }
  bool isStatic() {
    return (mAccessFlags & ACC_STATIC) != 0;
  }
  bool isFinal() {
    return (mAccessFlags & ACC_FINAL) != 0;
  }
};



}
