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
  static std::shared_ptr<ClassData> data;
  static std::shared_ptr<ClassFile> classFile;
  static int pos;
  // static const u4 MAGIC = 0xCAFEBABE;
  // static const u2 MINOR_VERSION = 0x0;
  // static const u2 MAJOR_VERSION = 0x34;
  // static const u2 CONSTANT_POOL_COUNT = 0xA0;
  // static const u2 ACCESS_FLAGS = 0x21;
  // static const u2 THIS_CLASS_NAME_INDEX = 0x22;
  // static const u2 SUPER_CLASS_NAME_INDEX = 0x35;
  // static const u2 INTERFACE_COUNT = 0x0;
  // static const u2 FIELDS_COUNT = 0x0C;
  // static const u2 METHODS_COUNT = 0x03;
  // static const u2 ATTRIBUTE_COUNT = 0x03;
  ClassParserTest() {
  }

  ~ClassParserTest() override {
  } 

  static void SetUpTestSuite() {
    std::string classDir = "/home/android/jvm-demo/tests/javasample";
    DirClassReader reader(classDir);
    std::string className = "com.sample.Sample";
    std::string classPath = classNameToClassPath(className);
    data = reader.readClass(classPath);
    ASSERT_EQ(data->readErrno, SUCCEED);
    classFile = std::make_shared<ClassFile>();
    //ASSERT_TRUE(checkClassMagic(data->data));
    pos = 0;
  }

  void SetUp() override {
    
  }
  

  void TearDown() override {
  }
};

int ClassParserTest::pos;
std::shared_ptr<ClassFile> ClassParserTest::classFile;
std::shared_ptr<ClassData> ClassParserTest::data;
const u4 MAGIC = 0xCAFEBABE;
const u2 MINOR_VERSION = 0x0;
const u2 MAJOR_VERSION = 0x34;
const u2 CONSTANT_POOL_COUNT = 0xA0;
const u2 ACCESS_FLAGS = 0x21;
const u2 THIS_CLASS_NAME_INDEX = 0x22;
const u2 SUPER_CLASS_NAME_INDEX = 0x23;
const u2 INTERFACE_COUNT = 0x0;
const u2 FIELDS_COUNT = 0x0C;
const u2 METHODS_COUNT = 0x03;
const u2 ATTRIBUTE_COUNT = 0x03;

TEST_F(ClassParserTest, parseAndCheckMagic) {
  //int pos = 0;
  JVM::parseAndCheckMagic(data, classFile, pos);
  ASSERT_EQ(classFile->magic, MAGIC);
}

TEST_F(ClassParserTest, parseAndCheckVersion) {
  //int pos = sizeof(classFile->magic);
  LOG(INFO) << "parseAndCheckVersion current pos = " << pos;
  JVM::parseAndCheckVersion(data, classFile, pos);
  //ASSERT_EQ(pos, sizeof(classFile->magic) + sizeof(classFile->minorVersion) + sizeof(classFile->majorVersion));
  ASSERT_EQ(classFile->minorVersion, MINOR_VERSION);
  ASSERT_EQ(classFile->majorVersion, MAJOR_VERSION);
}

TEST_F(ClassParserTest, parseConstantPool) {
  //int pos = 8;
  LOG(INFO) << "parseConstantPool current pos = " << pos;
  JVM::parseConstantPool(data, classFile, pos);
  ASSERT_NE(classFile->constantPool->constantPoolCount, 0);
  ASSERT_EQ(classFile->constantPool->constantPoolCount, CONSTANT_POOL_COUNT);
}
TEST_F(ClassParserTest, parseAccessFlags) {
  LOG(INFO) << "parseAccessFlags current pos = " << pos;
  JVM::parseAccessFlags(data, classFile, pos);
  ASSERT_EQ(classFile->accessFlags, ACCESS_FLAGS);
}
TEST_F(ClassParserTest, parseThisClass) {
  LOG(INFO) << "parseThisClass current pos = " << pos;
  JVM::parseThisClass(data, classFile, pos);
  ASSERT_EQ(classFile->thisClass, THIS_CLASS_NAME_INDEX);
}

TEST_F(ClassParserTest, parseSuperClass) {
  LOG(INFO) << "parseSuperClass current pos = " << pos;
  JVM::parseSuperClass(data, classFile, pos);
  ASSERT_EQ(classFile->superClass, SUPER_CLASS_NAME_INDEX);
}

TEST_F(ClassParserTest, parseInterfaces) {
  LOG(INFO) << "parseInterfaces current pos = " << pos;
  JVM::parseInterfaces(data, classFile, pos);
  ASSERT_EQ(classFile->interfaces.size(), INTERFACE_COUNT);
}

TEST_F(ClassParserTest, parseFieldInfos) {
  LOG(INFO) << "parseFieldInfos current pos = " << pos << " superClass = ";
  JVM::parseFieldInfos(data, classFile, pos);
  ASSERT_EQ(classFile->fields.size(), FIELDS_COUNT);
}
TEST_F(ClassParserTest, parseMethodInfos) {
  LOG(INFO) << "parseMethodInfos current pos = " << pos << " superClass = ";
  JVM::parseMethodInfos(data, classFile, pos);
  ASSERT_EQ(classFile->methods.size(), METHODS_COUNT);
}
TEST_F(ClassParserTest, parseAttributeInfos) {
  LOG(INFO) << "parseAttributeInfos current pos = " << pos << " superClass = ";
  JVM::parseAttributeInfos(data, classFile, pos);
  ASSERT_EQ(classFile->attributes.size(), ATTRIBUTE_COUNT);
}

TEST_F(ClassParserTest, parse) {
  pos = 0;
  classFile = JVM::parse(data);
  ASSERT_EQ(classFile->magic, MAGIC);
  ASSERT_EQ(classFile->minorVersion, MINOR_VERSION);
  ASSERT_EQ(classFile->majorVersion, MAJOR_VERSION);
  ASSERT_EQ(classFile->constantPool->constantPoolCount, CONSTANT_POOL_COUNT);
  ASSERT_EQ(classFile->accessFlags, ACCESS_FLAGS);
  ASSERT_EQ(classFile->thisClass, THIS_CLASS_NAME_INDEX);
  ASSERT_EQ(classFile->superClass, SUPER_CLASS_NAME_INDEX);
  ASSERT_EQ(classFile->interfaces.size(), INTERFACE_COUNT);
  ASSERT_EQ(classFile->fields.size(), FIELDS_COUNT);
  ASSERT_EQ(classFile->methods.size(), METHODS_COUNT);
  ASSERT_EQ(classFile->attributes.size(), ATTRIBUTE_COUNT);
}

}