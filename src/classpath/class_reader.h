#pragma once
#include <cstdlib>
#include <cstdint>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using std::string;
namespace classpath {
#ifdef _WIN_32
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

#define CLASS_PATH_SEPARATOR ":"

enum ReadErrno {
  kSucceed = 0,
  kNotClassFile,
  kOpenClassFailed,
  kReadClassStreamFailed,
  kReadClassFailed,
  kUnknown,
};
struct ClassData {
  uint8_t* data_;
  int size_;
  ReadErrno read_errno_;
  ClassData() : data_(nullptr), size_(0), read_errno_(kUnknown) {}
  ~ClassData() {
    size_ = 0;
    read_errno_ = kUnknown;
    if (data_) {
      free(data_);
    }
  }
};
class ClassReader {
public:
  virtual std::shared_ptr<ClassData> ReadClass(const string& class_name) = 0;
  virtual string String() = 0;
  virtual ~ClassReader() {}
};

class DirClassReader : public ClassReader {
private:
  string abs_dir_;

public:
  DirClassReader(string abs_dir) : abs_dir_(abs_dir) {}
  std::shared_ptr<ClassData> ReadClass(const string& class_name) override;
  string String() override;
};
class ZipClassReader : public ClassReader {
private:
  string abs_path_;

public:
  ZipClassReader(string abs_path) : abs_path_(abs_path) {}
  std::shared_ptr<ClassData> ReadClass(const string& class_name) override;
  string String() override;
};
class CompositeClassReader : public ClassReader {
protected:
  std::vector<std::shared_ptr<ClassReader>> readers_;
  std::string composite_path_;

public:
  CompositeClassReader(string composite_path) : composite_path_(composite_path) {}
  std::shared_ptr<ClassData> ReadClass(const string& class_name) override;
  void AddClassReader(ClassReader* reader);
  void AddClassReader(std::shared_ptr<ClassReader> reader);
  string String() override;
};

class WildcardClassReader : public CompositeClassReader {
public:
  WildcardClassReader(string wildcard_path);
};
void GetFiles(string path, std::vector<string>& exds,
              std::vector<string>& files);
void ReplaceString(std::string& inout, const std::string& what, const std::string& with);
std::string ClassNameToClassPath(std::string class_name);
std::shared_ptr<ClassReader> CreateClassReader(const string& path);
}