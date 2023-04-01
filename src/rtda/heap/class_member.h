#pragma once

#include "class.h"
#include "classfile/attr_info.h"
#include <classfile/member_info.h>
#include <_types/_uint16_t.h>
#include <_types/_uint32_t.h>
#include <_types/_uint8_t.h>
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

struct Field : public ClassMember {
  uint32_t mSlotId;
  uint32_t  mConstValueIndex;
  Field(std::shared_ptr<classfile::MemberInfo> cfField, std::shared_ptr<Class> classPtr) 
    : ClassMember(cfField, classPtr), mSlotId(0), mConstValueIndex(0) {
    std::shared_ptr<classfile::ConstantValueAttributeInfo> constantValueAttribute = cfField->getConstantAttribute();
    if (constantValueAttribute != nullptr) {
      mConstValueIndex = constantValueAttribute->constantValueIndex;
    }
  }
  
  
  bool isVolatile() {
    return (mAccessFlags & ACC_TRANSIENT) != 0;
  }
  bool isSynthetic() {
    return (mAccessFlags & ACC_SYNTHETIC) != 0;
  }
  bool isEnum() {
    return (mAccessFlags & ACC_ENUM) != 0;
  }
  bool isLongOrDouble() {
    return mDescriptor == "J" || mDescriptor == "D";
  }
};
struct Method : public ClassMember {
  Method(std::shared_ptr<classfile::MemberInfo>, std::shared_ptr<Class>);
  uint32_t maxStack;
  uint32_t maxLocals;
  std::vector<classfile::u1> codes;
  bool isSynchronized() {
    return (mAccessFlags & ACC_SYNCHRONIZED) != 0;
  }
  bool isBridge() {
    return (mAccessFlags & ACC_BRIDGE) != 0;
  }
  bool isVarargs() {
    return (mAccessFlags & ACC_VARARGS) != 0;
  }
  bool isNative() {
    return (mAccessFlags & ACC_NATIVE) != 0;
  }
  bool isAbstract() {
    return (mAccessFlags & ACC_ABSTRACT) != 0;
  }
  bool isStrict() {
    return (mAccessFlags & ACC_STRICT) != 0;
  }
  bool isSynthetic() {
    return (mAccessFlags & ACC_SYNTHETIC) != 0;
  }
};
void createFields(std::shared_ptr<Class>, std::vector<std::shared_ptr<classfile::MemberInfo>>&, std::vector<std::shared_ptr<Field>>&);
void createMethods(std::shared_ptr<Class>, std::vector<std::shared_ptr<classfile::MemberInfo>>&, std::vector<std::shared_ptr<Method>>&);
}
