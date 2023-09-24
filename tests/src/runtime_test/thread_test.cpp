#include <runtime/thread.h>
#include <runtime/oo/method.h>
#include <runtime/oo/class.h>
#include <gtest/gtest.h>
#include <classpath/class_reader.h>
#include <classfile/class_parser.h>
#include <classfile/constant_pool.h>

namespace unit_test {
class ThreadTest : public testing::Test {
public:
  std::shared_ptr<const classfile::ClassFile> class_file_;
  void SetUp() override {
    std::string class_dir = TEST_PATH "/test_materials";
    classpath::DirClassReader reader(class_dir);
    std::string class_name = "com/sample/ch02/ClassReaderTest";
    std::string class_path = classpath::ClassNameToClassPath(class_name);
    auto data = reader.ReadClass(class_path);
    ASSERT_EQ(data->GetReadErrno(), classpath::kSucceed);
    class_file_ = classfile::Parse(data);
  }
};

TEST_F(ThreadTest, TestCreate) {
  auto null_thread = runtime::Thread::Current();
  ASSERT_EQ(null_thread, nullptr);
  auto thread = runtime::Thread::Create();
  ASSERT_NE(thread, nullptr);
  auto thread2 = runtime::Thread::Current();
  ASSERT_EQ(thread, thread2);
}

TEST_F(ThreadTest, TestPc) {
  auto thread = runtime::Thread::Create();
  ASSERT_NE(thread, nullptr);
  thread->SetPc(100);
  ASSERT_EQ(thread->GetPc(), 100);
}

TEST_F(ThreadTest, TestCreateFrame) {
  auto thread = runtime::Thread::Create();
  ASSERT_NE(thread, nullptr);
  auto cf_methods = class_file_->GetMethods();
  ASSERT_NE(cf_methods.size(), 0);
  std::vector<runtime::Method*> methods;
  for (const auto& cf_method : cf_methods) {
    auto method = new runtime::Method(cf_method, new runtime::Class());
    methods.push_back(method);
  }
  ASSERT_NE(methods.size(), 0);
  auto frame = runtime::Thread::Current()->CreateFrame(methods[0]);
  ASSERT_NE(frame, nullptr);
  auto frame2 = runtime::Thread::Current()->CurrentFrame();
  ASSERT_EQ(frame, frame2);
  auto frame3 = runtime::Thread::Current()->PopFrame();
  ASSERT_EQ(frame, frame3);
  auto frame4 = runtime::Thread::Current()->CurrentFrame();
  ASSERT_EQ(frame4, nullptr);
  ASSERT_TRUE(runtime::Thread::Current()->IsStackEmpty());
}

TEST_F(ThreadTest, TestGetFrames) {
  auto thread = runtime::Thread::Create();
  ASSERT_NE(thread, nullptr);
  auto cf_methods = class_file_->GetMethods();
  ASSERT_NE(cf_methods.size(), 0);
  std::vector<runtime::Method*> methods;
  for (const auto& cf_method : cf_methods) {
    auto method = new runtime::Method(cf_method, new runtime::Class());
    methods.push_back(method);
  }
  ASSERT_NE(methods.size(), 0);
  for (auto method : methods) {
    runtime::Thread::Current()->CreateFrame(method);
  }
  auto frames = runtime::Thread::Current()->GetFrames();
  ASSERT_NE(frames, nullptr);

}

} //