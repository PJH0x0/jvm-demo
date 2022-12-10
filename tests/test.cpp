#include "ClassReader.h"
#include "gtest/gtest.h"
#include <iostream>
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
