#include "gtest/gtest.h"
#include <runtime/oo/class_member.h>
#include <runtime/oo/method.h>
#include <classpath/class_reader.h>
#include <classfile/class_parser.h>

namespace unit_test {
class MethodTest : public testing::Test {
public:
  static std::shared_ptr<const classfile::ClassFile> class_file_;
  MethodTest() = default;
  void SetUp() override {
    std::string class_dir = TEST_PATH "/test_materials";
    classpath::DirClassReader reader(class_dir);
    std::string class_name = "com/sample/ch02/ClassReaderTest";
    std::string class_path = classpath::ClassNameToClassPath(class_name);
    auto data = reader.ReadClass(class_path);
    ASSERT_EQ(data->GetReadErrno(), classpath::kSucceed);
    //class_file_ = std::make_shared<classfile::ClassFile>(data);
    class_file_ = classfile::Parse(data);
  }
  ~MethodTest() override = default;
};
std::shared_ptr<const classfile::ClassFile> MethodTest::class_file_;
TEST_F(MethodTest, TestConstructor) {
  auto cf_methods = class_file_->GetMethods();
  std::vector<runtime::Method*> methods;
  for (const auto& cf_method : cf_methods) {
    auto method = new runtime::Method(cf_method, new runtime::Class);
    methods.push_back(method);
  }
  for (auto method : methods) {
    free(method);
  }
}

TEST_F(MethodTest, GetMaxLocal) {

}

} //namespace unit_test
