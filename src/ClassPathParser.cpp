#include "include/ClassPathParser.h"
#include "include/ClassReader.h"
#include <cstdlib>
#include <cstring>
#include <exception>
#include <limits.h>
#include <iostream>
#include <memory>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
using std::string;


namespace JVM {
#ifdef _POSIX_PATH_MAX
#define PATHNAME_MAX		4096
#else
#define PATHNAME_MAX		1000
#endif

string getJreDir(const string &jrePath) {
  struct stat st;
  int err;
  if (jrePath != "") {
    err = lstat(jrePath.c_str(), &st);
    if (err == 0 && S_ISDIR(st.st_mode)) {
      return jrePath;
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
  throw "Can not find jre folder!";
}

void ClassPath::parseBootAndExtClassPath(const string& jrePath) {
  auto jreDir = getJreDir(jrePath);
  std::string jreLibPath = jreDir + "/lib/*";
  bootClassReader = std::make_shared<WildcardClassReader>(jreLibPath);
  std::string jreExtPath = jreDir + "/lib/ext/*";
  extClassReader = std::make_shared<WildcardClassReader>(jreExtPath);
}

void ClassPath::parseUserClassPath(const string& cpOption) {
  std::string classPath(cpOption);
  if (classPath == "") {
    char buf[PATHNAME_MAX];
    if (NULL == getcwd(buf, sizeof(buf))) {
        fprintf(stderr, "getcwd error: %s", strerror(errno));
        exit(1);
    }
    classPath = string(buf);
  }
  userClassReader = createClassReader(classPath);
}
std::shared_ptr<ClassData> ClassPath::readClass(const string &className) {
  string classPath = classNameToClassPath(className);
  std::shared_ptr<ClassData> data = bootClassReader->readClass(classPath);
  if (data->readErrno == SUCCEED) {
    return data;
  }
  data = extClassReader->readClass(classPath);
  if (data->readErrno == SUCCEED) {
    return data;
  }
  data = userClassReader->readClass(classPath);
  if (data->readErrno == SUCCEED) {
    return data;
  }
  //std::cout << "ClassNotFoundException: " << classPath << std::endl;
  throw "ClassNotFoundException: " + classPath;
}

}