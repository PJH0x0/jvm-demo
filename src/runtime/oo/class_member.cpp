#include "class_member.h"
#include "classfile/attr_info.h"
#include <memory>


namespace runtime {
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
  if (isProtected()) {
    return Class::isSubClassOf(mClassPtr, classPtr) || mClassPtr->getPackageName() == mClassPtr->getPackageName();
  }
  if (!isPrivate()) {
    return mClassPtr->getPackageName() == mClassPtr->getPackageName();
  }
  return mClassPtr == classPtr;
}



}