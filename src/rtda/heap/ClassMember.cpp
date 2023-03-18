#include "ClassMember.h"
#include "classfile/AttributeInfo.h"
#include <memory>


namespace rtda {
ClassMember::ClassMember(std::shared_ptr<classfile::MemberInfo> memberInfo, std::shared_ptr<Class> classPtr) : 
  mAccessFlags(memberInfo->accessFlags),
  mClassPtr(classPtr) {

  mName = memberInfo->getName();
  mDescriptor = memberInfo->getDescriptor();
}

Method::Method(std::shared_ptr<classfile::MemberInfo> cfMethod, std::shared_ptr<Class> classPtr) :
  ClassMember(cfMethod, classPtr) {
  std::shared_ptr<classfile::CodeAttributeInfo> codeAttr = cfMethod->getCodeAttribute();
  maxStack = codeAttr->maxOperandStack;
  maxLocals = codeAttr->maxLocals;
  codes = codeAttr->codes;
}
void createFields(std::shared_ptr<Class> classPtr, std::vector<std::shared_ptr<classfile::MemberInfo>>& cfFields, 
  std::vector<std::shared_ptr<Field>> fields) {
  for (auto cfField : cfFields) {
    std::shared_ptr<Field> field = std::make_shared<Field>(cfField, classPtr);
    fields.push_back(field);
  }
}
void createMethods(std::shared_ptr<Class> classPtr, std::vector<std::shared_ptr<classfile::MemberInfo>>& cfMethods, 
  std::vector<std::shared_ptr<Method>> methods) {
  for (auto cfMethod: cfMethods) {
    std::shared_ptr<Method> method = std::make_shared<Method>(cfMethod, classPtr);
    methods.push_back(method);
  }
}
}