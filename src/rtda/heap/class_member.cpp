#include "class_member.h"
#include "classfile/attr_info.h"
#include <memory>


namespace rtda {
ClassMember::ClassMember(std::shared_ptr<classfile::MemberInfo> memberInfo, std::shared_ptr<Class> classPtr) : 
  mAccessFlags(memberInfo->accessFlags),
  mClassPtr(classPtr) {

  mName = memberInfo->getName();
  mDescriptor = memberInfo->getDescriptor();
}

bool ClassMember::isAccessibleTo(std::shared_ptr<Class> classPtr) {
  if (isPublic()) {
    return true;
  }
  std::shared_ptr<Class> thisClass = mClassPtr;
  if (isProtected()) {
    return thisClass->isSubClassOf(classPtr) || thisClass->getPackageName() == classPtr->getPackageName();
  }
  if (!isPrivate()) {
    return thisClass->getPackageName() == classPtr->getPackageName();
  }
  return thisClass == classPtr;
}



}