#include "class_path_parser.h"
#include "class_reader.h"
#include <cstdlib>
#include <cstring>
#include <limits.h>
#include <iostream>
#include <memory>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <glog/logging.h>
using std::string;


namespace classpath {
#ifdef _POSIX_PATH_MAX
#define PATHNAME_MAX		4096
#else
#define PATHNAME_MAX		1000
#endif
static std::shared_ptr<ClassPathParser> instance_{};
static std::once_flag class_path_once_flag;
string GetJreDir(const string& jre_option) {
  struct stat st;
  int err;
  if (!jre_option.empty()) {
    err = lstat(jre_option.c_str(), &st);
    if (err == 0 && S_ISDIR(st.st_mode)) {
      return jre_option;
    }
  }
  string cur_jre = "./jre";
  err = lstat(cur_jre.c_str(), &st);
  if (err == 0 && S_ISDIR(st.st_mode)) {
    return cur_jre;
  }
  char* java_home = std::getenv("JAVA_HOME");
  if (java_home != nullptr) {
    cur_jre = string(java_home) + "/jre";
    return cur_jre;
  }
  //std::cout << "Can not find jre folder!" << std::endl;
  //abort();
  //throw "Can not find jre folder!";
  LOG(FATAL) << "Can not find jre folder!";
}

void ClassPathParser::Parse(const string& jre_path_option, const string& app_class_path_option) {
  ParseBootAndExtClassPath(jre_path_option);
  ParseUserClassPath(app_class_path_option);
}

void ClassPathParser::ParseBootAndExtClassPath(const string& jre_path) {
  auto jre_dir = GetJreDir(jre_path);
  std::string jre_lib_path = jre_dir + "/lib/*";
  boot_class_reader_ = std::make_shared<WildcardClassReader>(jre_lib_path);
  std::string jre_ext_path = jre_dir + "/lib/ext/*";
  ext_class_reader_ = std::make_shared<WildcardClassReader>(jre_ext_path);
}

void ClassPathParser::ParseUserClassPath(const string& cp_option) {
  std::string class_path(cp_option);
  if (class_path == "") {
    char buf[PATHNAME_MAX];
    if (nullptr == getcwd(buf, sizeof(buf))) {
        fprintf(stderr, "getcwd error: %s", strerror(errno));
        exit(1);
    }
    class_path = string(buf);
  }

  app_class_reader_ = CreateClassReader(class_path);
}
std::shared_ptr<ClassData> ClassPathParser::ReadClass(const string &class_name) {
  string class_path = ClassNameToClassPath(class_name);
  std::shared_ptr<ClassData> data = boot_class_reader_->ReadClass(class_path);
  if (data->GetReadErrno() == kSucceed) {
    return data;
  }
  data = ext_class_reader_->ReadClass(class_path);
  if (data->GetReadErrno() == kSucceed) {
    return data;
  }
  data = app_class_reader_->ReadClass(class_path);
  if (data->GetReadErrno() == kSucceed) {
    return data;
  }
  //std::cout << "ClassNotFoundException: " << class_path << std::endl;
  //throw "ClassNotFoundException: " + class_path;
  LOG(FATAL) << "ClassNotFoundException: " << class_path;
}

std::shared_ptr<ClassReader> ClassPathParser::GetBootClassReader() {
  return boot_class_reader_;
}
std::shared_ptr<ClassReader> ClassPathParser::GetExtClassReader() {
  return ext_class_reader_;
}
std::shared_ptr<ClassReader> ClassPathParser::GetAppClassReader() {
  return app_class_reader_;
}
}