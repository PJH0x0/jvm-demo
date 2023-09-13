#pragma once

#include "class_reader.h"
#include <memory>
#include <string>
using std::string;
namespace classpath {
class ClassPathParser {
  private:
  string jre_path_option_;
  string user_class_path_option_;
  std::shared_ptr<ClassReader> boot_class_reader_;
  std::shared_ptr<ClassReader> ext_class_reader_;
  std::shared_ptr<ClassReader> user_class_reader_;
  void ParseBootAndExtClassPath(const string& jre_path);
  void ParseUserClassPath(const string& cp_option);
  void Parse(const string& jre_path_option, const string& user_class_path_option);

  public:
  explicit ClassPathParser(string jre_path_option, string user_class_path_option)
    : jre_path_option_(jre_path_option), user_class_path_option_(user_class_path_option) {
    Parse(jre_path_option_, user_class_path_option_);
    }
  std::shared_ptr<ClassData> ReadClass(const string& class_name);
};
string GetJreDir(const string& jre_option);
}
