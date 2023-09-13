#include <classpath/class_reader.h>
#include <gtest/gtest.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <initializer_list>
using namespace classpath;

namespace unit_test {


class ClassReaderTest : public testing::Test {
  protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ClassReaderTest() {
     // You can do set-up work for each test here.
  }

  ~ClassReaderTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  bool checkClassMagic(const unsigned char* data) {
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

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }
};

#ifdef __linux
#define BOOT_CLASS_PATH "/usr/lib/jvm/java-8-openjdk-amd64/jre/lib"
#endif
#ifdef __APPLE__
#define BOOT_CLASS_PATH "/Library/Java/JavaVirtualMachines/openjdk-8.jdk/Contents/Home/jre/lib"
#endif



TEST_F(ClassReaderTest, GetFiles) {
  std::string dir(BOOT_CLASS_PATH);
  std::vector<std::string> exds = {"jar", "zip", "class"};
  std::vector<std::string> files;
  GetFiles(dir, exds, files);
  for (auto filePath : files) {
    std::cout << filePath << std::endl;
  }
}

TEST_F(ClassReaderTest, ReplaceString) {
  std::string arrayListStr = "java.util.ArrayList";
  ReplaceString(arrayListStr, ".", PATH_SEPARATOR);
  ASSERT_STREQ(arrayListStr.c_str(), "java/util/ArrayList");
}

TEST_F(ClassReaderTest, DirClassReader_readClass) {
  std::string classDir = TEST_PATH "/test_dependencies";
  DirClassReader reader(classDir);
  std::string className = "ClassReaderTest";
  std::string classPath = ClassNameToClassPath(className);
  std::shared_ptr<ClassData> classData = reader.ReadClass(classPath);
  ASSERT_EQ(classData->read_errno_, SUCCEED);
  ASSERT_TRUE(checkClassMagic(classData->data_));
}

TEST_F(ClassReaderTest, ZipClassReader_readClass) {
  std::string zipPath = BOOT_CLASS_PATH "/rt.jar";
  ZipClassReader reader(zipPath);
  std::string classPath = "java/util/ArrayList.class";
  std::shared_ptr<ClassData> classData = reader.ReadClass(classPath);
  ASSERT_EQ(classData->read_errno_, SUCCEED);
  ASSERT_TRUE(checkClassMagic(classData->data_));
}

TEST_F(ClassReaderTest, CompositeClassReader_readClass) {
  std::shared_ptr<ClassReader> reader = classpath::CreateClassReader(
      BOOT_CLASS_PATH "/*:" TEST_PATH "/test_dependencies");
  std::cout << reader->String() << std::endl;
  std::string classPath = "java/util/ArrayList.class";
  std::shared_ptr<ClassData> classData = reader->ReadClass(classPath);
  ASSERT_EQ(classData->read_errno_, SUCCEED);
  ASSERT_TRUE(checkClassMagic(classData->data_));
  classPath = "ClassReaderTest.class";
  classData = reader->ReadClass(classPath);
  ASSERT_EQ(classData->read_errno_, SUCCEED);
  ASSERT_TRUE(checkClassMagic(classData->data_));
}

TEST_F(ClassReaderTest, WildcardClassReader_readClass) {
  classpath::WildcardClassReader reader(BOOT_CLASS_PATH"/*");
  std::cout << reader.String() << std::endl;
  std::string classPath = "java/util/ArrayList.class";
  std::shared_ptr<ClassData> classData = reader.ReadClass(classPath);
  ASSERT_EQ(classData->read_errno_, SUCCEED);
  ASSERT_TRUE(checkClassMagic(classData->data_));
}
}

