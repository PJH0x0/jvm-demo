#include "ClassPathParser.h"
#include "gtest/gtest.h"
#include <memory>
#include <string>
using namespace JVM;

namespace unit_test {
class ClassPathParserTest : public testing::Test {
protected:
  ClassPathParserTest() {
    // You can do set-up work for each test here.
  }

  ~ClassPathParserTest() override {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {}

  void TearDown() override {
    // Code here will be called immediately after each test (right
    // before the destructor).
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
};

TEST_F(ClassPathParserTest, getJreDir) {
   std::string jreOption = "";
   std::string jrePath = getJreDir(jreOption);
   std::string javaHome = std::string(std::getenv("JAVA_HOME")) + "/jre";
   ASSERT_STREQ(jrePath.c_str(), javaHome.c_str());
}

TEST_F(ClassPathParserTest, ClassPathParser_readClass) {
   std::string jreOption = "";
   std::string cpOption = "/home/android/jvm-demo/tests/javasample";
   JVM::ClassPathParser parser(jreOption, cpOption);
   std::string sample = "com.sample.Sample";
   std::shared_ptr<ClassData> classData = parser.readClass(sample);
   ASSERT_TRUE(checkClassMagic(classData->data));
   std::string arrayListClass = "java.util.ArrayList";
   classData = parser.readClass(arrayListClass);
   ASSERT_TRUE(checkClassMagic(classData->data));
}
TEST_F(ClassPathParserTest, ClassPathParser_readClass2) {
   std::string jreOption = "/usr/lib/jvm/java-8-openjdk-amd64/jre";
   std::string cpOption = "/home/android/jvm-demo/tests/javasample";
   JVM::ClassPathParser parser(jreOption, cpOption);
   std::string sample = "com.sample.Sample";
   std::shared_ptr<ClassData> classData = parser.readClass(sample);
   ASSERT_TRUE(checkClassMagic(classData->data));
   std::string arrayListClass = "java.util.ArrayList";
   classData = parser.readClass(arrayListClass);
   ASSERT_TRUE(checkClassMagic(classData->data));
}

} // namespace unit_test
