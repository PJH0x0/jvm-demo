#pragma once
#include <cstdlib>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using std::string;
namespace JVM {
#ifdef _WIN_32
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

#define CLASS_PATH_SEPARATOR ":"

enum READ_ERRNO{
  SUCCEED = 0,
  NOT_CLASS_FILE,
  OPEN_CLASS_FAILED,
  READ_CLASS_STREAM_FAILED,
  READ_CLASS_FAILED,
  UNKNOWN,
};
struct ClassData {
  unsigned char *data;
  int size;
  READ_ERRNO readErrno;
  ClassData() : data(nullptr), size(0), readErrno(UNKNOWN) {}
  ~ClassData() {
    size = 0;
    readErrno = UNKNOWN;
    if (data) {
      free(data);
    }
  }
};
class ClassReader {
public:
  virtual std::shared_ptr<ClassData> readClass(const string &className) = 0;
  virtual string toString() = 0;
  virtual ~ClassReader() {}
};

class DirClassReader : public ClassReader {
private:
  string absDir;

public:
  DirClassReader(string _absDir) : absDir(_absDir) {}
  std::shared_ptr<ClassData> readClass(const string &className);
  string toString();
};
class ZipClassReader : public ClassReader {
private:
  string absPath;

public:
  ZipClassReader(string _absPath) : absPath(_absPath) {}
  std::shared_ptr<ClassData> readClass(const string &className);
  string toString();
};
class CompositeClassReader : public ClassReader {
protected:
  std::vector<std::shared_ptr<ClassReader>> readers;
  std::string compositePath;

public:
  CompositeClassReader(string _compositePath) : compositePath(_compositePath) {}
  std::shared_ptr<ClassData> readClass(const string &className);
  void addClassReader(ClassReader* reader);
  void addClassReader(std::shared_ptr<ClassReader> reader);
  string toString();
};

class WildcardClassReader : public CompositeClassReader {
public:
  WildcardClassReader(string _wildcardPath);
};
void getFiles(string path, std::vector<string> &exds,
              std::vector<string> &files);
void replaceString(std::string& inout, const std::string& what, const std::string& with);
std::string classNameToClassPath(std::string className);
std::shared_ptr<ClassReader> createClassReader(const string& path);
}