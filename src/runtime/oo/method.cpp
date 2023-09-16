#include "method.h"
#include <runtime/constant_pool.h>
#include <vector>
#include <string>

namespace runtime {
static std::shared_ptr<ClassRefConstant> getCatchType(std::shared_ptr<ConstantPool> cp, 
                                                      uint16_t catch_type_idx) {
  if (catch_type_idx == 0) {
    return nullptr;
  }
  return std::dynamic_pointer_cast<ClassRefConstant>(cp->GetConstant(catch_type_idx));
}
Method::Method(std::shared_ptr<classfile::MemberInfo> cf_method, Class* class_ptr) :
    ClassMember(cf_method, class_ptr), arg_slot_count_(0), max_stack_(0), max_locals_(0) {
  std::shared_ptr<classfile::CodeAttributeInfo> code_attr = cf_method->GetCodeAttribute();
  //Native method has no codes_
  if (code_attr != nullptr) {
    max_stack_ = code_attr->GetMaxOperandStack();
    max_locals_ = code_attr->GetMaxLocals();
    codes_ = code_attr->GetCodes();
    size_t size = code_attr->GetExceptionTables().size();
    for (int32_t i = 0; i < size; i++) {
      std::shared_ptr<ClassRefConstant> catch_type = getCatchType(class_ptr->GetConstantPool(),
                                                                  code_attr->GetExceptionTables()[i]->catch_type);
      ExceptionHandler exception_handler = ExceptionHandler(code_attr->GetExceptionTables()[i]->start_pc,
                                                            code_attr->GetExceptionTables()[i]->end_pc,
                                                            code_attr->GetExceptionTables()[i]->handler_pc,
                                                            catch_type);
      exception_table_.push_back(exception_handler);
    }
    for (const auto& attr : code_attr->GetAttributes()) {
      line_number_table_ = std::dynamic_pointer_cast<classfile::LineNumberTableAttributeInfo>(attr);
      if (line_number_table_ != nullptr) {
        break;
      }
    }
  }
  method_descriptor_ = std::make_shared<MethodDescriptor>(descriptor_);
  CalcArgSlotCount(method_descriptor_->GetParameterTypes());

  if (IsNative()) {
    InjectCodeAttribute(method_descriptor_->GetReturnType());
  }
}
void Method::CalcArgSlotCount(const std::vector<std::string>* paramTypes) {
  for (auto& paramType : *paramTypes) {
    arg_slot_count_++;
    if (paramType == "J" || paramType == "D") {
      arg_slot_count_++;
    }
  }
  if (!IsStatic()) {
    arg_slot_count_++;
  }
}

void Method::InjectCodeAttribute(std::string returnType) {
  max_stack_ = 4;
  max_locals_ = arg_slot_count_;
  switch (returnType[0]) {
    case 'V':
      codes_->push_back(0xfe);
      codes_->push_back(0xb1);
      break;
    case 'L':
    case '[':
      codes_->push_back(0xfe);
      codes_->push_back(0xb0);
      break;
    case 'D':
      codes_->push_back(0xfe);
      codes_->push_back(0xaf);
      break;
    case 'F':
      codes_->push_back(0xfe);
      codes_->push_back(0xae);
      break;
    case 'J':
      codes_->push_back(0xfe);
      codes_->push_back(0xad);
      break;
    case 'Z':
    case 'B':
    case 'C':
    case 'S':
    case 'I':
      codes_->push_back(0xfe);
      codes_->push_back(0xac);
      break;
    default:
      break;
  }
  //codes_.push_back(0xfe);
  //codes_.push_back(0xb1);
}

int32_t Method::FindExceptionHandler(Class* exClass, int32_t pc) {
  for (auto handler : *exception_table_) {
    if (pc >= handler.GetStartPc() && pc < handler.GetEndPc()) {
      if (handler.GetCatchType() == nullptr) {
        return handler.GetHandlerPc();
      }
      Class* catch_class = handler.GetCatchType()->ResolveClass();
      if (catch_class == exClass || Class::IsSuperClassOf(catch_class, exClass)) {
        return handler.GetHandlerPc();
      }
    }
  }
  return -1;
}

int32_t Method::GetLineNumber(int32_t pc) {
  if (IsNative()) {
    return -2;
  }
  if (line_number_table_ == nullptr) {
    return -1;
  }
  for (auto entry : line_number_table_->GetLineNumberTable()) {
    if (pc >= entry->start_pc) {
      return (int32_t)entry->line_number;
    }
  }
  return -1;
}


MethodDescriptor::MethodDescriptor(const std::string& descriptor) {
  ParseMethodDescriptor(descriptor);
}
std::string MethodDescriptor::GetReturnType() const {
  return return_type_;
}
const std::vector<std::string>& MethodDescriptor::GetParameterTypes() const {
  return parameter_types_;
}
void MethodDescriptor::ParseMethodDescriptor(const std::string& descriptor) {
  int begin = descriptor.find('(') + 1;
  int end = descriptor.find(')');
  std::string param_str = descriptor.substr(begin, end - begin);
  bool is_array = false;
  while (param_str.size() > 0) {
    std::string param_type;
    if (param_str[0] == 'L') {
      int semicolon_index = param_str.find(';');
      param_type = param_str.substr(0, semicolon_index + 1);
      if (is_array) {
        //param_type = param_type.substr(1);
        param_type = "[" + param_type;
        is_array = false;
      }
      param_str = param_str.substr(semicolon_index + 1);
    } else if (param_str[0] == '[') {
      is_array = true;
      param_str = param_str.substr(1);
      continue;
    } else {
      param_type = param_str[0];
      if (is_array) {
        param_type = "[" + param_type;
        is_array = false;
      }
      param_str = param_str.substr(1);
    }
    parameter_types_.push_back(param_type);
  }
  return_type_ = descriptor.substr(end + 1);
}

} // namespace runtime