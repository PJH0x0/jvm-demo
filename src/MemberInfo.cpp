#include "include/MemberInfo.h"
#include "include/ClassParser.h"
#include <glog/logging.h>
#include <memory>
#include <vector>

namespace JVM {
void parseMembers(std::shared_ptr<ClassData> data, int& pos, std::vector<std::shared_ptr<MemberInfo>>& memberInfos) {
  u2 count = 0;
  parseUint(data, pos, count);
  LOG(INFO) << "Member counts = " << count;
  for (u2 i = 0; i < count; i++) {
    memberInfos.push_back(parseMember(data, pos));
  }
}
std::shared_ptr<MemberInfo> parseMember(std::shared_ptr<ClassData> data, int& pos) {
  std::shared_ptr<MemberInfo> memberInfo = std::make_shared<MemberInfo>();
  parseUint(data, pos, memberInfo->accessFlags);
  parseUint(data, pos, memberInfo->nameIndex);
  parseUint(data, pos, memberInfo->descriptorIndex);
  //parseAttributes(data, pos, memberInfo->attributes);
}
}
