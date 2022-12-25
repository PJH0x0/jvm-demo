#include <ClassReader.h>
#include <ConstantInfo.h>
#include <ClassParser.h>
#include "gtest/gtest.h"
#include <ios>
#include <memory>
#include <string>

using namespace JVM;
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
    std::string classDir = "/home/android/jvm-demo/tests/javasample";
    DirClassReader reader(classDir);
    std::string className = "com.sample.ch03.ClassFileTest";
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
  JVM::parseUint(data, pos, _count);
  ASSERT_NE(_count, 0);
  ASSERT_NE(_count, -1);
  int count = _count;
  std::shared_ptr<ConstantPool> pool = std::make_shared<ConstantPool>();
  LOG(INFO) << "constant count = " << std::hex << count;
  for (int i = 1; i < count; i++) {
    std::shared_ptr<ConstantInfo> constantInfo = parseConstantInfo(data, pos);
    pool->constantInfos.push_back(constantInfo);
    switch (constantInfo->tag) {
      case JVM::CONSTANT_Double:
      case JVM::CONSTANT_Long:
        i++;
        break;
    }
  }
}

}