#ifndef __CLASS_PARSER_H__
#define __CLASS_PARSER_H__ 1
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using std::string;

typedef unsigned char byte;
struct ClassData {
  char *data;
  string errMessage;
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
  std::vector<ClassReader> readers;
  std::string rootPath;

public:
  CompositeClassReader(string _rootPath);
  std::shared_ptr<ClassData> readClass(const string &className);
  string toString();
};

class WildcardEntry : public CompositeClassReader {
public:
  WildcardEntry(string _rootPath);
};
void getFiles(string path, std::vector<string> &exds,
              std::vector<string> &files);

#endif