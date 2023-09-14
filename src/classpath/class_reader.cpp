#include "class_reader.h"
#include "ZipFile.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <ios>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <glog/logging.h>
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


std::shared_ptr<ClassReader> CreateClassReader(const string& path) {
  //int find_ = path.find(CLASS_PATH_SEPARATOR);
  //std::cout << find_ << std::endl;
  if (path.find(CLASS_PATH_SEPARATOR) != path.npos) {
    std::vector<string> paths;
    splitString(path, CLASS_PATH_SEPARATOR, paths);
    
    std::shared_ptr<CompositeClassReader> class_reader = std::make_shared<CompositeClassReader>(path);
    for (const auto& tmp_path : paths) {
      std::shared_ptr<ClassReader> tmp_reader = CreateClassReader(tmp_path);
      class_reader->AddClassReader(tmp_reader);
    }
    return class_reader;
  }
  if (path.find_last_of('*') == path.length() - 1) {
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


void GetFiles(string path, std::vector<string>& exds,
              std::vector<string>& files) {
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
  struct dirent* dirent;
  DIR* dir;
  dir = opendir(path.c_str());
  if (nullptr == dir) {
    LOG(ERROR) << "Open " << path << " failed";
    return;
  }
  while ((dirent = readdir(dir)) != nullptr) {
    std::string file_name(dirent->d_name);
    if (file_name == "." || file_name == "..") {
      continue;
    }
    std::string file_path = path + PATH_SEPARATOR + file_name;
    std::string file_suffix = file_name.substr(file_name.find_last_of('.') + 1);
    if (std::find(exds.begin(), exds.end(), file_suffix) != exds.end()) {
      files.push_back(file_path);
    }
    // lstat(file_path.c_str(), &s);
    // if (S_ISDIR(s.st_mode)) {
    //   GetFiles(file_path, exds, files);
    // }
  }
}

void ReplaceString(std::string& inout, const std::string& what,
                   const std::string& with) {
  std::size_t count{};
  for (std::string::size_type pos{};
       inout.npos != (pos = inout.find(what.data(), pos, what.length()));
       pos += with.length(), ++count) {
    inout.replace(pos, what.length(), with.data(), with.length());
  }
}

bool CheckClassPathValid(const string& class_path, std::shared_ptr<ClassData> class_data) {
  string class_suffix = class_path.substr(class_path.find_last_of(".") + 1);
  if (class_suffix != "class") {
    class_data->SetReadErrno(kNotClassFile);
    return false;
  }
  return true;
}

std::string ClassNameToClassPath(std::string class_name) {
  ReplaceString(class_name, ".", PATH_SEPARATOR);
  std::string class_path = class_name + ".class";
  return class_path;
}
/**
 * @params class_name must be such java/lang/Object.class
 */
std::shared_ptr<ClassData> DirClassReader::ReadClass(const string& class_name) {
  std::shared_ptr<ClassData> class_data = std::make_shared<ClassData>();
  if (!CheckClassPathValid(class_name, class_data)) {
    return class_data;
  }
  std::string class_path = abs_dir_ + PATH_SEPARATOR + class_name;
  struct stat st;
  lstat(class_path.c_str(), &st);
  size_t size = st.st_size;
  std::ifstream class_stream(class_path.c_str(), std::ios::in | std::ios::binary);
  if (!class_stream) {
    //LOG(ERROR) << "Open " << class_path << " failed";
    class_data->SetReadErrno(kOpenClassFailed);
    return class_data;
  }
  char* tmp = (char *)malloc(size);
  memset(tmp, 0, size);
  if (class_stream.read(tmp, size)) {
    class_data->SetData(reinterpret_cast<unsigned char *>(tmp));
    class_data->SetSize(size);
    class_data->SetReadErrno(kSucceed);
    //LOG(INFO) << "Read " << class_path << " succeed";
  } else {
    free(tmp);
    class_data->SetReadErrno(kReadClassStreamFailed);
    return class_data;
  }
  class_stream.close();

  return class_data;
}

std::string DirClassReader::String() { return abs_dir_; }

std::shared_ptr<ClassData> ZipClassReader::ReadClass(const string& class_name) {
  std::shared_ptr<ClassData> class_data = std::make_shared<ClassData>();
  if (!CheckClassPathValid(class_name, class_data)) {
    return class_data;
  }
  ZipArchive::Ptr archive = ZipFile::Open(abs_path_);
  size_t entries = archive->GetEntriesCount();
  for (size_t i = 0; i < entries; i++) {
    auto entry = archive->GetEntry((int)i);
    if (entry->GetFullName() == class_name) {
      std::istream* decompression_stream = entry->GetDecompressionStream();
      size_t size = entry->GetSize();
      char* tmp = (char *)malloc(sizeof(char) * size);
      memset(tmp, 0, size);
      if (decompression_stream->read(tmp, size)) {
        class_data->SetData(reinterpret_cast<unsigned char *>(tmp));
        class_data->SetSize(size);
        class_data->SetReadErrno(kSucceed);
        //std::cout << "read class success" << std::endl;
        //LOG(INFO) << "ZipClassReader::ReadClass -> read class succeed";
      } else {
        free(tmp);
        class_data->SetReadErrno(kReadClassStreamFailed);
        return class_data;
      }
      decompression_stream->clear();
      break;
    }
  }
  return class_data;
}

std::string ZipClassReader::String() { return abs_path_; }

void CompositeClassReader::AddClassReader(ClassReader* reader) {
  std::shared_ptr<ClassReader> tmp_reader(reader);
  AddClassReader(tmp_reader);
}
void CompositeClassReader::AddClassReader(std::shared_ptr<ClassReader> reader) {
  readers_.push_back(reader);
}

std::shared_ptr<ClassData> CompositeClassReader::ReadClass(const string& class_name) {
  std::shared_ptr<ClassData> class_data = std::make_shared<ClassData>();
  if (!CheckClassPathValid(class_name, class_data)) {
    return class_data;
  }
  for (const auto& class_reader : readers_) {
    class_data = class_reader->ReadClass(class_name);
    if (class_data->GetReadErrno() == kSucceed) {
      return class_data;
    }
  }
  class_data->SetReadErrno(kReadClassFailed);
  return class_data;
}

std::string CompositeClassReader::String() {
  string result = composite_path_ + " ";
  for (const auto& class_reader : readers_) {
    result += class_reader->String();
  }
  return result;
}

WildcardClassReader::WildcardClassReader(string wildcard_path) : CompositeClassReader(wildcard_path) {
  composite_path_ = composite_path_.substr(0, composite_path_.find_last_of("*"));
  readers_.push_back(std::make_shared<DirClassReader>(composite_path_));
  std::vector<std::string> exds = {"jar", "zip", "JAR"};
  std::vector<string> jar_files;
  GetFiles(composite_path_, exds, jar_files);
  for (const auto& jar_file_path : jar_files) {
    readers_.push_back(std::make_shared<ZipClassReader>(jar_file_path));
  }
}
}