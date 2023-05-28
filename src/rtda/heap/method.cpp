#include "method.h"
#include <vector>
#include <string>

namespace rtda {
Method::Method(std::shared_ptr<classfile::MemberInfo> cfMethod, std::shared_ptr<Class> classPtr) :
  ClassMember(cfMethod, classPtr), mArgSlotCount(0), maxStack(0), maxLocals(0) {
  std::shared_ptr<classfile::CodeAttributeInfo> codeAttr = cfMethod->getCodeAttribute();
  //Native method has no codes
  if (codeAttr != nullptr) {
    maxStack = codeAttr->maxOperandStack;
    maxLocals = codeAttr->maxLocals;
    codes = codeAttr->codes;
  }
  mMethodDescriptor = std::make_shared<MethodDescriptor>(mDescriptor);
  calcArgSlotCount(mMethodDescriptor->getParameterTypes());

  if (isNative()) {
    injectCodeAttribute(mMethodDescriptor->getReturnType());
  }
  
}
void Method::calcArgSlotCount(const std::vector<string>& paramTypes) {
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

void Method::injectCodeAttribute(std::string returnType) {
  maxStack = 4;
  maxLocals = mArgSlotCount;
  switch (returnType[0]) {
    case 'V':
      codes.push_back(0xfe);
      codes.push_back(0xb1);
      break;
    case 'L':
    case '[':
      codes.push_back(0xfe);
      codes.push_back(0xb0);
      break;
    case 'D':
      codes.push_back(0xfe);
      codes.push_back(0xaf);
      break;
    case 'F':
      codes.push_back(0xfe);
      codes.push_back(0xae);
      break;
    case 'J':
      codes.push_back(0xfe);
      codes.push_back(0xad);
      break;
    case 'Z':
    case 'B':
    case 'C':
    case 'S':
    case 'I':
      codes.push_back(0xfe);
      codes.push_back(0xac);
      break;
    default:
      break;
  }
  //codes.push_back(0xfe);
  //codes.push_back(0xb1);
}


MethodDescriptor::MethodDescriptor(const std::string& descriptor) {
  parseMethodDescriptor(descriptor);
}
std::string MethodDescriptor::getReturnType() {
  return mReturnType;
}
const std::vector<std::string>& MethodDescriptor::getParameterTypes() {
  return mParameterTypes;
}
void MethodDescriptor::parseMethodDescriptor(const std::string& descriptor) {
  int begin = descriptor.find('(') + 1;
  int end = descriptor.find(')');
  std::string paramStr = descriptor.substr(begin, end - begin);
  bool isArray = false;
  while (paramStr.size() > 0) {
    std::string paramType;
    if (paramStr[0] == 'L') {
      int semicolonIndex = paramStr.find(';');
      paramType = paramStr.substr(0, semicolonIndex + 1);
      if (isArray) {
        //paramType = paramType.substr(1);
        paramType = "[" + paramType;
        isArray = false;
      }
      paramStr = paramStr.substr(semicolonIndex + 1);
    } else if (paramStr[0] == '[') {
      isArray = true;
      paramStr = paramStr.substr(1);
      continue;
    } else {
      paramType = paramStr[0];
      if (isArray) {
        paramType = "[" + paramType;
        isArray = false;
      }
      paramStr = paramStr.substr(1);
    }
    mParameterTypes.push_back(paramType);
  }
  mReturnType = descriptor.substr(end + 1);
}

} // namespace rtda