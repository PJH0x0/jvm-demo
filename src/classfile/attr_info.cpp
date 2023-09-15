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

void SourceFileAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> class_data, int &pos) {
  ParseUnsignedInt(class_data, pos, source_file_index_);
}
void ConstantValueAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> class_data, int &pos) {
  ParseUnsignedInt(class_data, pos, constant_value_index_);
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
    ParseUnsignedInt(class_data, pos, exceptionTable->start_pc_);
    ParseUnsignedInt(class_data, pos, exceptionTable->end_pc_);
    ParseUnsignedInt(class_data, pos, exceptionTable->handler_pc_);
    ParseUnsignedInt(class_data, pos, exceptionTable->catch_type_);
    exception_tables.push_back(exceptionTable);
  }
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

void LineNumberTableAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> class_data, int &pos) {
  u2 lineNumberTableLength = 0;
  ParseUnsignedInt(class_data, pos, lineNumberTableLength);
  for (u2 i = 0; i < lineNumberTableLength; i++) {
    std::shared_ptr<LineNumberTableEntry> entry = std::make_shared<LineNumberTableEntry>();
    ParseUnsignedInt(class_data, pos, entry->start_pc_);
    ParseUnsignedInt(class_data, pos, entry->line_number_);
    line_number_table_.push_back(entry);
  }
}

void LocalVariableTableAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> class_data, int &pos) {
  u2 localVariableTableLength = 0;
  ParseUnsignedInt(class_data, pos, localVariableTableLength);
  for (u2 i = 0; i < localVariableTableLength; i++) {
    std::shared_ptr<LocalVariableTableEntry> entry = std::make_shared<LocalVariableTableEntry>();
    ParseUnsignedInt(class_data, pos, entry->start_pc_);
    ParseUnsignedInt(class_data, pos, entry->length_);
    ParseUnsignedInt(class_data, pos, entry->name_index_);
    ParseUnsignedInt(class_data, pos, entry->descriptor_index_);
    ParseUnsignedInt(class_data, pos, entry->index_);
    local_variable_table_.push_back(entry);
  }
}

}