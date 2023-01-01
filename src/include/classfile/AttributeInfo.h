#pragma once
#include "ClassParser.h"
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

namespace classfile {
struct AttributeInfo {
  virtual void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) = 0;
};

struct UnparsedAttributeInfo : public AttributeInfo {
  string name;
  u4 len;
  u1* info;
  UnparsedAttributeInfo(string _name, u4 _len) : name(_name), len(_len), info(nullptr){}
  ~UnparsedAttributeInfo() {
    if (info) {
      free(info);
      info = nullptr;
    }
  }
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    info = parseBytes(classData, pos, len);
  }
};
struct DeprecatedAttributeInfo : public AttributeInfo {
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override {}
};
struct SyntheticAttributeInfo : public AttributeInfo {
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override {}
};
struct SourceFileAttributeInfo : public AttributeInfo {
  u2 sourceFileIndex;
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    parseUint(classData, pos, sourceFileIndex);
  }
};
struct ConstantValueAttributeInfo : public AttributeInfo {
  u2 constantValueIndex;
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    parseUint(classData, pos, constantValueIndex);
  }
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
  u1* code;
  std::vector<std::shared_ptr<ExceptionTable>> exceptionTables;
  std::vector<std::shared_ptr<AttributeInfo>> attributes;
  std::shared_ptr<ConstantPool> cp;
  CodeAttributeInfo(std::shared_ptr<ConstantPool> _cp) : cp(_cp), code(nullptr) {}
  
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    parseUint(classData, pos, maxOperandStack);
    parseUint(classData, pos, maxLocals);
    parseUint(classData, pos, codeLen);
    code = parseBytes(classData, pos, codeLen);
    parseExceptionTable(classData, pos, exceptionTables);
    parseAttributeInfos(classData, cp, attributes, pos);
  }
  
  void parseExceptionTable(std::shared_ptr<ClassData> classData, int& pos, std::vector<std::shared_ptr<ExceptionTable>>& exceptionTables) {
    u2 len = 0;
    parseUint(classData, pos, len);
    for (u2 i = 0; i < len; i++) {
      std::shared_ptr<ExceptionTable> exceptionTable = std::make_shared<ExceptionTable>();
      parseUint(classData, pos, exceptionTable->startPc);
      parseUint(classData, pos, exceptionTable->endPc);
      parseUint(classData, pos, exceptionTable->handlerPc);
      parseUint(classData, pos, exceptionTable->catchType);
      exceptionTables.push_back(exceptionTable);
    }
  }
  ~CodeAttributeInfo() {
    if (code) {
      free(code);
      code = nullptr;
    }
  }
};
struct ExceptionsAttributeInfo : public AttributeInfo {
  std::vector<u2> exceptionIndexTables;
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    u2 len = 0;
    parseUint(classData, pos, len);
    u2 exIndex = 0;
    for (u2 i = 0; i < len; i++) {
      parseUint(classData, pos, exIndex);
      exceptionIndexTables.push_back(exIndex);
    }
  }
};
struct LineNumberTableEntry {
  u2 startPc;
  u2 lineNumber;
};
struct LineNumberTableAttributeInfo : public AttributeInfo {
  std::vector<std::shared_ptr<LineNumberTableEntry>> lineNumberTable;
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    u2 lineNumberTableLength = 0;
    parseUint(classData, pos, lineNumberTableLength);
    for (u2 i = 0; i < lineNumberTableLength; i++) {
      std::shared_ptr<LineNumberTableEntry> entry = std::make_shared<LineNumberTableEntry>();
      parseUint(classData, pos, entry->startPc);
      parseUint(classData, pos, entry->lineNumber);
      lineNumberTable.push_back(entry);
    }
  }
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
  void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) override {
    u2 localVariableTableLength = 0;
    parseUint(classData, pos, localVariableTableLength);
    for (u2 i = 0; i < localVariableTableLength; i++) {
      std::shared_ptr<LocalVariableTableEntry> entry = std::make_shared<LocalVariableTableEntry>();
      parseUint(classData, pos, entry->startPc);
      parseUint(classData, pos, entry->length);
      parseUint(classData, pos, entry->nameIndex);
      parseUint(classData, pos, entry->descriptorIndex);
      parseUint(classData, pos, entry->index);
      localVariableTable.push_back(entry);
    }
  }
};
}