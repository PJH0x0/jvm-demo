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
    return thisClass->isSubClassOf(classPtr) || thisClass->mPackageName == classPtr->mPackageName;
  }
  if (!isPrivate()) {
    return thisClass->mPackageName == classPtr->mPackageName;
  }
  return thisClass == classPtr;
}

Method::Method(std::shared_ptr<classfile::MemberInfo> cfMethod, std::shared_ptr<Class> classPtr) :
  ClassMember(cfMethod, classPtr) {
  std::shared_ptr<classfile::CodeAttributeInfo> codeAttr = cfMethod->getCodeAttribute();
  //registerNatives has no codes
  if (codeAttr == nullptr) {
    return;
  }
  maxStack = codeAttr->maxOperandStack;
  maxLocals = codeAttr->maxLocals;
  codes = codeAttr->codes;
}
void createFields(std::shared_ptr<Class> classPtr, std::vector<std::shared_ptr<classfile::MemberInfo>>& cfFields, 
  std::vector<std::shared_ptr<Field>>& fields) {
  for (auto cfField : cfFields) {
    std::shared_ptr<Field> field = std::make_shared<Field>(cfField, classPtr);
    fields.push_back(field);
  }
}
void createMethods(std::shared_ptr<Class> classPtr, std::vector<std::shared_ptr<classfile::MemberInfo>>& cfMethods, 
  std::vector<std::shared_ptr<Method>>& methods) {
  for (auto cfMethod: cfMethods) {
    std::shared_ptr<Method> method = std::make_shared<Method>(cfMethod, classPtr);
    methods.push_back(method);
  }
}
}