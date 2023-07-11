#pragma once

#include "class_member.h"
#include <runtime/constant_pool.h>
#include <classfile/attr_info.h>
#include <classfile/member_info.h>
#include <memory>
#include <vector>
#include <string>

namespace runtime {
class MethodDescriptor;
class ExceptionHandler;
class Method : public ClassMember {
  using ExceptionTable = std::vector<ExceptionHandler>;
  typedef classfile::u1 u1;
  private:
  std::vector<u1> codes;
  uint32_t maxStack;
  uint32_t maxLocals;
  uint32_t mArgSlotCount;
  std::shared_ptr<MethodDescriptor> mMethodDescriptor;
  ExceptionTable mExceptionTable;
  std::shared_ptr<classfile::LineNumberTableAttributeInfo> mLineNumberTable;

  public:
  Method(std::shared_ptr<classfile::MemberInfo>, Class*);

  std::shared_ptr<std::vector<u1>> getCodes() {
    return std::make_shared<std::vector<u1>>(codes);
  }

  uint32_t getMaxStack() {
    return maxStack;
  }
  uint32_t getMaxLocals() {
    return maxLocals;
  }
  uint32_t getArgSlotCount() {
    return mArgSlotCount;
  }

  void calcArgSlotCount(const std::vector<std::string>& paramTypes);
  void injectCodeAttribute(std::string returnType);
  int32_t findExceptionHandler(Class* exClass, int32_t pc);
  int32_t getLineNumber(int32_t pc);
  
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
  const std::vector<std::string>& getParameterTypes();
  std::string getReturnType();
  
};

struct ExceptionHandler {
  private:
  int32_t mStartPc;
  int32_t mEndPc;
  int32_t mHandlerPc;
  std::shared_ptr<ClassRefConstant> mCatchType;
  public:
  ExceptionHandler(int32_t startPc, int32_t endPc, int32_t handlerPc, std::shared_ptr<ClassRefConstant> catchType) :
    mStartPc(startPc), mEndPc(endPc), mHandlerPc(handlerPc), mCatchType(catchType) {};
  int32_t getStartPc() {
    return mStartPc;
  }
  int32_t getEndPc() {
    return mEndPc;
  }
  int32_t getHandlerPc() {
    return mHandlerPc;
  }
  std::shared_ptr<ClassRefConstant> getCatchType() {
    return mCatchType;
  }
};

} // namespace runtime