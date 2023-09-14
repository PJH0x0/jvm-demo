#pragma once
#include "class_parser.h"
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>
#include <glog/logging.h>

namespace classfile {
struct AttributeInfo {
  virtual void ParseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) = 0;
};

struct UnparsedAttributeInfo : public AttributeInfo {
  std::string name_;
  u4 len_;
  u1* info_;
  UnparsedAttributeInfo(std::string name, u4 len);
  ~UnparsedAttributeInfo(); 
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override;
};
struct DeprecatedAttributeInfo : public AttributeInfo {
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override {}
};
struct SyntheticAttributeInfo : public AttributeInfo {
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override {}
};
struct SourceFileAttributeInfo : public AttributeInfo {
  u2 source_file_index_;
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override;
};
struct ConstantValueAttributeInfo : public AttributeInfo {
  u2 constant_value_index_;
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override;
};
struct ExceptionTable {
  u2 start_pc_;
  u2 end_pc_;
  u2 handler_pc_;
  u2 catch_type_;
};
struct CodeAttributeInfo : public AttributeInfo {
  u2 max_operand_stack_;
  u2 max_locals_;
  u4 code_len_;
  std::vector<u1> codes_;
  std::vector<std::shared_ptr<ExceptionTable>> exception_tables_;
  std::vector<std::shared_ptr<AttributeInfo>> attributes_;
  std::shared_ptr<ConstantPool> cp_;
  CodeAttributeInfo(std::shared_ptr<ConstantPool> cp) : cp_(cp) {}
  
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override;
  
  void parseExceptionTable(std::shared_ptr<ClassData> class_data, int& pos, std::vector<std::shared_ptr<ExceptionTable>>& exception_tables);
  
};
struct ExceptionsAttributeInfo : public AttributeInfo {
  std::vector<u2> exception_index_tables_;
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override;
};
struct LineNumberTableEntry {
  u2 start_pc_;
  u2 line_number_;
};
struct LineNumberTableAttributeInfo : public AttributeInfo {
  std::vector<std::shared_ptr<LineNumberTableEntry>> line_number_table_;
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override;
};
struct LocalVariableTableEntry {
  u2 start_pc_;
  u2 length_;
  u2 name_index_;
  u2 descriptor_index_;
  u2 index_;
};
struct LocalVariableTableAttributeInfo : public AttributeInfo {
  std::vector<std::shared_ptr<LocalVariableTableEntry>> local_variable_table_;
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override;
};
}