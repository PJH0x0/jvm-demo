#pragma once

#include <memory>
#include <runtime/frame.h>
#include <string>
#include <unordered_map>
#include "oo/class.h"
#include "oo/object.h"
namespace runtime {
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