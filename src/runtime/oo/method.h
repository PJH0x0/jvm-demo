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
public:
  Method(std::shared_ptr<classfile::MemberInfo>, Class*);

  const std::vector<u1>* GetCodes() const {
    return codes_;
  }

  uint32_t GetMaxStack() const {
    return max_stack_;
  }
  uint32_t GetMaxLocals() const {
    return max_locals_;
  }
  uint32_t GetArgSlotCount() const {
    return arg_slot_count_;
  }
  bool IsSynchronized() const {
    return (access_flags_ & ACC_SYNCHRONIZED) != 0;
  }
  bool IsBridge() const {
    return (access_flags_ & ACC_BRIDGE) != 0;
  }
  bool IsVarargs() const {
    return (access_flags_ & ACC_VARARGS) != 0;
  }
  bool IsNative() const {
    return (access_flags_ & ACC_NATIVE) != 0;
  }
  bool IsAbstract() const {
    return (access_flags_ & ACC_ABSTRACT) != 0;
  }
  bool IsStrict() const {
    return (access_flags_ & ACC_STRICT) != 0;
  }
  bool IsSynthetic() const {
    return (access_flags_ & ACC_SYNTHETIC) != 0;
  }
  void CalcArgSlotCount(const std::vector<std::string>* paramTypes);
  void InjectCodeAttribute(std::string returnType);
  int32_t FindExceptionHandler(Class* exClass, int32_t pc);
  int32_t GetLineNumber(int32_t pc);
private:
  std::vector<u1>* codes_;
  uint32_t max_stack_;
  uint32_t max_locals_;
  uint32_t arg_slot_count_;
  MethodDescriptor* method_descriptor_;
  const ExceptionTable* exception_table_;
  std::shared_ptr<classfile::LineNumberTableAttributeInfo> line_number_table_;
};
class MethodDescriptor {
public:
  explicit MethodDescriptor(const std::string& descriptor);
  const std::vector<std::string>& GetParameterTypes() const;
  std::string GetReturnType() const;
private:
  std::vector<std::string> parameter_types_;
  std::string return_type_;

  void ParseMethodDescriptor(const std::string& descriptor);
  
};

struct ExceptionHandler {
  private:
  int32_t start_pc_;
  int32_t end_pc_;
  int32_t handler_pc_;
  std::shared_ptr<ClassRefConstant> catch_type_;
  public:
  ExceptionHandler(int32_t startPc, int32_t endPc, int32_t handlerPc, std::shared_ptr<ClassRefConstant> catchType) :
      start_pc_(startPc), end_pc_(endPc), handler_pc_(handlerPc), catch_type_(catchType) {};
  int32_t GetStartPc() const {
    return start_pc_;
  }
  int32_t GetEndPc() const {
    return end_pc_;
  }
  int32_t GetHandlerPc() const {
    return handler_pc_;
  }
  std::shared_ptr<ClassRefConstant> GetCatchType() {
    return catch_type_;
  }
};

} // namespace runtime