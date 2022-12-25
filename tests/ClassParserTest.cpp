#include <ClassReader.h>
#include <ClassParser.h>
#include <ConstantPool.h>
#include "gtest/gtest.h"
#include <glog/logging.h>
#include <memory>
#include <string>

using namespace JVM;
namespace unit_test {
class ClassParserTest : public testing::Test {
  protected:
  std::shared_ptr<ClassData> data;
  std::shared_ptr<ClassFile> classFile;
  static int pos;
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

int ClassParserTest::pos = 0;

TEST_F(ClassParserTest, parseAndCheckMagic) {
  //int pos = 0;
  JVM::parseAndCheckMagic(data, classFile, pos);
  ASSERT_EQ(pos, sizeof(classFile->magic));
}

TEST_F(ClassParserTest, parseAndCheckVersion) {
  //int pos = sizeof(classFile->magic);
  LOG(INFO) << "parseAndCheckVersion current pos = " << pos;
  JVM::parseAndCheckVersion(data, classFile, pos);
  ASSERT_EQ(pos, sizeof(classFile->magic) + sizeof(classFile->minorVersion) + sizeof(classFile->majorVersion));
}

TEST_F(ClassParserTest, parseConstantPool) {
  //int pos = 8;
  LOG(INFO) << "parseConstantPool current pos = " << pos;
  JVM::parseConstantPool(data, classFile, pos);
  ASSERT_NE(classFile->constantPool->constantPoolCount, 0);
}

}