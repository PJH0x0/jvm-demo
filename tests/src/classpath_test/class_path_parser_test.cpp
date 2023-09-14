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

  static bool CheckClassMagic(const unsigned char* data) {
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
  std::string java_home = std::string(std::getenv("JAVA_HOME")) ;
  if (java_home.empty()) {
  GTEST_SKIP() << "Skipping test due to not set JAVA_HOME";
  }
  java_home += "/jre";
  std::string jre_option;
  std::string jre_path = GetJreDir(jre_option);
  
  ASSERT_STREQ(jre_path.c_str(), java_home.c_str());
}

TEST_F(ClassPathParserTest, ClassPathParser_readClass) {
  std::string java_home = std::string(std::getenv("JAVA_HOME")) ;
  if (java_home.empty()) {
  GTEST_SKIP() << "Skipping test due to not set JAVA_HOME";
  }
  std::string jre_option;
  std::string cp_option = TEST_PATH "/test_dependencies";
  classpath::ClassPathParser parser(jre_option, cp_option);
  std::string sample = "ClassReaderTest";
  std::shared_ptr<ClassData> class_data = parser.ReadClass(sample);
  ASSERT_TRUE(CheckClassMagic(class_data->data_));
  std::string class_str = "java.util.ArrayList";
  class_data = parser.ReadClass(class_str);
  ASSERT_TRUE(CheckClassMagic(class_data->data_));
}
TEST_F(ClassPathParserTest, ClassPathParser_readClass2) {
  #ifdef linux
  std::string jre_option = "/usr/lib/jvm/java-8-openjdk-amd64/jre";
  #elif __APPLE__
  std::string jre_option = "/Library/Java/JavaVirtualMachines/adoptopenjdk-8.jdk/Contents/Home/jre";
  #endif
  std::string cp_option = TEST_PATH "/test_dependencies";
  classpath::ClassPathParser parser(jre_option, cp_option);
  std::string sample = "ClassReaderTest";
  std::shared_ptr<ClassData> class_data = parser.ReadClass(sample);
  ASSERT_TRUE(CheckClassMagic(class_data->data_));
  std::string arraylist_class_str = "java.util.ArrayList";
  class_data = parser.ReadClass(arraylist_class_str);
  ASSERT_TRUE(CheckClassMagic(class_data->data_));
}

} // namespace unit_test
