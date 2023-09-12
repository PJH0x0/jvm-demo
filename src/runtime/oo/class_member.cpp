#include "class_member.h"
#include "classfile/attr_info.h"
#include <memory>


namespace runtime {
ClassMember::ClassMember(std::shared_ptr<classfile::MemberInfo> memberInfo, Class* classPtr) :
        access_flags_(memberInfo->accessFlags),
        class_ptr_(classPtr) {

  mName = memberInfo->getName();
    descriptor_ = memberInfo->getDescriptor();
}

bool ClassMember::IsAccessibleTo(Class* classPtr) {
  if (isPublic()) {
    return true;
  }
  if (isProtected()) {
    return Class::IsSubClassOf(class_ptr_, classPtr) || class_ptr_->GetPackageName() == class_ptr_->GetPackageName();
  }
  if (!isPrivate()) {
    return class_ptr_->GetPackageName() == class_ptr_->GetPackageName();
  }
  return class_ptr_ == classPtr;
}



}