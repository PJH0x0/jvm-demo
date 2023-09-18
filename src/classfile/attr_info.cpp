#include "attr_info.h"
namespace classfile {
UnparsedAttributeInfo::UnparsedAttributeInfo(string name, u4 len) : name_(name), len_(len), info_(nullptr){}
UnparsedAttributeInfo::~UnparsedAttributeInfo() {
  if (info_) {
    free(info_);
    info_ = nullptr;
  }
}
void UnparsedAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) {
  info_ = ParseBytes(class_data, pos, len_);
}

const string& UnparsedAttributeInfo::GetName() const {
  return name_;
}

u4 UnparsedAttributeInfo::GetLen() const {
  return len_;
}

u1* UnparsedAttributeInfo::GetInfo() const {
  return info_;
}

void SourceFileAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> class_data, int &pos) {
  ParseUnsignedInt(class_data, pos, source_file_index_);
}

u2 SourceFileAttributeInfo::GetSourceFileIndex() const {
  return source_file_index_;
}

void ConstantValueAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> class_data, int &pos) {
  ParseUnsignedInt(class_data, pos, constant_value_index_);
}

u2 ConstantValueAttributeInfo::GetConstantValueIndex() const {
  return constant_value_index_;
}

void CodeAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> class_data, int &pos) {
  ParseUnsignedInt(class_data, pos, max_operand_stack_);
  ParseUnsignedInt(class_data, pos, max_locals_);
  ParseUnsignedInt(class_data, pos, code_len_);
  u1* _code = ParseBytes(class_data, pos, code_len_);
  codes_.resize(code_len_);
  for (u4 i = 0; i < code_len_; i++) {
    codes_[i] = _code[i];
  }
  // int size = codes_.size();
  // LOG(INFO) << "codes_ size = " << codes_.size();
  // for (u4 i = 0; i < size; i++) {
  //   LOG(INFO) << "codes_[" << i << "] = " << std::hex << (int)codes_[i];
  // }

  ParseExceptionTable(class_data, pos, exception_tables_);
  //ParseAttributeInfos(class_data, cp_, attributes_, pos);
  u2 attribute_count = 0;
  ParseUnsignedInt(class_data, pos, attribute_count);
  for (u2 i = 0; i < attribute_count; i++) {
    attributes_.push_back(ParseAttributeInfo(class_data, cp_, pos));
  }
}

void CodeAttributeInfo::ParseExceptionTable(std::shared_ptr<ClassData> class_data, int& pos, std::vector<std::shared_ptr<ExceptionTable>>& exception_tables) {
  u2 len = 0;
  ParseUnsignedInt(class_data, pos, len);
  for (u2 i = 0; i < len; i++) {
    std::shared_ptr<ExceptionTable> exceptionTable = std::make_shared<ExceptionTable>();
    ParseUnsignedInt(class_data, pos, exceptionTable->start_pc);
    ParseUnsignedInt(class_data, pos, exceptionTable->end_pc);
    ParseUnsignedInt(class_data, pos, exceptionTable->handler_pc);
    ParseUnsignedInt(class_data, pos, exceptionTable->catch_type);
    exception_tables.push_back(exceptionTable);
  }
}

u2 CodeAttributeInfo::GetMaxOperandStack() const {
  return max_operand_stack_;
}

u2 CodeAttributeInfo::GetMaxLocals() const {
  return max_locals_;
}

u4 CodeAttributeInfo::GetCodeLen() const {
  return code_len_;
}

const std::vector<u1>& CodeAttributeInfo::GetCodes() const {
  return codes_;
}

const std::vector<std::shared_ptr<ExceptionTable>>& CodeAttributeInfo::GetExceptionTables() const {
  return exception_tables_;
}

const std::vector<std::shared_ptr<AttributeInfo>>& CodeAttributeInfo::GetAttributes() const {
  return attributes_;
}

void ExceptionsAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> class_data, int &pos) {
  u2 len = 0;
  ParseUnsignedInt(class_data, pos, len);
  u2 exIndex = 0;
  for (u2 i = 0; i < len; i++) {
    ParseUnsignedInt(class_data, pos, exIndex);
    exception_index_tables_.push_back(exIndex);
  }
}

const std::vector<u2>& ExceptionsAttributeInfo::GetExceptionIndexTables() const {
  return exception_index_tables_;
}

void LineNumberTableAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> class_data, int &pos) {
  u2 lineNumberTableLength = 0;
  ParseUnsignedInt(class_data, pos, lineNumberTableLength);
  for (u2 i = 0; i < lineNumberTableLength; i++) {
    std::shared_ptr<LineNumberTableEntry> entry = std::make_shared<LineNumberTableEntry>();
    ParseUnsignedInt(class_data, pos, entry->start_pc);
    ParseUnsignedInt(class_data, pos, entry->line_number);
    line_number_table_.push_back(entry);
  }
}

const std::vector<std::shared_ptr<LineNumberTableEntry>>& LineNumberTableAttributeInfo::GetLineNumberTable() const {
  return line_number_table_;
}

void LocalVariableTableAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> class_data, int &pos) {
  u2 localVariableTableLength = 0;
  ParseUnsignedInt(class_data, pos, localVariableTableLength);
  for (u2 i = 0; i < localVariableTableLength; i++) {
    std::shared_ptr<LocalVariableTableEntry> entry = std::make_shared<LocalVariableTableEntry>();
    ParseUnsignedInt(class_data, pos, entry->start_pc);
    ParseUnsignedInt(class_data, pos, entry->length);
    ParseUnsignedInt(class_data, pos, entry->name_index);
    ParseUnsignedInt(class_data, pos, entry->descriptor_index);
    ParseUnsignedInt(class_data, pos, entry->index);
    local_variable_table_.push_back(entry);
  }
}

const std::vector<std::shared_ptr<LocalVariableTableEntry>>&
LocalVariableTableAttributeInfo::GetLocalVariableTable() const {
  return local_variable_table_;
}

}