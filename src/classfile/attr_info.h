#pragma once
#include "class_parser.h"
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>
#include <glog/logging.h>

namespace classfile {
class AttributeInfo {
public:
  virtual void ParseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) = 0;
};

class UnparsedAttributeInfo : public AttributeInfo {
public:
  UnparsedAttributeInfo(std::string name, u4 len);
  const string& GetName() const;
  u4 GetLen() const;
  u1* GetInfo() const;
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override;
  ~UnparsedAttributeInfo();

private:
  std::string name_;
  u4 len_;
  u1* info_;
};

class DeprecatedAttributeInfo : public AttributeInfo {
public:
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override {}
};

class SyntheticAttributeInfo : public AttributeInfo {
public:
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override {}
};

class SourceFileAttributeInfo : public AttributeInfo {
public:
  SourceFileAttributeInfo() : source_file_index_(0) {}
  u2 GetSourceFileIndex() const;
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override;

private:
  u2 source_file_index_;
public:
};

class ConstantValueAttributeInfo : public AttributeInfo {
public:
  ConstantValueAttributeInfo() : constant_value_index_(0) {}
  u2 GetConstantValueIndex() const;
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override;

private:
  u2 constant_value_index_;
};

struct ExceptionTable {
  u2 start_pc;
  u2 end_pc;
  u2 handler_pc;
  u2 catch_type;
};

class CodeAttributeInfo : public AttributeInfo {
public:
  explicit CodeAttributeInfo(std::shared_ptr<ConstantPool> cp) : cp_(cp),
                                                                 max_locals_(0),
                                                                 max_operand_stack_(0),
                                                                 code_len_(0) {}
  u2 GetMaxOperandStack() const;
  u2 GetMaxLocals() const;
  u4 GetCodeLen() const;
  const std::vector<u1>& GetCodes() const;
  const std::vector<std::shared_ptr<ExceptionTable>>& GetExceptionTables() const;
  const std::vector<std::shared_ptr<AttributeInfo>>& GetAttributes() const;
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override;
  void ParseExceptionTable(std::shared_ptr<ClassData> class_data, int& pos, std::vector<std::shared_ptr<ExceptionTable>>& exception_tables);

private:
  u2 max_operand_stack_;
  u2 max_locals_;
  u4 code_len_;
  std::vector<u1> codes_;
  std::vector<std::shared_ptr<ExceptionTable>> exception_tables_;
  std::vector<std::shared_ptr<AttributeInfo>> attributes_;
  std::shared_ptr<ConstantPool> cp_;
};

class ExceptionsAttributeInfo : public AttributeInfo {
public:
  const std::vector<u2>& GetExceptionIndexTables() const;
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override;

private:
  std::vector<u2> exception_index_tables_;
};

struct LineNumberTableEntry {
  u2 start_pc;
  u2 line_number;
};

class LineNumberTableAttributeInfo : public AttributeInfo {
public:
  const std::vector<std::shared_ptr<LineNumberTableEntry>>& GetLineNumberTable() const;
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override;

private:
  std::vector<std::shared_ptr<LineNumberTableEntry>> line_number_table_;
};

struct LocalVariableTableEntry {
  u2 start_pc;
  u2 length;
  u2 name_index;
  u2 descriptor_index;
  u2 index;
};

class LocalVariableTableAttributeInfo : public AttributeInfo {
public:
  const std::vector<std::shared_ptr<LocalVariableTableEntry>>& GetLocalVariableTable() const;
  void ParseAttrInfo(std::shared_ptr<ClassData> class_data, int& pos) override;

private:
  std::vector<std::shared_ptr<LocalVariableTableEntry>> local_variable_table_;
};
}