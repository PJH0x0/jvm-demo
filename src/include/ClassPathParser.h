#pragma once

#include "ClassReader.h"
#include <memory>
#include <string>
using std::string;
namespace JVM {
class ClassPath {
  private:
  std::shared_ptr<ClassReader> bootClassReader;
  std::shared_ptr<ClassReader> extClassReader;
  std::shared_ptr<ClassReader> userClassReader;

  public:
  void parseBootAndExtClassPath(const string& jrePath);
  void parseUserClassPath(const string& cpOption);
  std::shared_ptr<ClassData> readClass(const string& className);
};
string getJreDir(const string& jreOption);
}
