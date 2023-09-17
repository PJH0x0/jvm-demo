#include <classpath/class_reader.h>
#include <classfile/constant_pool.h>
//#include <ConstantInfo.h>
#include <classfile/class_parser.h>
#include "gtest/gtest.h"
#include <ios>
#include <memory>
#include <string>

using namespace classfile;
using namespace classpath;
namespace unit_test {
class ConstantInfoTest : public testing::Test {
  protected:
  std::shared_ptr<ClassData> class_data_;
  std::shared_ptr<ClassFile> class_file_;
  ConstantInfoTest() {
  }

  ~ConstantInfoTest() override {
  }

  void SetUp() override {
    std::string class_dir = TEST_PATH "/test_materials";
    DirClassReader reader(class_dir);
    std::string className = "com/sample/ch02/ClassReaderTest";
    std::string classPath = ClassNameToClassPath(className);
    class_data_ = reader.ReadClass(classPath);
    ASSERT_EQ(class_data_->GetReadErrno(), kSucceed);
    class_file_ = std::make_shared<ClassFile>(class_data_);
    //ASSERT_TRUE(CheckClassMagic(data->data));
  }
  

  void TearDown() override {
  }
};

TEST_F(ConstantInfoTest, ConstantInfo_ParseConstantInfo) {
  int pos = 8;
  u2 count = -1;
  classfile::ParseUnsignedInt(class_data_, pos, count);
  ASSERT_NE(count, 0);
  ASSERT_NE(count, -1);
  std::shared_ptr<ConstantPool> pool = std::make_shared<ConstantPool>(count);
  //LOG(INFO) << "constant count = " << std::hex << count;
  /*for (int i = 1; i < count; i++) {
    std::shared_ptr<ConstantInfo> constantInfo = ParseConstantInfo(class_data_, pos);
    pool->constant_infos_.push_back(constantInfo);
    switch (constantInfo->type_) {
      case classfile::kConstantDouble:
      case classfile::kConstantLong:
        i++;
        break;
    }
  }*/
}

}