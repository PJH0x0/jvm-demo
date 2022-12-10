#include "include/ClassReader.h"
#include "ZipFile.h"
#include "stdio.h"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <ios>
#include <iostream>
#include <istream>
#include <fstream>
#include <memory>
#include <ostream>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <initialize_list>


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
    lstat(filePath.c_str(), &s);
    if (S_ISDIR(s.st_mode)) {
      getFiles(filePath, exds, files);
    }
  }
}

void replace_all(std::string &inout, const std::string &what, const std::string &with) {
  std::size_t count{};
    for (std::string::size_type pos{};
         inout.npos != (pos = inout.find(what.data(), pos, what.length()));
         pos += with.length(), ++count) {
        inout.replace(pos, what.length(), with.data(), with.length());
    }
}

std::shared_ptr<ClassData> DirClassReader::readClass(const string &className) {
  std::shared_ptr<ClassData> classData = std::make_shared<ClassData>();
  std::string classPath = absDir + PATH_SEPARATOR + className;
  struct stat st;
  lstat(classPath.c_str(), &st);
  int size = st.st_size;
  char *tmp = (char*)malloc(size + 1);
  memset(tmp, 0, size);
  std::ifstream classStream(classPath.c_str(), std::ios::in | std::ios::binary);
  if (!classStream) {
    std::cout << "open " << classPath << " failed" << std::endl;
    return nullptr;
  }
  if (classStream.read(tmp, size)) {
    *(tmp + 1) = '\0';
    classData->data = tmp;
    classData->size = size;
    std::cout << "read " << classPath << " success" << std::endl;
  } else {
    free(tmp);
    return nullptr;
  }
  classStream.close();

  return classData;
}
std::string DirClassReader::toString() { return absDir; }
std::shared_ptr<ClassData> ZipClassReader::readClass(const string &className) {
  std::shared_ptr<ClassData> classData = std::make_shared<ClassData>();
  ZipArchive::Ptr archive = ZipFile::Open(absPath);
  size_t entries = archive->GetEntriesCount();
  for (size_t i = 0; i < entries; i++) {
    auto entry = archive->GetEntry((int)i);
    if (entry->GetFullName() == className) {
      std::cout << "class name = " << className << std::endl;
      std::istream *decompressionsStream = entry->GetDecompressionStream();
      size_t size = entry->GetSize();
      std::cout << "size = " << size << std::endl;
      char *tmp = (char *)malloc(sizeof(char) * size + 1);
      memset(tmp, 0, size);
      if (decompressionsStream->read(tmp, size)) {
        *(tmp+1) = '\0';
        classData->data = tmp;
        std::cout << "read class success" << std::endl;
        classData->size = size;
      } else {
        free(tmp);
        return nullptr;
      }
      decompressionsStream->clear();
      break;
    }
  }
  return classData;
}
std::string ZipClassReader::toString() { return absPath; }