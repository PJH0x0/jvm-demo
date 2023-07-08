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
  std::shared_ptr<ClassData> data;
  //std::shared_ptr<ClassFile> classFile;
  ConstantInfoTest() {
  }

  ~ConstantInfoTest() override {
  }

  void SetUp() override {
    std::string classDir = TEST_PATH "/test_dependencies";
    DirClassReader reader(classDir);
    std::string className = "ClassReaderTest";
    std::string classPath = classNameToClassPath(className);
    data = reader.readClass(classPath);
    ASSERT_EQ(data->readErrno, SUCCEED);
    //classFile = std::make_shared<ClassFile>();
    //ASSERT_TRUE(checkClassMagic(data->data));
  }
  

  void TearDown() override {
  }
};

TEST_F(ConstantInfoTest, ConstantInfo_parseConstantInfo) {
  int pos = 8;
  u2 _count = -1;
  classfile::parseUint(data, pos, _count);
  ASSERT_NE(_count, 0);
  ASSERT_NE(_count, -1);
  int count = _count;
  std::shared_ptr<ConstantPool> pool = std::make_shared<ConstantPool>();
  LOG(INFO) << "constant count = " << std::hex << count;
  for (int i = 1; i < count; i++) {
    std::shared_ptr<ConstantInfo> constantInfo = parseConstantInfo(data, pos);
    pool->constantInfos.push_back(constantInfo);
    switch (constantInfo->mTag) {
      case classfile::CONSTANT_Double:
      case classfile::CONSTANT_Long:
        i++;
        break;
    }
  }
}

}