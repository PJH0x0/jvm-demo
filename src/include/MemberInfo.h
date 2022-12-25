#pragma once
#include "ClassParser.h"
#include <memory>
#include <vector>

namespace JVM {
class AttributeInfo;
class ConstantPool;
struct MemberInfo {
  u2 accessFlags;
  u2 nameIndex;
  u2 descriptorIndex;
  std::vector<std::shared_ptr<AttributeInfo>> attributes;
};
void parseMembers(std::shared_ptr<ClassData> data, int& pos, std::vector<std::shared_ptr<MemberInfo>>& memberInfos);
std::shared_ptr<MemberInfo> parseMember(std::shared_ptr<ClassData> data, int& pos);
}