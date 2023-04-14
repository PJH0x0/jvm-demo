#pragma once
#include "class_parser.h"
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>
#include <glog/logging.h>

namespace classfile {
struct AttributeInfo {
  virtual void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) = 0;
};

struct UnparsedAttributeInfo : public AttributeInfo {
  std::string name;
  u4 len;
  u1* info;
  UnparsedAttributeInfo(std::string _name, u4 _len);
  ~UnparsedAttributeInfo(); 
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override; 
};
struct DeprecatedAttributeInfo : public AttributeInfo {
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override {}
};
struct SyntheticAttributeInfo : public AttributeInfo {
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override {}
};
struct SourceFileAttributeInfo : public AttributeInfo {
  u2 sourceFileIndex;
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override; 
};
struct ConstantValueAttributeInfo : public AttributeInfo {
  u2 constantValueIndex;
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override;
};
struct ExceptionTable {
  u2 startPc;
  u2 endPc;
  u2 handlerPc;
  u2 catchType;
};
struct CodeAttributeInfo : public AttributeInfo {
  u2 maxOperandStack;
  u2 maxLocals;
  u4 codeLen;
  std::vector<u1> codes;
  std::vector<std::shared_ptr<ExceptionTable>> exceptionTables;
  std::vector<std::shared_ptr<AttributeInfo>> attributes;
  std::shared_ptr<ConstantPool> cp;
  CodeAttributeInfo(std::shared_ptr<ConstantPool> _cp) : cp(_cp) {}
  
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override;
  
  void parseExceptionTable(std::shared_ptr<ClassData> classData, int& pos, std::vector<std::shared_ptr<ExceptionTable>>& exceptionTables);
  
};
struct ExceptionsAttributeInfo : public AttributeInfo {
  std::vector<u2> exceptionIndexTables;
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override;
};
struct LineNumberTableEntry {
  u2 startPc;
  u2 lineNumber;
};
struct LineNumberTableAttributeInfo : public AttributeInfo {
  std::vector<std::shared_ptr<LineNumberTableEntry>> lineNumberTable;
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override;
};
struct LocalVariableTableEntry {
  u2 startPc;
  u2 length;
  u2 nameIndex;
  u2 descriptorIndex;
  u2 index;
};
struct LocalVariableTableAttributeInfo : public AttributeInfo {
  std::vector<std::shared_ptr<LocalVariableTableEntry>> localVariableTable;
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override;
};
}