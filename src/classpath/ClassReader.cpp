#include "ClassReader.h"
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
#include <string>
#include <sys/stat.h>
#include <vector>
#include <glog/logging.h>
// #include <initialize_list>
using std::string;

namespace classpath {

void splitString(const string& src, const string& split, std::vector<string>& res) {
  if (src == "") return;
  string strs = src + split;
  size_t pos = strs.find(split);
  while (pos != strs.npos) {
    string tmp = strs.substr(0, pos);
    res.push_back(tmp);
    strs = strs.substr(pos+1, strs.length());
    pos = strs.find(split);
  }
}


std::shared_ptr<ClassReader> createClassReader(const string& path) {
  //int find_ = path.find(CLASS_PATH_SEPARATOR);
  //std::cout << find_ << std::endl;
  if (path.find(CLASS_PATH_SEPARATOR) != path.npos) {
    std::vector<string> paths;
    splitString(path, CLASS_PATH_SEPARATOR, paths);
    
    std::shared_ptr<CompositeClassReader> classReader = std::make_shared<CompositeClassReader>(path);
    for (auto tmpPath : paths) {
      std::shared_ptr<ClassReader> tmpReader = createClassReader(tmpPath);
      classReader->addClassReader(tmpReader);
    }
    return classReader;
  }
  if (path.find_last_of("*") == path.length() - 1) {
    std::shared_ptr<WildcardClassReader> reader = std::make_shared<WildcardClassReader>(path);
    return reader;
  }
  std::string suffix = path.substr(path.find_last_of('.') + 1);
  if (suffix == "zip" || suffix == "jar" || suffix == "JAR") {
    std::shared_ptr<ZipClassReader> reader = std::make_shared<ZipClassReader>(path);
    return reader;
  }
  std::shared_ptr<DirClassReader> reader = std::make_shared<DirClassReader>(path);
  return reader;

}


void getFiles(string path, std::vector<string> &exds,
              std::vector<string> &files) {
  if (nullptr == path.c_str()) {
    LOG(ERROR) << "Path is null";
    return;
  }
  struct stat s;
  int err = lstat(path.c_str(), &s);
  if (err || !S_ISDIR(s.st_mode)) {
    LOG(WARNING) << "Path " << path << " is not a valid directory";
    return;
  }
  struct dirent *dirent;
  DIR *dir;
  dir = opendir(path.c_str());
  if (nullptr == dir) {
    LOG(ERROR) << "Open " << path << " failed";
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
  std::ifstream classStream(classPath.c_str(), std::ios::in | std::ios::binary);
  if (!classStream) {
    LOG(ERROR) << "Open " << classPath << " failed";
    classData->readErrno = OPEN_CLASS_FAILED;
    return classData;
  }
  char *tmp = (char *)malloc(size);
  memset(tmp, 0, size);
  if (classStream.read(tmp, size)) {
    classData->data = reinterpret_cast<unsigned char *>(tmp);
    classData->size = size;
    classData->readErrno = SUCCEED;
    LOG(INFO) << "Read " << classPath << " succeed";
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
      std::istream *decompressionsStream = entry->GetDecompressionStream();
      size_t size = entry->GetSize();
      char *tmp = (char *)malloc(sizeof(char) * size);
      memset(tmp, 0, size);
      if (decompressionsStream->read(tmp, size)) {
        classData->data = reinterpret_cast<unsigned char *>(tmp);
        classData->size = size;
        classData->readErrno = SUCCEED;
        //std::cout << "read class success" << std::endl;
        LOG(INFO) << "ZipClassReader::readClass -> read class succeed";
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

void CompositeClassReader::addClassReader(ClassReader* reader) {
  std::shared_ptr<ClassReader> tmpReader(reader);
  addClassReader(tmpReader);
}
void CompositeClassReader::addClassReader(std::shared_ptr<ClassReader> reader) {
  readers.push_back(reader);
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
  string result = compositePath + " ";
  for (auto classReader : readers) {
    result += classReader->toString();
  }
  return result;
}

WildcardClassReader::WildcardClassReader(string _wildcardPath) : CompositeClassReader(_wildcardPath) {
  compositePath = compositePath.substr(0, compositePath.find_last_of("*"));
  readers.push_back(std::make_shared<DirClassReader>(compositePath));
  std::vector<std::string> exds = {"jar", "zip", "JAR"};
  std::vector<string> jarFiles;
  getFiles(compositePath, exds, jarFiles);
  for (auto jarFilePath : jarFiles) {
    readers.push_back(std::make_shared<ZipClassReader>(jarFilePath));
  }
}
}