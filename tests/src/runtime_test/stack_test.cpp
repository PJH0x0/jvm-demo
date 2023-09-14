#include "gtest/gtest.h"
#include <gtest/gtest-spi.h>
#include <memory>
#include <runtime/stack.h>
#include <runtime/frame.h>
#include <runtime/oo/class_member.h>
#include <runtime/oo/method.h>


namespace unit_test {
class StackTest : public testing::Test {
  protected:
  static runtime::Stack stack_;
  StackTest() {}
  void SetUp() override {}
  ~StackTest() {}
};
runtime::Stack StackTest::stack_(3);
TEST_F(StackTest, StackTest_testPushPop) {
  std::shared_ptr<runtime::Thread> thread = std::make_shared<runtime::Thread>();
  std::shared_ptr<runtime::Frame> frame = std::make_shared<runtime::Frame>(thread, 20, 20, nullptr);
  runtime::Stack stack(10);
  stack.push(frame);
  EXPECT_NE(stack.pop(), nullptr);
  EXPECT_DEATH(stack.top(), "jvm stack is empty");
}
TEST_F(StackTest, Stack_testFatal) {
  // std::shared_ptr<runtime::Thread> thread = std::make_shared<runtime::Thread>();
  // std::shared_ptr<runtime::Method> method = std::make_shared<runtime::Method>(nullptr, nullptr);
  // std::shared_ptr<runtime::Frame> frame = std::make_shared<runtime::Frame>(thread, 20, 20, nullptr);
  //runtime::Stack stack(3);
  //EXPECT_FATAL_FAILURE(stack_.pop(), "jvm stack is empty");
  //EXPECT_FATAL_FAILURE(stack_.top(), "jvm stack is empty");
  
  //EXPECT_FATAL_FAILURE(stack_.push(frame), "java.lang.StackOverflowError");
}
}