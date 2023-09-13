#include "class_path_parser.h"
#include "class_reader.h"
#include <cstdlib>
#include <cstring>
#include <exception>
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

string GetJreDir(const string &jre_option) {
  struct stat st;
  int err;
  if (jre_option != "") {
    err = lstat(jre_option.c_str(), &st);
    if (err == 0 && S_ISDIR(st.st_mode)) {
      return jre_option;
    }
  }
  string curJre = "./jre";
  err = lstat(curJre.c_str(), &st);
  if (err == 0 && S_ISDIR(st.st_mode)) {
    return curJre;
  }
  char* javaHome = std::getenv("JAVA_HOME");
  if (javaHome != nullptr) {
    curJre = string(javaHome) + "/jre";
    return curJre;
  }
  //std::cout << "Can not find jre folder!" << std::endl;
  //abort();
  //throw "Can not find jre folder!";
  LOG(FATAL) << "Can not find jre folder!";
}

void ClassPathParser::Parse(const string& jre_path_option, const string& user_class_path_option) {
  ParseBootAndExtClassPath(jre_path_option);
  ParseUserClassPath(user_class_path_option);
}

void ClassPathParser::ParseBootAndExtClassPath(const string& jre_path) {
  auto jreDir = GetJreDir(jre_path);
  std::string jreLibPath = jreDir + "/lib/*";
  boot_class_reader_ = std::make_shared<WildcardClassReader>(jreLibPath);
  std::string jreExtPath = jreDir + "/lib/ext/*";
  ext_class_reader_ = std::make_shared<WildcardClassReader>(jreExtPath);
}

void ClassPathParser::ParseUserClassPath(const string& cp_option) {
  std::string classPath(cp_option);
  if (classPath == "") {
    char buf[PATHNAME_MAX];
    if (NULL == getcwd(buf, sizeof(buf))) {
        fprintf(stderr, "getcwd error: %s", strerror(errno));
        exit(1);
    }
    classPath = string(buf);
  }

  user_class_reader_ = CreateClassReader(classPath);
}
std::shared_ptr<ClassData> ClassPathParser::ReadClass(const string &class_name) {
  string classPath = ClassNameToClassPath(class_name);
  std::shared_ptr<ClassData> data = boot_class_reader_->ReadClass(classPath);
  if (data->read_errno_ == kSucceed) {
    return data;
  }
  data = ext_class_reader_->ReadClass(classPath);
  if (data->read_errno_ == kSucceed) {
    return data;
  }
  data = user_class_reader_->ReadClass(classPath);
  if (data->read_errno_ == kSucceed) {
    return data;
  }
  //std::cout << "ClassNotFoundException: " << classPath << std::endl;
  //throw "ClassNotFoundException: " + classPath;
  LOG(FATAL) << "ClassNotFoundException: " << classPath;
}

}