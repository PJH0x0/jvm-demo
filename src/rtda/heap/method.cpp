#include "method.h"
#include "constant_pool.h"
#include <vector>
#include <string>

namespace rtda {
static std::shared_ptr<ClassRefConstant> getCatchType(std::shared_ptr<ConstantPool> cp, 
                                                      uint16_t catchTypeIdx) {
  if (catchTypeIdx == 0) {
    return nullptr;
  }
  return std::dynamic_pointer_cast<ClassRefConstant>(cp->getConstant(catchTypeIdx));
}
Method::Method(std::shared_ptr<classfile::MemberInfo> cfMethod, std::shared_ptr<Class> classPtr) :
  ClassMember(cfMethod, classPtr), mArgSlotCount(0), maxStack(0), maxLocals(0) {
  std::shared_ptr<classfile::CodeAttributeInfo> codeAttr = cfMethod->getCodeAttribute();
  //Native method has no codes
  if (codeAttr != nullptr) {
    maxStack = codeAttr->maxOperandStack;
    maxLocals = codeAttr->maxLocals;
    codes = codeAttr->codes;
    size_t size = codeAttr->exceptionTables.size();
    for (int32_t i = 0; i < size; i++) {
      std::shared_ptr<ClassRefConstant> catchType = getCatchType(classPtr->getConstantPool(), 
                                                                 codeAttr->exceptionTables[i]->catchType);
      ExceptionHandler exceptionHandler = ExceptionHandler(codeAttr->exceptionTables[i]->startPc,
                                                           codeAttr->exceptionTables[i]->endPc,
                                                           codeAttr->exceptionTables[i]->handlerPc,
                                                           catchType);
      mExceptionTable.push_back(exceptionHandler);
    }
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

int32_t Method::findExceptionHandler(std::shared_ptr<Class> exClass, int32_t pc) {
  for (int32_t i = 0; i < mExceptionTable.size(); i++) {
    ExceptionHandler handler = mExceptionTable[i];
    if (pc >= handler.getStartPc() && pc < handler.getEndPc()) {
      if (handler.getCatchType() == nullptr) {
        return handler.getHandlerPc();
      }
      std::shared_ptr<Class> catchClass = handler.getCatchType()->resolveClass();
      if (catchClass == exClass || Class::isSuperClassOf(catchClass, exClass)) {
        return handler.getHandlerPc();
      }
    }
  }
  return -1;
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