#include "gtest/gtest.h"
#include <memory>
#include <rtda/Stack.h>
#include <rtda/Frame.h>


namespace unit_test {
using namespace rtda;
class StackTest : public testing::Test {
  protected:
  StackTest() {}
  ~StackTest() {}
};
TEST_F(StackTest, StackTest_testPushPop) {
  std::shared_ptr<rtda::Frame> frame = std::make_shared<rtda::Frame>(20, 20, nullptr);
  rtda::Stack stack(10);
  stack.push(frame);
  EXPECT_NE(stack.pop(), nullptr);
  EXPECT_DEATH(stack.top(), "jvm stack is empty");
}
TEST_F(StackTest, StackTest_testFatal) {
  std::shared_ptr<rtda::Frame> frame = std::make_shared<rtda::Frame>(20, 20, nullptr);
  rtda::Stack stack(3);
  EXPECT_DEATH(stack.pop(), "jvm stack is empty");
  EXPECT_DEATH(stack.top(), "jvm stack is empty");
  for (int i = 0; i < 3; i++) {
    stack.push(frame);
  }
  EXPECT_DEATH(stack.push(frame), "java.lang.StackOverflowError");
}
}