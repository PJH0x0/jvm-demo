#include "attr_info.h"
namespace classfile {
UnparsedAttributeInfo::UnparsedAttributeInfo(string _name, u4 _len) : name(_name), len(_len), info(nullptr){}
UnparsedAttributeInfo::~UnparsedAttributeInfo() {
  if (info) {
    free(info);
    info = nullptr;
  }
}
void UnparsedAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) {
  info = ParseBytes(classData, pos, len);
}

void SourceFileAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> classData, int &pos) {
    ParseUint(classData, pos, sourceFileIndex);
}
void ConstantValueAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> classData, int &pos) {
    ParseUint(classData, pos, constantValueIndex);
}
void CodeAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> classData, int &pos) {
    ParseUint(classData, pos, maxOperandStack);
    ParseUint(classData, pos, maxLocals);
    ParseUint(classData, pos, codeLen);
  u1* _code = ParseBytes(classData, pos, codeLen);
  codes.resize(codeLen);
  for (u4 i = 0; i < codeLen; i++) {
    codes[i] = _code[i];
  }
  // int size = codes.size();
  // LOG(INFO) << "codes size = " << codes.size();
  // for (u4 i = 0; i < size; i++) {
  //   LOG(INFO) << "codes[" << i << "] = " << std::hex << (int)codes[i];
  // }
  
  parseExceptionTable(classData, pos, exceptionTables);
    ParseAttributeInfos(classData, cp, attributes, pos);
}

void CodeAttributeInfo::parseExceptionTable(std::shared_ptr<ClassData> classData, int& pos, std::vector<std::shared_ptr<ExceptionTable>>& exceptionTables) {
  u2 len = 0;
    ParseUint(classData, pos, len);
  for (u2 i = 0; i < len; i++) {
    std::shared_ptr<ExceptionTable> exceptionTable = std::make_shared<ExceptionTable>();
      ParseUint(classData, pos, exceptionTable->startPc);
      ParseUint(classData, pos, exceptionTable->endPc);
      ParseUint(classData, pos, exceptionTable->handlerPc);
      ParseUint(classData, pos, exceptionTable->catchType);
    exceptionTables.push_back(exceptionTable);
  }
}

void ExceptionsAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> classData, int &pos) {
  u2 len = 0;
    ParseUint(classData, pos, len);
  u2 exIndex = 0;
  for (u2 i = 0; i < len; i++) {
      ParseUint(classData, pos, exIndex);
    exceptionIndexTables.push_back(exIndex);
  }
}

void LineNumberTableAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> classData, int &pos) {
  u2 lineNumberTableLength = 0;
    ParseUint(classData, pos, lineNumberTableLength);
  for (u2 i = 0; i < lineNumberTableLength; i++) {
    std::shared_ptr<LineNumberTableEntry> entry = std::make_shared<LineNumberTableEntry>();
      ParseUint(classData, pos, entry->startPc);
      ParseUint(classData, pos, entry->lineNumber);
    lineNumberTable.push_back(entry);
  }
}

void LocalVariableTableAttributeInfo::ParseAttrInfo(std::shared_ptr<ClassData> classData, int &pos) {
  u2 localVariableTableLength = 0;
    ParseUint(classData, pos, localVariableTableLength);
  for (u2 i = 0; i < localVariableTableLength; i++) {
    std::shared_ptr<LocalVariableTableEntry> entry = std::make_shared<LocalVariableTableEntry>();
      ParseUint(classData, pos, entry->startPc);
      ParseUint(classData, pos, entry->length);
      ParseUint(classData, pos, entry->nameIndex);
      ParseUint(classData, pos, entry->descriptorIndex);
      ParseUint(classData, pos, entry->index);
    localVariableTable.push_back(entry);
  }
}

}