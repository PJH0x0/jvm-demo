#pragma once
#include "class_parser.h"
#include <memory>
#include <string>
#include <vector>

namespace classfile {
class AttributeInfo;
class CodeAttributeInfo;
class ConstantValueAttributeInfo;
class MemberInfo {
public:
  //MemberInfo(std::shared_ptr<ConstantPool> cp) : constant_pool_(cp) {}
  MemberInfo(u2 access_flags, u2 name_index, u2 descriptor_index,
             const std::shared_ptr<ConstantPool>& constant_pool);
  u2 GetAccessFlags() const;
  u2 GetNameIndex() const;
  u2 GetDescriptorIndex() const;
  std::shared_ptr<CodeAttributeInfo> GetCodeAttribute() const;
  std::shared_ptr<ConstantValueAttributeInfo> GetConstantAttribute() const;
  void PutAttributeInfo(std::shared_ptr<AttributeInfo> attribute_info);
  std::string GetName() const;
  std::string GetDescriptor() const;
private:
  u2 access_flags_;
  u2 name_index_;
  u2 descriptor_index_;
  std::shared_ptr<ConstantPool> constant_pool_;
  std::vector<std::shared_ptr<AttributeInfo>> attributes_;

};

}