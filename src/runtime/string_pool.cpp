#include "string_pool.h"
#include "oo/class.h"
#include "constant_pool.h"
#include <string>

namespace runtime {
std::unordered_map<std::string, Object*> StringPool::mStringPool;
Object* StringPool::internString(Object* jStr) {
  std::string str = javaStringToString(jStr);
  auto internedStr = mStringPool.find(str);
  if (internedStr != mStringPool.end()) {
    return internedStr->second;
  }
  mStringPool[str] = jStr;
  return jStr;
}
std::string StringPool::javaStringToString(Object* jStr) {
  // auto charArr = jStr->getRefVar("value", "[C");
  // const char16_t* u16Chars = charArr->getArray<char16_t>();
  //return runtime::StringConstant::utf16ToUtf8(u16Chars);
  std::string str = "hello";
  return str;
}

} // namespace runtime