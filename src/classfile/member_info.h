#pragma once
#include "class_parser.h"
#include <memory>
#include <string>
#include <vector>

namespace classfile {
class AttributeInfo;
class CodeAttributeInfo;
struct MemberInfo {
  u2 accessFlags;
  u2 nameIndex;
  u2 descriptorIndex;
  std::vector<std::shared_ptr<AttributeInfo>> attributes;
  std::shared_ptr<ConstantPool> cp;
  MemberInfo(std::shared_ptr<ConstantPool> cp_) : cp(cp_) {}
  const std::shared_ptr<CodeAttributeInfo> getCodeAttribute() const;
  const std::string getName() const;
  const std::string getDescriptor() const;
};

}