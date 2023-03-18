#include "attr_info.h"
namespace classfile {
UnparsedAttributeInfo::UnparsedAttributeInfo(string _name, u4 _len) : name(_name), len(_len), info(nullptr){}
UnparsedAttributeInfo::~UnparsedAttributeInfo() {
  if (info) {
    free(info);
    info = nullptr;
  }
}
void UnparsedAttributeInfo::parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) {
  info = parseBytes(classData, pos, len);
}

void SourceFileAttributeInfo::parseAttrInfo(std::shared_ptr<ClassData> classData, int &pos) {
  parseUint(classData, pos, sourceFileIndex);
}
void ConstantValueAttributeInfo::parseAttrInfo(std::shared_ptr<ClassData> classData, int &pos) {
  parseUint(classData, pos, constantValueIndex);
}
void CodeAttributeInfo::parseAttrInfo(std::shared_ptr<ClassData> classData, int &pos) {
  parseUint(classData, pos, maxOperandStack);
  parseUint(classData, pos, maxLocals);
  parseUint(classData, pos, codeLen);
  u1* _code = parseBytes(classData, pos, codeLen);
  codes.resize(codeLen);
  for (u4 i = 0; i < codeLen; i++) {
    codes.push_back(_code[i]);
  }
  parseExceptionTable(classData, pos, exceptionTables);
  parseAttributeInfos(classData, cp, attributes, pos);
}

void CodeAttributeInfo::parseExceptionTable(std::shared_ptr<ClassData> classData, int& pos, std::vector<std::shared_ptr<ExceptionTable>>& exceptionTables) {
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

void ExceptionsAttributeInfo::parseAttrInfo(std::shared_ptr<ClassData> classData, int &pos) {
  u2 len = 0;
  parseUint(classData, pos, len);
  u2 exIndex = 0;
  for (u2 i = 0; i < len; i++) {
    parseUint(classData, pos, exIndex);
    exceptionIndexTables.push_back(exIndex);
  }
}

void LineNumberTableAttributeInfo::parseAttrInfo(std::shared_ptr<ClassData> classData, int &pos) {
  u2 lineNumberTableLength = 0;
  parseUint(classData, pos, lineNumberTableLength);
  for (u2 i = 0; i < lineNumberTableLength; i++) {
    std::shared_ptr<LineNumberTableEntry> entry = std::make_shared<LineNumberTableEntry>();
    parseUint(classData, pos, entry->startPc);
    parseUint(classData, pos, entry->lineNumber);
    lineNumberTable.push_back(entry);
  }
}

void LocalVariableTableAttributeInfo::parseAttrInfo(std::shared_ptr<ClassData> classData, int &pos) {
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

}