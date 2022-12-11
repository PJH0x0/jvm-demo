#include "ClassReader.h"
#include "gtest/gtest.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <initializer_list>
#ifdef __linux
#define BOOT_CLASS_PATH "/usr/lib/jvm/java-8-openjdk-amd64/jre/lib"
#endif
#ifdef __APPLE__
#define BOOT_CLASS_PATH "/Library/Java/JavaVirtualMachines/adoptopenjdk-8.jdk/Contents/Home/jre/lib"
#endif
static bool checkClassMagic(const unsigned char* data) {
  unsigned char magic[] = {
    static_cast<unsigned char>(0xca), 
    static_cast<unsigned char>(0xfe), 
    static_cast<unsigned char>(0xba), 
    static_cast<unsigned char>(0xbe)};
  for (int i = 0; i < sizeof(magic); i++) {
    if (*(data + i) != magic[i]) return false;
  }
  return true;
}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(ClassReader, getFiles) {
  std::string dir(BOOT_CLASS_PATH);
  std::vector<std::string> exds = {"jar", "zip", "class"};
  std::vector<std::string> files;
  getFiles(dir, exds, files);
  for (auto filePath : files) {
    std::cout << filePath << std::endl;
  }
}

TEST(ClassReader, replace_all) {
  std::string arrayListStr = "java.util.ArrayList";
  replaceString(arrayListStr, ".", PATH_SEPARATOR);
  ASSERT_STREQ(arrayListStr.c_str(), "java/util/ArrayList");
}

TEST(DirClassReader, readClass) {
  std::string classDir = "/home/android/jvm-demo/tests/javasample";
  DirClassReader reader(classDir);
  std::string className = "com.sample.Sample";
  std::string classPath = classNameToClassPath(className);
  std::shared_ptr<ClassData> classData = reader.readClass(classPath);
  ASSERT_EQ(classData->readErrno, SUCCEED);
  ASSERT_TRUE(checkClassMagic(classData->data));
}

TEST(ZipClassReader, readClass) {
  std::string zipPath = BOOT_CLASS_PATH "/rt.jar";
  ZipClassReader reader(zipPath);
  std::string classPath = "java/util/ArrayList.class";
  std::shared_ptr<ClassData> classData = reader.readClass(classPath);
  ASSERT_EQ(classData->readErrno, SUCCEED);
  ASSERT_TRUE(checkClassMagic(classData->data));
}

TEST(CompositeClassReader, readClass) {
  CompositeClassReader reader(BOOT_CLASS_PATH);
  std::string classPath = "java/util/ArrayList.class";
  std::shared_ptr<ClassData> classData = reader.readClass(classPath);
  ASSERT_EQ(classData->readErrno, SUCCEED);
  ASSERT_TRUE(checkClassMagic(classData->data));
}

