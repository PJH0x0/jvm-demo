#include "gtest/gtest.h"
#include <runtime/operand_stack.h>


namespace test {
using namespace runtime;
class OperandStackTest : public testing::Test {
  protected:
  OperandStackTest() {}
  ~OperandStackTest() {}
};

TEST_F(OperandStackTest, OperandStackTest_testPushPop) {
  runtime::OperandStack stack(20);
  stack.PushInt(100);
  stack.PushInt(-100);
  stack.PushLong(2997924580);
  stack.PushLong(-2997924580);
  stack.PushFloat(3.1415926f);
  stack.PushDouble(-2.71828182845);
  stack.PushRef(nullptr);
  EXPECT_EQ(stack.PopRef(), nullptr);
  EXPECT_EQ(stack.PopDouble(), -2.71828182845);
  EXPECT_EQ(stack.PopFloat(), 3.1415926f);
  EXPECT_EQ(stack.PopLong(), -2997924580);
  EXPECT_EQ(stack.PopLong(), 2997924580);
  EXPECT_EQ(stack.PopInt(), -100);
  EXPECT_EQ(stack.PopInt(), 100);
}
TEST_F(OperandStackTest, OperandStackTest_testFatal) {
//  runtime::OperandStack stack(2);
//  EXPECT_DEATH(stack.PopInt(), "PopInt operand_stack_ empty");
//  stack.PushInt(100);
//  EXPECT_DEATH(stack.PopLong(), "PopLong operand_stack_ empty");
}
}