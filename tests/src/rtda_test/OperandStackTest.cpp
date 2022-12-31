#include "gtest/gtest.h"
#include <rtda/OperandStack.h>


namespace unit_test {
using namespace rtda;
class OperandStackTest : public testing::Test {
  protected:
  OperandStackTest() {}
  ~OperandStackTest() {}
};

TEST_F(OperandStackTest, OperandStackTest_testPushPop) {
  rtda::OperandStack stack(20);
  stack.pushInt(100);
  stack.pushInt(-100);
  stack.pushLong(2997924580);
  stack.pushLong(-2997924580);
  stack.pushFloat(3.1415926f);
  stack.pushDouble(-2.71828182845);
  stack.pushRef(nullptr);
  EXPECT_EQ(stack.popRef(), nullptr);
  EXPECT_EQ(stack.popDouble(), -2.71828182845);
  EXPECT_EQ(stack.popFloat(), 3.1415926f);
  EXPECT_EQ(stack.popLong(), -2997924580);
  EXPECT_EQ(stack.popLong(), 2997924580);
  EXPECT_EQ(stack.popInt(), -100);
  EXPECT_EQ(stack.popInt(), 100);
}
TEST_F(OperandStackTest, OperandStackTest_testFatal) {
  rtda::OperandStack stack(2);
  EXPECT_DEATH(stack.popInt(), "popInt operandStack empty");
  stack.pushInt(100);
  EXPECT_DEATH(stack.popLong(), "popLong operandStack empty");
}
}