#pragma once
#include "ClassParser.h"

namespace JVM {
struct AttributeInfo {
  virtual void parseAttrInfo(std::shared_ptr<ClassData> classData, int& pos) = 0;
};
}