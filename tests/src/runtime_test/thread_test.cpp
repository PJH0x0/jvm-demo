#include <runtime/thread.h>
#include <gtest/gtest.h>
#include <classpath/class_reader.h>
#include <classfile/class_parser.h>
#include <classfile/constant_pool.h>

namespace unit_test {
class ThreadTest : public testing::Test {
public:
  std::shared_ptr<const classfile::ClassFile> class_file_;
  void SetUp() override {
    auto thread = runtime::Thread::Create();
    std::string class_dir = TEST_PATH "/test_materials";
    classpath::DirClassReader reader(class_dir);
    std::string class_name = "com/sample/ch02/ClassReaderTest";
    std::string class_path = classpath::ClassNameToClassPath(class_name);
    auto data = reader.ReadClass(class_path);
    ASSERT_EQ(data->GetReadErrno(), classpath::kSucceed);
    class_file_ = classfile::Parse(data);
  }
};

} //