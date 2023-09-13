#pragma once
#include "class_parser.h"
#include <memory>
#include <string>
#include <vector>

namespace classfile {
class AttributeInfo;
class CodeAttributeInfo;
class ConstantValueAttributeInfo;
struct MemberInfo {
  u2 access_flags_;
  u2 name_index_;
  u2 descriptor_index_;
  std::vector<std::shared_ptr<AttributeInfo>> attributes_;
  std::shared_ptr<ConstantPool> constant_pool_;
  MemberInfo(std::shared_ptr<ConstantPool> cp) : constant_pool_(cp) {}
  const std::shared_ptr<CodeAttributeInfo> GetCodeAttribute() const;
  const std::shared_ptr<ConstantValueAttributeInfo> GetConstantAttribute() const;
  const std::string GetName() const;
  const std::string GetDescriptor() const;
};

}