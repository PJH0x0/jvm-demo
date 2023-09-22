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
class LineNumberEntry;
class Method : public ClassMember {
  using ExceptionTable = std::vector<ExceptionHandler>;
  using LineNumberTable = std::vector<LineNumberEntry>;
  typedef classfile::u1 u1;
public:
  Method(const std::shared_ptr<classfile::MemberInfo>&, Class*);

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
  int32_t FindExceptionHandler(Class* exClass, int32_t pc) const;
  int32_t GetLineNumber(int32_t pc);
  ~Method();
private:
  void CalcArgSlotCount(const std::vector<std::string>* paramTypes);
  void InjectCodeAttribute(const std::string& returnType);
  std::vector<u1>* codes_;
  uint32_t max_stack_;
  uint32_t max_locals_;
  uint32_t arg_slot_count_;
  MethodDescriptor* method_descriptor_;
  ExceptionTable* exception_table_;
  LineNumberTable* line_number_table_;
};
class MethodDescriptor {
public:
  explicit MethodDescriptor(const std::string& descriptor);
  const std::vector<std::string>* GetParameterTypes() const;
  std::string GetReturnType() const;
private:
  std::vector<std::string> parameter_types_;
  std::string return_type_;

  void ParseMethodDescriptor(const std::string& descriptor);
  
};

class ExceptionHandler {
public:
  ExceptionHandler() = default;
  ExceptionHandler(int32_t start_pc, int32_t end_pc, int32_t handler_pc, ClassRefConstant* catch_type) :
      start_pc_(start_pc), end_pc_(end_pc), handler_pc_(handler_pc), catch_type_(catch_type) {};
  ExceptionHandler& operator=(const ExceptionHandler&) = default;
  void SetStartPc(int32_t start_pc) {
    start_pc_ = start_pc;
  }
  int32_t GetStartPc() const {
    return start_pc_;
  }
  void SetEndPc(int32_t end_pc) {
    end_pc_ = end_pc;
  }
  int32_t GetEndPc() const {
    return end_pc_;
  }
  void SetHandlerPc(int32_t handler_pc) {
    handler_pc_ = handler_pc;
  }
  int32_t GetHandlerPc() const {
    return handler_pc_;
  }
  void SetCatchType(ClassRefConstant* catch_type) {
    catch_type_ = catch_type;
  }
  ClassRefConstant* GetCatchType() const {
    return catch_type_;
  }
private:
  int32_t start_pc_{0};
  int32_t end_pc_{0};
  int32_t handler_pc_{0};
  ClassRefConstant* catch_type_{nullptr};
};

struct LineNumberEntry {
  uint16_t start_pc;
  uint16_t line_number;
};

} // namespace runtime