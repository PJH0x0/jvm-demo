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
  static std::shared_ptr<const classfile::ClassFile> class_file_;
  StackTest() {}
  void SetUp() override {
    auto thread = runtime::Thread::Create();
    std::string class_dir = TEST_PATH "/test_materials";
    classpath::DirClassReader reader(class_dir);
    std::string class_name = "com/sample/ch02/ClassReaderTest";
    std::string class_path = classpath::ClassNameToClassPath(class_name);
    auto data = reader.ReadClass(class_path);
    ASSERT_EQ(data->GetReadErrno(), classpath::kSucceed);
    //class_file_ = std::make_shared<classfile::ClassFile>(data);
    class_file_ = classfile::Parse(data);
  }
  ~StackTest() override = default;
};
class MockMethod : public runtime::Method {
public:
  explicit MockMethod(const std::shared_ptr<classfile::MemberInfo>& cf_method)
            : runtime::Method(cf_method, new runtime::Class()){}

  MOCK_METHOD0(GetCodes, const std::vector<uint8_t>*());
  MOCK_METHOD0(GetMaxStack, uint32_t());
  MOCK_METHOD0(GetMaxLocals, uint32_t());

};
std::shared_ptr<const classfile::ClassFile> StackTest::class_file_;
runtime::Stack StackTest::stack_(runtime::Frame::GetHeaderSize() + 5);
TEST_F(StackTest, StackTest_testPushPop) {
  auto thread = runtime::Thread::Current();
  EXPECT_NE(thread, nullptr);
  LOG(INFO) << "before create Frame";
  auto cf_methods = class_file_->GetMethods();
  LOG(INFO) << "methods size = " << cf_methods.size();

  auto method = new runtime::Method(cf_methods[0], new runtime::Class());
//  EXPECT_CALL(method, GetCodes())
//      .WillOnce(testing::Return(nullptr));
//
//  EXPECT_CALL(method, GetMaxLocals)
//      .Times(2)
//      .WillOnce(testing::Return(2));
//  EXPECT_CALL(method, GetMaxLocals)
//      .Times(2)
//      .WillOnce(testing::Return(3));
  LOG(INFO) << "before create Frame";
  auto frame1 = thread->CreateFrame(method);
  EXPECT_NE(frame1, nullptr);

//  std::shared_ptr<runtime::Frame> frame = std::make_shared<runtime::Frame>(thread, 20, 20, nullptr);
//  runtime::Stack stack(10);
//  stack.push(frame);
//  EXPECT_NE(stack.pop(), nullptr);
//  EXPECT_DEATH(stack.top(), "jvm stack is empty");
}
TEST_F(StackTest, Stack_testFatal) {
  // std::shared_ptr<runtime::Thread> thread = std::make_shared<runtime::Thread>();
  // std::shared_ptr<runtime::Method> method = std::make_shared<runtime::Method>(nullptr, nullptr);
  // std::shared_ptr<runtime::Frame> frame = std::make_shared<runtime::Frame>(thread, 20, 20, nullptr);
  //runtime::Stack stack(3);
  //EXPECT_FATAL_FAILURE(frames_.pop(), "jvm stack is empty");
  //EXPECT_FATAL_FAILURE(frames_.top(), "jvm stack is empty");
  
  //EXPECT_FATAL_FAILURE(frames_.push(frame), "java.lang.StackOverflowError");
}
}