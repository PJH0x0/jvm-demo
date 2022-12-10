#include "include/ClassReader.h"
#include "ZipFile.h"
#include "stdio.h"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <dirent.h>
#include <iostream>
#include <istream>
#include <memory>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <vector>

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
    std::string filePath = path + '/' + fileName;
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

std::shared_ptr<ClassData> DirClassReader::readClass(const string &className) {
  std::shared_ptr<ClassData> classData = std::make_shared<ClassData>();

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
      char *tmp = (char *)malloc(sizeof(char) * size);
      memset(tmp, 0, size);
      if (decompressionsStream->read(tmp, size)) {
        classData->data = tmp;
        std::cout << "read class success" << std::endl;
        // std::cout << "classData->data" << std::endl;
        // for (int i = 0; i < size; i++) {
        //     //std::cout << std::to_integer<int>(std::byte(*(tmp+i))) << " ";
        //     printf("%c ", *(tmp+i));
        // }
      } else {
        classData->data = nullptr;
      }
      break;
    }
  }
  return classData;
}
std::string ZipClassReader::toString() { return absPath; }