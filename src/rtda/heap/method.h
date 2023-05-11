#pragma once

#include "class_member.h"
#include <classfile/attr_info.h>
#include <classfile/member_info.h>
#include <memory>
#include <vector>
#include <string>

namespace rtda {
struct Method : public ClassMember {
  typedef classfile::u1 u1;
  Method(std::shared_ptr<classfile::MemberInfo>, std::shared_ptr<Class>);
  uint32_t maxStack;
  uint32_t maxLocals;
  std::vector<u1> codes;
  uint32_t mArgSlotCount;

  void calcArgSlotCount();
  
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
struct MethodDescriptor {
  private:
  std::vector<std::string> mParameterTypes;
  std::string mReturnType;

  void parseMethodDescriptor(const std::string&);
  public:
  MethodDescriptor(const std::string& descriptor);
  std::vector<std::string> getParameterTypes();
  std::string getReturnType();
  
};
void createMethods(std::shared_ptr<Class>, std::vector<std::shared_ptr<classfile::MemberInfo>>&, std::vector<std::shared_ptr<Method>>&);

} // namespace rtda