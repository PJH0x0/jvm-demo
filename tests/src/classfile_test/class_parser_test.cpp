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
  static std::shared_ptr<ClassData> data_;
  static std::shared_ptr<ClassFile> class_file_;
  static int pos_;
  // static const u4 kMagic = 0xCAFEBABE;
  // static const u2 kMinorVersion = 0x0;
  // static const u2 kMajorVersion = 0x34;
  // static const u2 kConstantPoolCount = 0xA0;
  // static const u2 kAccessFlags = 0x21;
  // static const u2 kThisClassNameIndex = 0x22;
  // static const u2 kSuperClassNameIndex = 0x35;
  // static const u2 kInterfaceCount = 0x0;
  // static const u2 kFieldsCount = 0x0C;
  // static const u2 kMethodsCount = 0x03;
  // static const u2 kAttributeCount = 0x03;
  ClassParserTest() {
  }

  ~ClassParserTest() override {
  } 

  static void SetUpTestSuite() {
    std::string class_dir = TEST_PATH "/test_dependencies";
    DirClassReader reader(class_dir);
    std::string class_name = "ClassReaderTest";
    std::string class_path = ClassNameToClassPath(class_name);
    data_ = reader.ReadClass(class_path);
    ASSERT_EQ(data_->read_errno_, kSucceed);
    class_file_ = std::make_shared<ClassFile>();
    //ASSERT_TRUE(CheckClassMagic(data->data));
    pos_ = 0;
  }

  static void TearDownTestSuite() {
    delete[] data_->data_;
  }

  void SetUp() override {
    
  }
  

  void TearDown() override {
  }
};

int ClassParserTest::pos_;
std::shared_ptr<ClassFile> ClassParserTest::class_file_;
std::shared_ptr<ClassData> ClassParserTest::data_;
constexpr u4 kMagic = 0xCAFEBABE;
constexpr u2 kMinorVersion = 0x0;
constexpr u2 kMajorVersion = 0x34;
constexpr u2 kConstantPoolCount = 0x85;
constexpr u2 kAccessFlags = 0x21;
constexpr u2 kThisClassNameIndex = 0x20;
constexpr u2 kSuperClassNameIndex = 0x21;
constexpr u2 kInterfaceCount = 0x0;
constexpr u2 kFieldsCount = 0x0C;
constexpr u2 kMethodsCount = 0x04;
constexpr u2 kAttributeCount = 0x01;

TEST_F(ClassParserTest, ParseAndCheckMagic) {
  //int pos_ = 0;
  classfile::ParseAndCheckMagic(data_, class_file_, pos_);
  ASSERT_EQ(class_file_->magic_, kMagic);
}

TEST_F(ClassParserTest, ParseAndCheckVersion) {
  //int pos_ = sizeof(class_file_->magic);
  LOG(INFO) << "ParseAndCheckVersion current pos_ = " << pos_;
        classfile::ParseAndCheckVersion(data_, class_file_, pos_);
  //ASSERT_EQ(pos_, sizeof(class_file_->magic) + sizeof(class_file_->minorVersion) + sizeof(class_file_->majorVersion));
  ASSERT_EQ(class_file_->minor_version_, kMinorVersion);
  ASSERT_EQ(class_file_->major_version_, kMajorVersion);
}

TEST_F(ClassParserTest, ParseConstantPool) {
  //int pos_ = 8;
  LOG(INFO) << "ParseConstantPool current pos_ = " << pos_;
        classfile::ParseConstantPool(data_, class_file_, pos_);
  ASSERT_NE(class_file_->constant_pool_->constant_pool_count_, 0);
  ASSERT_EQ(class_file_->constant_pool_->constant_pool_count_, kConstantPoolCount);
}
TEST_F(ClassParserTest, ParseAccessFlags) {
  LOG(INFO) << "ParseAccessFlags current pos_ = " << pos_;
        classfile::ParseAccessFlags(data_, class_file_, pos_);
  ASSERT_EQ(class_file_->access_flags_, kAccessFlags);
}
TEST_F(ClassParserTest, ParseThisClass) {
  LOG(INFO) << "ParseThisClass current pos_ = " << pos_;
        classfile::ParseThisClass(data_, class_file_, pos_);
  ASSERT_EQ(class_file_->this_class_, kThisClassNameIndex);
}

TEST_F(ClassParserTest, ParseSuperClass) {
  LOG(INFO) << "ParseSuperClass current pos_ = " << pos_;
        classfile::ParseSuperClass(data_, class_file_, pos_);
  ASSERT_EQ(class_file_->super_class_, kSuperClassNameIndex);
}

TEST_F(ClassParserTest, ParseInterfaces) {
  LOG(INFO) << "ParseInterfaces current pos_ = " << pos_;
        classfile::ParseInterfaces(data_, class_file_, pos_);
  ASSERT_EQ(class_file_->interfaces_.size(), kInterfaceCount);
}

TEST_F(ClassParserTest, ParseFieldInfos) {
  LOG(INFO) << "ParseFieldInfos current pos_ = " << pos_ << " superClass = ";
        classfile::ParseFieldInfos(data_, class_file_, pos_);
  ASSERT_EQ(class_file_->fields_.size(), kFieldsCount);
}
TEST_F(ClassParserTest, ParseMethodInfos) {
  LOG(INFO) << "ParseMethodInfos current pos_ = " << pos_ << " superClass = ";
        classfile::ParseMethodInfos(data_, class_file_, pos_);
  ASSERT_EQ(class_file_->methods_.size(), kMethodsCount);
}
TEST_F(ClassParserTest, ParseAttributeInfos) {
  LOG(INFO) << "ParseAttributeInfos current pos_ = " << pos_ << " superClass = ";
        classfile::ParseAttributeInfos(data_, class_file_, pos_);
  ASSERT_EQ(class_file_->attributes_.size(), kAttributeCount);
}

TEST_F(ClassParserTest, parse) {
  pos_ = 0;
  class_file_ = classfile::Parse(data_);
  ASSERT_EQ(class_file_->magic_, kMagic);
  ASSERT_EQ(class_file_->minor_version_, kMinorVersion);
  ASSERT_EQ(class_file_->major_version_, kMajorVersion);
  ASSERT_EQ(class_file_->constant_pool_->constant_pool_count_, kConstantPoolCount);
  ASSERT_EQ(class_file_->access_flags_, kAccessFlags);
  ASSERT_EQ(class_file_->this_class_, kThisClassNameIndex);
  ASSERT_EQ(class_file_->super_class_, kSuperClassNameIndex);
  ASSERT_EQ(class_file_->interfaces_.size(), kInterfaceCount);
  ASSERT_EQ(class_file_->fields_.size(), kFieldsCount);
  ASSERT_EQ(class_file_->methods_.size(), kMethodsCount);
  ASSERT_EQ(class_file_->attributes_.size(), kAttributeCount);
}

}