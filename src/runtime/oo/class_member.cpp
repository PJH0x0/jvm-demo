#include "class_member.h"
#include "classfile/attr_info.h"
#include <memory>


namespace runtime {
ClassMember::ClassMember(std::shared_ptr<classfile::MemberInfo> member_info, Class* classPtr) :
        access_flags_(member_info->access_flags_),
        class_ptr_(classPtr) {

  name_ = member_info->GetName();
  descriptor_ = member_info->GetDescriptor();
}

bool ClassMember::IsAccessibleTo(Class* class_ptr) {
  if (IsPublic()) {
    return true;
  }
  if (IsProtected()) {
    return Class::IsSubClassOf(class_ptr_, class_ptr) || class_ptr_->GetPackageName() == class_ptr_->GetPackageName();
  }
  if (!IsPrivate()) {
    return class_ptr_->GetPackageName() == class_ptr_->GetPackageName();
  }
  return class_ptr_ == class_ptr;
}



}