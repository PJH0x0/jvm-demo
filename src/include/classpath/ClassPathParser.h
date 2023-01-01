#pragma once

#include "ClassReader.h"
#include <memory>
#include <string>
using std::string;
namespace classpath {
class ClassPathParser {
  private:
  string jrePathOption;
  string userClassPathOption;
  std::shared_ptr<ClassReader> bootClassReader;
  std::shared_ptr<ClassReader> extClassReader;
  std::shared_ptr<ClassReader> userClassReader;
  void parseBootAndExtClassPath(const string& jrePath);
  void parseUserClassPath(const string& cpOption);
  void parse(const string& jrePathOption, const string& userClassPathOption);

  public:
  explicit ClassPathParser(string _jrePathOption, string _userClassPathOption)
    : jrePathOption(_jrePathOption), userClassPathOption(_userClassPathOption) {
      parse(jrePathOption, userClassPathOption);
    }
  std::shared_ptr<ClassData> readClass(const string& className);
};
string getJreDir(const string& jreOption);
}
