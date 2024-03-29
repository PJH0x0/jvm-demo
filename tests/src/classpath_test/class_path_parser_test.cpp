#include <classpath/class_path_parser.h>
#include "gtest/gtest.h"
#include <memory>
#include <string>
#include <cstdlib>
using namespace classpath;

namespace test {
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

constexpr const char* kJavaHome = "JAVA_HOME";
#ifdef linux
constexpr const char* kJavaHomeEnv = "/usr/lib/jvm/java-8-openjdk-amd64/";
#elif __APPLE__
constexpr const char* kJavaHomeEnv = "/usr/local/Cellar/openjdk@8/1.8.0+372/libexec/openjdk.jdk/Contents/Home";
#endif

TEST_F(ClassPathParserTest, GetJreDir) {
  auto java_home = std::getenv(kJavaHome) ;
  if (nullptr == java_home) {
    //GTEST_SKIP() << "Skipping test due to not set JAVA_HOME";
    setenv(kJavaHome, kJavaHomeEnv, 1);
    java_home = std::getenv(kJavaHome) ;
  }

  std::string java_home_str = java_home;
  java_home_str += "/jre";
  std::string jre_option;
  std::string jre_path = GetJreDir(jre_option);
  
  ASSERT_STREQ(jre_path.c_str(), java_home_str.c_str());
}

TEST_F(ClassPathParserTest, ReadClass) {
  auto java_home_env = std::getenv("JAVA_HOME");
  if (nullptr == java_home_env) {
    //GTEST_SKIP() << "Skipping test due to not set JAVA_HOME";
    setenv(kJavaHome, kJavaHomeEnv, 1);
    java_home_env = std::getenv(kJavaHome);
  }

  std::string java_home = java_home_env;

  std::string jre_option;
  std::string cp_option = TEST_PATH "/test_materials";
  classpath::ClassPathParser parser(jre_option, cp_option);
  std::string sample = "com/sample/ch02/ClassReaderTest";
  std::shared_ptr<ClassData> class_data = parser.ReadClass(sample);
  ASSERT_TRUE(CheckClassMagic(class_data->GetData()));
  std::string class_str = "java.util.ArrayList";
  class_data = parser.ReadClass(class_str);
  ASSERT_TRUE(CheckClassMagic(class_data->GetData()));
}
TEST_F(ClassPathParserTest, ReadClass2) {
  #ifdef linux
  std::string jre_option = "/usr/lib/jvm/java-8-openjdk-amd64/jre";
  #elif __APPLE__
  std::string jre_option = "/Library/Java/JavaVirtualMachines/adoptopenjdk-8.jdk/Contents/Home/jre";
  #endif
  std::string cp_option = TEST_PATH "/test_materials";
  classpath::ClassPathParser parser(jre_option, cp_option);
  std::string sample = "com/sample/ch02/ClassReaderTest";
  std::shared_ptr<ClassData> class_data = parser.ReadClass(sample);
  ASSERT_TRUE(CheckClassMagic(class_data->GetData()));
  std::string arraylist_class_str = "java.util.ArrayList";
  class_data = parser.ReadClass(arraylist_class_str);
  ASSERT_TRUE(CheckClassMagic(class_data->GetData()));
}

TEST_F(ClassPathParserTest, GetClassReader) {
#ifdef linux
  std::string jre_option = "/usr/lib/jvm/java-8-openjdk-amd64/jre";
#elif __APPLE__
  std::string jre_option = "/Library/Java/JavaVirtualMachines/adoptopenjdk-8.jdk/Contents/Home/jre";
#endif
  std::string cp_option = TEST_PATH "/test_materials";
  classpath::ClassPathParser parser(jre_option, cp_option);
  auto boot_class_reader = parser.GetBootClassReader();
  ASSERT_NE(boot_class_reader, nullptr);
  auto ext_class_reader = parser.GetExtClassReader();
  ASSERT_NE(ext_class_reader, nullptr);
  auto app_class_reader = parser.GetAppClassReader();
  ASSERT_NE(app_class_reader, nullptr);
}

} // namespace test
