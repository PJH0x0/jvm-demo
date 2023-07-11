#include "gtest/gtest.h"
#include <memory>
#include <runtime/stack.h>
#include <runtime/frame.h>
#include <runtime/oo/class_member.h>
#include <runtime/oo/method.h>


namespace unit_test {
using namespace runtime;
class StackTest : public testing::Test {
  protected:
  StackTest() {}
  ~StackTest() {}
};
TEST_F(StackTest, StackTest_testPushPop) {
  std::shared_ptr<runtime::Thread> thread = std::make_shared<runtime::Thread>();
  std::shared_ptr<runtime::Frame> frame = std::make_shared<runtime::Frame>(thread, 20, 20, nullptr);
  runtime::Stack stack(10);
  stack.push(frame);
  EXPECT_NE(stack.pop(), nullptr);
  EXPECT_DEATH(stack.top(), "jvm stack is empty");
}
TEST_F(StackTest, StackTest_testFatal) {
  std::shared_ptr<runtime::Thread> thread = std::make_shared<runtime::Thread>();
  std::shared_ptr<runtime::Method> method = std::make_shared<runtime::Method>(nullptr, nullptr);
  std::shared_ptr<runtime::Frame> frame = std::make_shared<runtime::Frame>(thread, 20, 20, nullptr);
  runtime::Stack stack(3);
  EXPECT_DEATH(stack.pop(), "jvm stack is empty");
  EXPECT_DEATH(stack.top(), "jvm stack is empty");
  for (int i = 0; i < 3; i++) {
    stack.push(frame);
  }
  EXPECT_DEATH(stack.push(frame), "java.lang.StackOverflowError");
}
}