#include <instructions/base/base_insts.h>
#include <runtime/oo/object.h>

#include <gtest/gtest.h>
#include <cstdint>


namespace unit_test {
class BaseInstructionsTest : public testing::Test {
  protected:
  BaseInstructionsTest() {}
  ~BaseInstructionsTest() {}
};
TEST_F(BaseInstructionsTest, pushOperandStack_int) {
  runtime::OperandStack operandStack(10);
  instructions::pushOperandStack<int32_t>(operandStack, 100);
  EXPECT_EQ(operandStack.popInt(), 100);
}
TEST_F(BaseInstructionsTest, pushOperandStack_long) {
  runtime::OperandStack operandStack(10);
  instructions::pushOperandStack<int64_t>(operandStack, 2997924580L);
  EXPECT_EQ(operandStack.popLong(), 2997924580L);
}
TEST_F(BaseInstructionsTest, pushOperandStack_float) {
  runtime::OperandStack operandStack(10);
  instructions::pushOperandStack(operandStack, 3.1415926f);
  EXPECT_EQ(operandStack.popFloat(), 3.1415926f);
}
TEST_F(BaseInstructionsTest, pushOperandStack_double) {
  runtime::OperandStack operandStack(10);
  instructions::pushOperandStack(operandStack, 2.71828182845);
  EXPECT_EQ(operandStack.popDouble(), 2.71828182845);
}

}// namespace unit_test