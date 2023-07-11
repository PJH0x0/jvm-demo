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
  protected:
  std::string mName;
  std::string mDescriptor;
  Class* mClassPtr;
  uint16_t mAccessFlags;
  public:
  ClassMember(std::shared_ptr<classfile::MemberInfo>, Class*); 
  uint16_t getAccessFlags() {
    return mAccessFlags;
  }
  std::string getName() {
    return mName;
  }
  std::string getDescriptor() {
    return mDescriptor;
  }
  Class* getClass() {
    return mClassPtr;
  }
  bool isAccessibleTo(Class* classPtr);
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
