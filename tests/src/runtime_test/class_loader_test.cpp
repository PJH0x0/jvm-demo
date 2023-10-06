#include <gtest/gtest.h>
#include <runtime/class_loader.h>
#include <runtime/oo/class.h>
#include "../stub.h"

namespace test {
class ClassLoaderTest : public testing::Test {
public:
  std::shared_ptr<classpath::ClassPathParser> parser_;
  runtime::ClassLoader* boot_class_loader_{nullptr};
  runtime::ClassLoader* ext_class_loader_{nullptr};
  runtime::ClassLoader* base_class_loader_{nullptr};
  void SetUp() override {
#ifdef linux
    std::string jre_option = "/usr/lib/jvm/java-8-openjdk-amd64/jre";
#elif __APPLE__
    std::string jre_option = "/Library/Java/JavaVirtualMachines/openjdk-8.jdk/Contents/Home/jre";
#endif
    std::string cp_option = TEST_PATH "/test_materials";
    parser_ = std::make_shared<classpath::ClassPathParser>(jre_option, cp_option);
    runtime::ClassLoader::CreateBootClassLoader(parser_->GetBootClassReader());
    runtime::ClassLoader::CreateExtClassLoader(parser_->GetExtClassReader());
    runtime::ClassLoader::CreateBaseClassLoader(parser_->GetAppClassReader());
    boot_class_loader_ = runtime::ClassLoader::GetBootClassLoader();
    ext_class_loader_ = runtime::ClassLoader::GetExtClassLoader();
    base_class_loader_ = runtime::ClassLoader::GetBaseClassLoader();
  }
};

TEST_F(ClassLoaderTest, GetClassLoader) {
  ASSERT_NE(boot_class_loader_, nullptr);
  ASSERT_NE(ext_class_loader_, nullptr);
  ASSERT_NE(base_class_loader_, nullptr);
}

runtime::Class* StubNewClassObject() {
  return new runtime::Class();
}

TEST_F(ClassLoaderTest, LoadClass) {
  auto loaded_classes = boot_class_loader_->GetLoadedClasses();
  boot_class_loader_->LoadBasicClass();
  ASSERT_NE(loaded_classes.find("java/lang/Class"), loaded_classes.end());
  boot_class_loader_->LoadPrimitiveClasses();
  for (const auto& primitive_type : runtime::Class::GetPrimitiveTypes()) {
    ASSERT_NE(loaded_classes.find(primitive_type.first), loaded_classes.end());
  }
  boot_class_loader_->LoadClass("java/lang/Object");
  ASSERT_NE(loaded_classes.find("java/lang/Object"), loaded_classes.end());
  base_class_loader_->LoadClass("com/sample/ch02/ClassReaderTest");
  ASSERT_NE(loaded_classes.find("com/sample/ch02/ClassReaderTest"), loaded_classes.end());
}
}
