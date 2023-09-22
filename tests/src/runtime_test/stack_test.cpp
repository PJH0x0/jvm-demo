#include "gtest/gtest.h"
#include <gtest/gtest-spi.h>
#include <gmock/gmock.h>
#include <gmock/gmock-actions.h>
#include <memory>
#include <runtime/stack.h>
#include <runtime/frame.h>
#include <runtime/oo/class_member.h>
#include <runtime/oo/method.h>
#include <classpath/class_reader.h>
#include <classfile/class_parser.h>
#include <classfile/constant_pool.h>


namespace unit_test {
class StackTest : public testing::Test {
  protected:
  static runtime::Stack stack_;
  std::shared_ptr<const classfile::ClassFile> class_file_;
  StackTest() {}
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

  void TearDown() override {
    class_file_ = nullptr;
  }

  ~StackTest() override = default;
};
class MockStack : public runtime::Stack {
public:
  explicit MockStack(uint32_t max_size)
            : runtime::Stack(max_size){}

  MOCK_METHOD1(CheckStackOverflow, void(uint32_t));
};
runtime::Stack StackTest::stack_(runtime::Frame::GetHeaderSize() + 1024);
TEST_F(StackTest, StackTest_TestPushPop) {
  auto cf_methods = class_file_->GetMethods();
  void* sp = stack_.GetSp();
  ASSERT_NE(sp, nullptr);
  auto method = new runtime::Method(cf_methods[0], new runtime::Class());
  auto frame1 = new (sp) runtime::Frame(method, nullptr);
  EXPECT_NE(frame1, nullptr);
  stack_.PushFrame(frame1);
  auto frame2 = stack_.PopFrame();
  EXPECT_EQ(frame1, frame2);
  auto frame3 = stack_.GetCurrentFrame();
  EXPECT_EQ(frame3, nullptr);
  EXPECT_TRUE(stack_.IsEmpty());
}
TEST_F(StackTest, Stack_TestStackOverflow) {
  runtime::Stack stack(runtime::Frame::GetHeaderSize() + 5);
  auto cf_methods = class_file_->GetMethods();
  void* sp = stack_.GetSp();
  size_t frame_size = 0;
  std::vector<runtime::Method*> methods;
  for (const auto& cf_method : cf_methods) {
    auto method = new runtime::Method(cf_method, new runtime::Class());
    methods.push_back(method);
    frame_size += method->GetMaxLocals() + method->GetMaxStack() + runtime::Frame::GetHeaderSize();
  }
  ASSERT_NE(frame_size, 0);
  LOG(INFO) << frame_size << " " << runtime::Frame::GetHeaderSize();
  for (auto method : methods) {
    free(method);
  }

  //assert(false);
  EXPECT_DEATH(assert(false), "");
  EXPECT_DEATH(stack.CheckStackOverflow(frame_size),"java.lang.StackOverflowError");
//  auto pid = fork();
//  if (pid == 0) {
//    EXPECT_EXIT(stack.CheckStackOverflow(frame_size), ::testing::KilledBySignal(6), "java.lang.StackOverflowError");
//  } else {
//    //EXPECT_EXIT(wait(pid), ::testing::KilledBySignal(6));
//  }
  //EXPECT_EXIT(stack.CheckStackOverflow(frame_size), ::testing::KilledBySignal(6), "java.lang.StackOverflowError");
}
}