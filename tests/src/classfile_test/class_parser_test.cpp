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
    std::string class_dir = TEST_PATH "/test_materials";
    DirClassReader reader(class_dir);
    std::string class_name = "com/sample/ch02/ClassReaderTest";
    std::string class_path = ClassNameToClassPath(class_name);
    data_ = reader.ReadClass(class_path);
    ASSERT_EQ(data_->GetReadErrno(), kSucceed);
    class_file_ = std::make_shared<ClassFile>(data_);
    //ASSERT_TRUE(CheckClassMagic(data->data));
    pos_ = 0;
  }

  static void TearDownTestSuite() {
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
constexpr u2 kConstantPoolCount = 0x8D;
constexpr u2 kAccessFlags = 0x21;
constexpr u2 kThisClassNameIndex = 0x20;
constexpr u2 kSuperClassNameIndex = 0x21;
constexpr u2 kInterfaceCount = 0x0;
constexpr u2 kFieldsCount = 0x0C;
constexpr u2 kMethodsCount = 0x04;
constexpr u2 kAttributeCount = 0x01;

TEST_F(ClassParserTest, ParseAndCheckMagic) {
  //int pos_ = 0;
  class_file_->ParseAndCheckMagic();
  ASSERT_EQ(class_file_->GetMagic(), kMagic);
}

TEST_F(ClassParserTest, ParseAndCheckVersion) {
  //int pos_ = sizeof(class_file_->magic);
  //LOG(INFO) << "ParseAndCheckVersion current pos_ = " << pos_;
  class_file_->ParseAndCheckVersion();
  //ASSERT_EQ(pos_, sizeof(class_file_->magic) + sizeof(class_file_->minorVersion) + sizeof(class_file_->majorVersion));
  ASSERT_EQ(class_file_->GetMinorVersion(), kMinorVersion);
  ASSERT_EQ(class_file_->GetMajorVersion(), kMajorVersion);
}

TEST_F(ClassParserTest, ParseConstantPool) {
  //int pos_ = 8;
  //LOG(INFO) << "ParseConstantPool current pos_ = " << pos_;
  class_file_->ParseConstantPool();
  ASSERT_NE(class_file_->GetConstantPool()->GetConstantCount(), 0);
  ASSERT_EQ(class_file_->GetConstantPool()->GetConstantCount(), kConstantPoolCount);
}
TEST_F(ClassParserTest, ParseAccessFlags) {
  //LOG(INFO) << "ParseAccessFlags current pos_ = " << pos_;
  class_file_->ParseAccessFlags();
  ASSERT_EQ(class_file_->GetAccessFlags(), kAccessFlags);
}
TEST_F(ClassParserTest, ParseThisClass) {
  //LOG(INFO) << "ParseThisClass current pos_ = " << pos_;
  class_file_->ParseThisClass();
  ASSERT_EQ(class_file_->GetThisClass(), kThisClassNameIndex);
}

TEST_F(ClassParserTest, ParseSuperClass) {
  //LOG(INFO) << "ParseSuperClass current pos_ = " << pos_;
  class_file_->ParseSuperClass();
  ASSERT_EQ(class_file_->GetSuperClass(), kSuperClassNameIndex);
}

TEST_F(ClassParserTest, ParseInterfaces) {
  //LOG(INFO) << "ParseInterfaces current pos_ = " << pos_;
  class_file_->ParseInterfaces();
  ASSERT_EQ(class_file_->GetInterfaces().size(), kInterfaceCount);
}

TEST_F(ClassParserTest, ParseFieldInfos) {
  //LOG(INFO) << "ParseFieldInfos current pos_ = " << pos_ << " superClass = ";
  class_file_->ParseFieldInfos();
  ASSERT_EQ(class_file_->GetFields().size(), kFieldsCount);
}
TEST_F(ClassParserTest, ParseMethodInfos) {
  //LOG(INFO) << "ParseMethodInfos current pos_ = " << pos_ << " superClass = ";
  class_file_->ParseMethodInfos();
  ASSERT_EQ(class_file_->GetMethods().size(), kMethodsCount);
}
TEST_F(ClassParserTest, ParseAttributeInfos) {
  //LOG(INFO) << "ParseAttributeInfos current pos_ = " << pos_ << " superClass = ";
  class_file_->ParseAttributeInfos();
  ASSERT_EQ(class_file_->GetAttributes().size(), kAttributeCount);
}

TEST_F(ClassParserTest, Parse) {
  pos_ = 0;
  auto class_file = classfile::Parse(data_);
  ASSERT_EQ(class_file->GetMagic(), kMagic);
  ASSERT_EQ(class_file->GetMinorVersion(), kMinorVersion);
  ASSERT_EQ(class_file->GetMajorVersion(), kMajorVersion);
  ASSERT_EQ(class_file->GetConstantPool()->GetConstantCount(), kConstantPoolCount);
  ASSERT_EQ(class_file->GetAccessFlags(), kAccessFlags);
  ASSERT_EQ(class_file->GetThisClass(), kThisClassNameIndex);
  ASSERT_EQ(class_file->GetSuperClass(), kSuperClassNameIndex);
  ASSERT_EQ(class_file->GetInterfaces().size(), kInterfaceCount);
  ASSERT_EQ(class_file->GetFields().size(), kFieldsCount);
  ASSERT_EQ(class_file->GetMethods().size(), kMethodsCount);
  ASSERT_EQ(class_file->GetAttributes().size(), kAttributeCount);
}

}