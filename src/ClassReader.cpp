#include "include/ClassReader.h"
#include "ZipFile.h"
#include "stdio.h"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <memory>
#include <ostream>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <vector>
// #include <initialize_list>
using std::string;

void getFiles(string path, std::vector<string> &exds,
              std::vector<string> &files) {
  if (nullptr == path.c_str()) {
    std::cout << "path is null ptr" << std::endl;
    return;
  }
  struct stat s;
  lstat(path.c_str(), &s);
  if (!S_ISDIR(s.st_mode)) {
    std::cout << "path " << path << " is not a valid directory" << std::endl;
    return;
  }
  struct dirent *dirent;
  DIR *dir;
  dir = opendir(path.c_str());
  if (nullptr == dir) {
    std::cout << "open " << path << " failed" << std::endl;
    return;
  }
  while ((dirent = readdir(dir)) != nullptr) {
    std::string fileName(dirent->d_name);
    if (fileName == "." || fileName == "..") {
      continue;
    }
    std::string filePath = path + PATH_SEPARATOR + fileName;
    std::string fileSuffix = fileName.substr(fileName.find_last_of('.') + 1);
    if (std::find(exds.begin(), exds.end(), fileSuffix) != exds.end()) {
      files.push_back(filePath);
    }
    // lstat(filePath.c_str(), &s);
    // if (S_ISDIR(s.st_mode)) {
    //   getFiles(filePath, exds, files);
    // }
  }
}

void replaceString(std::string &inout, const std::string &what,
                 const std::string &with) {
  std::size_t count{};
  for (std::string::size_type pos{};
       inout.npos != (pos = inout.find(what.data(), pos, what.length()));
       pos += with.length(), ++count) {
    inout.replace(pos, what.length(), with.data(), with.length());
  }
}

bool checkClassPathValid(const string &classPath, std::shared_ptr<ClassData> classData) {
  string classSuffix = classPath.substr(classPath.find_last_of(".") + 1);
  if (classSuffix != "class") {
    classData->readErrno = NOT_CLASS_FILE;
    return false;
  }
  return true;
}

std::string classNameToClassPath(std::string className) {
  replaceString(className, ".", PATH_SEPARATOR);
  std::string classPath = className + ".class";
  return classPath;
}
/**
 * @params className must be such java/lang/Object.class
 */
std::shared_ptr<ClassData> DirClassReader::readClass(const string &className) {
  std::shared_ptr<ClassData> classData = std::make_shared<ClassData>();
  if (!checkClassPathValid(className, classData)) {
    return classData;
  }
  std::string classPath = absDir + PATH_SEPARATOR + className;
  struct stat st;
  lstat(classPath.c_str(), &st);
  size_t size = st.st_size;
  char *tmp = (char *)malloc(size);
  memset(tmp, 0, size);
  std::ifstream classStream(classPath.c_str(), std::ios::in | std::ios::binary);
  if (!classStream) {
    std::cout << "open " << classPath << " failed" << std::endl;
    classData->readErrno = OPEN_CLASS_FAILED;
    return classData;
  }
  if (classStream.read(tmp, size)) {
    classData->data = reinterpret_cast<unsigned char *>(tmp);
    classData->size = size;
    classData->readErrno = SUCCEED;
    std::cout << "read " << classPath << " success" << std::endl;
  } else {
    free(tmp);
    classData->readErrno = READ_CLASS_STREAM_FAILED;
    return classData;
  }
  classStream.close();

  return classData;
}

std::string DirClassReader::toString() { return absDir; }

std::shared_ptr<ClassData> ZipClassReader::readClass(const string &className) {
  std::shared_ptr<ClassData> classData = std::make_shared<ClassData>();
  if (!checkClassPathValid(className, classData)) {
    return classData;
  }
  ZipArchive::Ptr archive = ZipFile::Open(absPath);
  size_t entries = archive->GetEntriesCount();
  for (size_t i = 0; i < entries; i++) {
    auto entry = archive->GetEntry((int)i);
    if (entry->GetFullName() == className) {
      std::cout << "class name = " << className << std::endl;
      std::istream *decompressionsStream = entry->GetDecompressionStream();
      size_t size = entry->GetSize();
      std::cout << "size = " << size << std::endl;
      char *tmp = (char *)malloc(sizeof(char) * size);
      memset(tmp, 0, size);
      if (decompressionsStream->read(tmp, size)) {
        classData->data = reinterpret_cast<unsigned char *>(tmp);
        classData->size = size;
        classData->readErrno = SUCCEED;
        std::cout << "read class success" << std::endl;
      } else {
        free(tmp);
        classData->readErrno = READ_CLASS_STREAM_FAILED;
        return classData;
      }
      decompressionsStream->clear();
      break;
    }
  }
  return classData;
}

std::string ZipClassReader::toString() { return absPath; }

CompositeClassReader::CompositeClassReader(string _rootPath)
    : rootPath(_rootPath) {
  readers.push_back(std::make_shared<DirClassReader>(rootPath));
  std::vector<std::string> exds = {"jar", "zip"};
  std::vector<string> jarFiles;
  getFiles(rootPath, exds, jarFiles);
  for (auto jarFilePath : jarFiles) {
    readers.push_back(std::make_shared<ZipClassReader>(jarFilePath));
  }
}

std::shared_ptr<ClassData> CompositeClassReader::readClass(const string &className) {
  std::shared_ptr<ClassData> classData = std::make_shared<ClassData>();
  if (!checkClassPathValid(className, classData)) {
    return classData;
  }
  for (auto classReader : readers) {
    classData = classReader->readClass(className);
    if (classData->readErrno == SUCCEED) {
      return classData;
    }
  }
  classData->readErrno = READ_CLASS_FAILED;
  return classData;
}

std::string CompositeClassReader::toString() {
  string result = rootPath;
  for (auto classReader : readers) {
    result += classReader->toString();
  }
  return result;
}