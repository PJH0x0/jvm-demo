#include <ClassReader.h>
#include <ClassParser.h>
#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace JVM;
namespace unit_test {
class ClassParserTest : public testing::Test {
  protected:
  std::shared_ptr<ClassData> data;
  std::shared_ptr<ClassFile> classFile;

  ClassParserTest() {
  }

  ~ClassParserTest() override {
  }

  void SetUp() override {
    std::string classDir = "/home/android/jvm-demo/tests/javasample";
    DirClassReader reader(classDir);
    std::string className = "com.sample.Sample";
    std::string classPath = classNameToClassPath(className);
    data = reader.readClass(classPath);
    ASSERT_EQ(data->readErrno, SUCCEED);
    classFile = std::make_shared<ClassFile>();
    //ASSERT_TRUE(checkClassMagic(data->data));
  }
  

  void TearDown() override {
  }
};

TEST_F(ClassParserTest, parseAndCheckMagic) {
  int pos = 0;
  pos = JVM::parseAndCheckMagic(data, classFile, pos);
  ASSERT_EQ(pos, sizeof(classFile->magic));
}

TEST_F(ClassParserTest, parseAndCheckVersion) {
  int pos = sizeof(classFile->magic);
  pos = JVM::parseAndCheckVersion(data, classFile, pos);
  ASSERT_EQ(pos, sizeof(classFile->magic) + sizeof(classFile->minorVersion) + sizeof(classFile->majorVersion));
}

}