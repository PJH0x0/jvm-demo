#include "ClassReader.h"
#include "gtest/gtest.h"
#include "stdio.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <initializer_list>
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(ClassReader, getFiles) {
  std::string dir("/Library/Java/JavaVirtualMachines/adoptopenjdk-8.jdk/Contents/Home/jre/lib");
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
  ASSERT_STREQ(arrayListStr, "java/util/ArrayList");
}

TEST(DirClassReader, readClass) {
  std::string classDir = "/home/android/jvm-demo/tests/javasample";
  DirClassReader reader(classDir);
  std::string className = "com.sample.Sample";
  std::string classPath = replace_all(className, ".", PATH_SEPARATOR) + ".class";
  std::shared_ptr<ClassData> classData = reader.readClass(classPath);
  ASSERT_NE(classData, nullptr);
  std::cout << "classData->data" << std::endl;
  printf("%s ", classData->data);

}
