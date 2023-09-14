#pragma once
#include <cstdlib>
#include <cstdint>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <glog/logging.h>
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
class ClassData {
public:
  ClassData() : data_(nullptr), size_(0), read_errno_(kUnknown) {}
  const uint8_t* GetData() const {
    return data_;
  }

  void SetData(uint8_t* data) {
    if (nullptr != data_) {
      LOG(FATAL) << "Attempt to override class data";
      return;
    }
    data_ = data;
  }

  int GetSize() const {
    return size_;
  }

  void SetSize(int size) {
    size_ = size;
  }

  ReadErrno GetReadErrno() const {
    return read_errno_;
  }

  void SetReadErrno(ReadErrno read_errno) {
    read_errno_ = read_errno;
  }
  ~ClassData() {
    size_ = 0;
    read_errno_ = kUnknown;
    if (data_) {
      free(data_);
    }
  }
private:
  uint8_t* data_;
  int size_;
  ReadErrno read_errno_;
};
class ClassReader {
public:
  virtual std::shared_ptr<ClassData> ReadClass(const string& class_name) = 0;
  virtual string String() = 0;
  virtual ~ClassReader() = default;
};

class DirClassReader : public ClassReader {
private:
  string abs_dir_;

public:
  explicit DirClassReader(string abs_dir) : abs_dir_(abs_dir) {}
  std::shared_ptr<ClassData> ReadClass(const string& class_name) override;
  string String() override;
};
class ZipClassReader : public ClassReader {
private:
  string abs_path_;

public:
  explicit ZipClassReader(string abs_path) : abs_path_(abs_path) {}
  std::shared_ptr<ClassData> ReadClass(const string& class_name) override;
  string String() override;
};
class CompositeClassReader : public ClassReader {
protected:
  std::vector<std::shared_ptr<ClassReader>> readers_;
  std::string composite_path_;

public:
  explicit CompositeClassReader(string composite_path) : composite_path_(composite_path) {}
  std::shared_ptr<ClassData> ReadClass(const string& class_name) override;
  void AddClassReader(ClassReader* reader);
  void AddClassReader(std::shared_ptr<ClassReader> reader);
  string String() override;
};

class WildcardClassReader : public CompositeClassReader {
public:
  explicit WildcardClassReader(string wildcard_path);
};
void GetFiles(string path, std::vector<string>& exds,
              std::vector<string>& files);
void ReplaceString(std::string& inout, const std::string& what, const std::string& with);
std::string ClassNameToClassPath(std::string class_name);
std::shared_ptr<ClassReader> CreateClassReader(const string& path);
}