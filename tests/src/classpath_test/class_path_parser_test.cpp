#include <classpath/class_path_parser.h>
#include "gtest/gtest.h"
#include <memory>
#include <string>
using namespace classpath;

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

TEST_F(ClassPathParserTest, GetJreDir) {
  std::string javaHome = std::string(std::getenv("JAVA_HOME")) ;
  if (javaHome == "") {
  GTEST_SKIP() << "Skipping test due to not set JAVA_HOME";
  }
  javaHome += "/jre";
  std::string jreOption = "";
  std::string jrePath = GetJreDir(jreOption);
  
  ASSERT_STREQ(jrePath.c_str(), javaHome.c_str());
}

TEST_F(ClassPathParserTest, ClassPathParser_readClass) {
  std::string javaHome = std::string(std::getenv("JAVA_HOME")) ;
  if (javaHome == "") {
  GTEST_SKIP() << "Skipping test due to not set JAVA_HOME";
  }
  std::string jreOption = "";
  std::string cpOption = TEST_PATH "/test_dependencies";
  classpath::ClassPathParser parser(jreOption, cpOption);
  std::string sample = "ClassReaderTest";
  std::shared_ptr<ClassData> classData = parser.ReadClass(sample);
  ASSERT_TRUE(checkClassMagic(classData->data_));
  std::string arrayListClass = "java.util.ArrayList";
  classData = parser.ReadClass(arrayListClass);
  ASSERT_TRUE(checkClassMagic(classData->data_));
}
TEST_F(ClassPathParserTest, ClassPathParser_readClass2) {
  #ifdef linux
  std::string jreOption = "/usr/lib/jvm/java-8-openjdk-amd64/jre";
  #elif __APPLE__
  std::string jreOption = "/Library/Java/JavaVirtualMachines/adoptopenjdk-8.jdk/Contents/Home/jre";
  #endif
  std::string cpOption = TEST_PATH "/test_dependencies";
  classpath::ClassPathParser parser(jreOption, cpOption);
  std::string sample = "ClassReaderTest";
  std::shared_ptr<ClassData> classData = parser.ReadClass(sample);
  ASSERT_TRUE(checkClassMagic(classData->data_));
  std::string arrayListClass = "java.util.ArrayList";
  classData = parser.ReadClass(arrayListClass);
  ASSERT_TRUE(checkClassMagic(classData->data_));
}

} // namespace unit_test
