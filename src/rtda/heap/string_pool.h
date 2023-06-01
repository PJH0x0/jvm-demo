#pragma once

#include <memory>
#include <rtda/frame.h>
#include <string>
#include <unordered_map>
#include "class.h"
#include "object.h"
namespace rtda {
class StringPool {
  private:
  static std::unordered_map<std::string, Object*> mStringPool;
  friend class Class;
  public:
  static std::unordered_map<std::string, Object*>& getStringPool() { return mStringPool; }
  static Object* internString(Object* jStr);
  static std::string javaStringToString(Object* jStr);
};
}