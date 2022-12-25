#pragma once
#include "ClassParser.h"
#include <memory>
#include <vector>

namespace JVM {
class AttributeInfo;
struct MemberInfo {
  u2 accessFlags;
  u2 nameIndex;
  u2 descriptorIndex;
  std::vector<std::shared_ptr<AttributeInfo>> attributes;
};

}