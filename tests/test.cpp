#include "ClassReader.h"
#include "gtest/gtest.h"
#include "stdio.h"
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
  replace_all(arrayListStr, ".", PATH_SEPARATOR);
  ASSERT_STREQ(arrayListStr.c_str(), "java/util/ArrayList");
}

TEST(DirClassReader, readClass) {
  std::string classDir = "/home/android/jvm-demo/tests/javasample";
  DirClassReader reader(classDir);
  std::string className = "com.sample.Sample";
  replace_all(className, ".", PATH_SEPARATOR);
  std::string classPath = className + ".class";
  std::shared_ptr<ClassData> classData = reader.readClass(classPath);
  ASSERT_NE(classData, nullptr);
  std::cout << "classData->data" << std::endl;
  printf("%s \n", classData->data);
}

TEST(ZipClassReader, readClass) {
  std::string zipPath = BOOT_CLASS_PATH "/rt.jar";
  ZipClassReader reader(zipPath);
  std::string classPath = "java/util/ArrayList.class";
  std::shared_ptr<ClassData> classData = reader.readClass(classPath);
  ASSERT_NE(classData, nullptr);
  std::cout << "classData->data" << std::endl;
  printf("%s \n", classData->data);
}
