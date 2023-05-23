#include "method.h"
#include <vector>
#include <string>

namespace rtda {
Method::Method(std::shared_ptr<classfile::MemberInfo> cfMethod, std::shared_ptr<Class> classPtr) :
  ClassMember(cfMethod, classPtr), mArgSlotCount(0) {
  calcArgSlotCount();
  std::shared_ptr<classfile::CodeAttributeInfo> codeAttr = cfMethod->getCodeAttribute();
  //registerNatives has no codes
  if (codeAttr == nullptr) {
    return;
  }
  maxStack = codeAttr->maxOperandStack;
  maxLocals = codeAttr->maxLocals;
  codes = codeAttr->codes;
}
void Method::calcArgSlotCount() {
  std::vector<std::string> paramTypes = MethodDescriptor(mDescriptor).getParameterTypes();
  for (auto paramType : paramTypes) {
    mArgSlotCount++;
    if (paramType == "J" || paramType == "D") {
      mArgSlotCount++;
    }
  }
  if (!isStatic()) {
    mArgSlotCount++;
  }
}
void createMethods(std::shared_ptr<Class> classPtr, std::vector<std::shared_ptr<classfile::MemberInfo>>& cfMethods, 
  std::vector<std::shared_ptr<Method>>& methods) {
  for (auto cfMethod: cfMethods) {
    std::shared_ptr<Method> method = std::make_shared<Method>(cfMethod, classPtr);
    methods.push_back(method);
  }
}

MethodDescriptor::MethodDescriptor(const std::string& descriptor) {
  parseMethodDescriptor(descriptor);
}
std::string MethodDescriptor::getReturnType() {
  return mReturnType;
}
std::vector<std::string> MethodDescriptor::getParameterTypes() {
  return mParameterTypes;
}
void MethodDescriptor::parseMethodDescriptor(const std::string& descriptor) {
  int begin = descriptor.find('(') + 1;
  int end = descriptor.find(')');
  std::string paramStr = descriptor.substr(begin, end - begin);
  while (paramStr.size() > 0) {
    std::string paramType;
    if (paramStr[0] == 'L') {
      int semicolonIndex = paramStr.find(';');
      paramType = paramStr.substr(0, semicolonIndex + 1);
      paramStr = paramStr.substr(semicolonIndex + 1);
    } else {
      paramType = paramStr[0];
      paramStr = paramStr.substr(1);
    }
    mParameterTypes.push_back(paramType);
  }
  mReturnType = descriptor.substr(end + 1);
}

} // namespace rtda