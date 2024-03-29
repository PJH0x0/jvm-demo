#pragma once

#include "class_reader.h"
#include <memory>
#include <string>
using std::string;
namespace classpath {
class ClassPathParser {
private:
  static std::shared_ptr<ClassPathParser> instance_;
  string jre_path_option_;
  string app_class_path_option_;
  std::shared_ptr<ClassReader> boot_class_reader_;
  std::shared_ptr<ClassReader> ext_class_reader_;
  std::shared_ptr<ClassReader> app_class_reader_;
  void ParseBootAndExtClassPath(const string& jre_path);
  void ParseUserClassPath(const string& cp_option);
  void Parse(const string& jre_path_option, const string& app_class_path_option);
  ClassPathParser(){}

public:
  ClassPathParser(const string& jre_path_option, const string& app_class_path_option)
      : jre_path_option_(jre_path_option), app_class_path_option_(app_class_path_option) {
    Parse(jre_path_option_, app_class_path_option_);
  }
  std::shared_ptr<ClassData> ReadClass(const string& class_name);
  std::shared_ptr<ClassReader> GetBootClassReader();
  std::shared_ptr<ClassReader> GetExtClassReader();
  std::shared_ptr<ClassReader> GetAppClassReader();
};
string GetJreDir(const string& jre_option);
}
