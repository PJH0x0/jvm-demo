#include <classpath/class_reader.h>
#include <classfile/class_parser.h>
#include <classfile/constant_pool.h>
#include "gtest/gtest.h"
#include <cstring>
#include <glog/logging.h>
#include <memory>
#include <string>

using namespace classfile;
using namespace classpath;
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
    std::string classDir = TEST_PATH "/test_dependencies";
    DirClassReader reader(classDir);
    std::string className = "ClassReaderTest";
    std::string classPath = ClassNameToClassPath(className);
    data = reader.ReadClass(classPath);
    ASSERT_EQ(data->read_errno_, SUCCEED);
    classFile = std::make_shared<ClassFile>();
    //ASSERT_TRUE(checkClassMagic(data->data));
    pos = 0;
  }

  static void TearDownTestSuite() {
    delete[] data->data_;
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
const u2 CONSTANT_POOL_COUNT = 0x85;
const u2 ACCESS_FLAGS = 0x21;
const u2 THIS_CLASS_NAME_INDEX = 0x20;
const u2 SUPER_CLASS_NAME_INDEX = 0x21;
const u2 INTERFACE_COUNT = 0x0;
const u2 FIELDS_COUNT = 0x0C;
const u2 METHODS_COUNT = 0x04;
const u2 ATTRIBUTE_COUNT = 0x01;

TEST_F(ClassParserTest, ParseAndCheckMagic) {
  //int pos = 0;
        classfile::ParseAndCheckMagic(data, classFile, pos);
  ASSERT_EQ(classFile->magic, MAGIC);
}

TEST_F(ClassParserTest, ParseAndCheckVersion) {
  //int pos = sizeof(classFile->magic);
  LOG(INFO) << "ParseAndCheckVersion current pos = " << pos;
        classfile::ParseAndCheckVersion(data, classFile, pos);
  //ASSERT_EQ(pos, sizeof(classFile->magic) + sizeof(classFile->minorVersion) + sizeof(classFile->majorVersion));
  ASSERT_EQ(classFile->minorVersion, MINOR_VERSION);
  ASSERT_EQ(classFile->majorVersion, MAJOR_VERSION);
}

TEST_F(ClassParserTest, ParseConstantPool) {
  //int pos = 8;
  LOG(INFO) << "ParseConstantPool current pos = " << pos;
        classfile::ParseConstantPool(data, classFile, pos);
  ASSERT_NE(classFile->constantPool->constant_pool_count_, 0);
  ASSERT_EQ(classFile->constantPool->constant_pool_count_, CONSTANT_POOL_COUNT);
}
TEST_F(ClassParserTest, ParseAccessFlags) {
  LOG(INFO) << "ParseAccessFlags current pos = " << pos;
        classfile::ParseAccessFlags(data, classFile, pos);
  ASSERT_EQ(classFile->accessFlags, ACCESS_FLAGS);
}
TEST_F(ClassParserTest, ParseThisClass) {
  LOG(INFO) << "ParseThisClass current pos = " << pos;
        classfile::ParseThisClass(data, classFile, pos);
  ASSERT_EQ(classFile->thisClass, THIS_CLASS_NAME_INDEX);
}

TEST_F(ClassParserTest, ParseSuperClass) {
  LOG(INFO) << "ParseSuperClass current pos = " << pos;
        classfile::ParseSuperClass(data, classFile, pos);
  ASSERT_EQ(classFile->superClass, SUPER_CLASS_NAME_INDEX);
}

TEST_F(ClassParserTest, ParseInterfaces) {
  LOG(INFO) << "ParseInterfaces current pos = " << pos;
        classfile::ParseInterfaces(data, classFile, pos);
  ASSERT_EQ(classFile->interfaces.size_(), INTERFACE_COUNT);
}

TEST_F(ClassParserTest, ParseFieldInfos) {
  LOG(INFO) << "ParseFieldInfos current pos = " << pos << " superClass = ";
        classfile::ParseFieldInfos(data, classFile, pos);
  ASSERT_EQ(classFile->fields.size_(), FIELDS_COUNT);
}
TEST_F(ClassParserTest, ParseMethodInfos) {
  LOG(INFO) << "ParseMethodInfos current pos = " << pos << " superClass = ";
        classfile::ParseMethodInfos(data, classFile, pos);
  ASSERT_EQ(classFile->methods.size_(), METHODS_COUNT);
}
TEST_F(ClassParserTest, ParseAttributeInfos) {
  LOG(INFO) << "ParseAttributeInfos current pos = " << pos << " superClass = ";
        classfile::ParseAttributeInfos(data, classFile, pos);
  ASSERT_EQ(classFile->attributes.size_(), ATTRIBUTE_COUNT);
}

TEST_F(ClassParserTest, parse) {
  pos = 0;
  classFile = classfile::parse(data);
  ASSERT_EQ(classFile->magic, MAGIC);
  ASSERT_EQ(classFile->minorVersion, MINOR_VERSION);
  ASSERT_EQ(classFile->majorVersion, MAJOR_VERSION);
  ASSERT_EQ(classFile->constantPool->constant_pool_count_, CONSTANT_POOL_COUNT);
  ASSERT_EQ(classFile->accessFlags, ACCESS_FLAGS);
  ASSERT_EQ(classFile->thisClass, THIS_CLASS_NAME_INDEX);
  ASSERT_EQ(classFile->superClass, SUPER_CLASS_NAME_INDEX);
  ASSERT_EQ(classFile->interfaces.size_(), INTERFACE_COUNT);
  ASSERT_EQ(classFile->fields.size_(), FIELDS_COUNT);
  ASSERT_EQ(classFile->methods.size_(), METHODS_COUNT);
  ASSERT_EQ(classFile->attributes.size_(), ATTRIBUTE_COUNT);
}

}