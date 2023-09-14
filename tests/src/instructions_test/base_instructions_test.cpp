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
  runtime::OperandStack operand_stack(10);
  instructions::PushOperandStack<int32_t>(operand_stack, 100);
  EXPECT_EQ(operand_stack.PopInt(), 100);
}
TEST_F(BaseInstructionsTest, pushOperandStack_long) {
  runtime::OperandStack operand_stack(10);
  instructions::PushOperandStack<int64_t>(operand_stack, 2997924580L);
  EXPECT_EQ(operand_stack.PopLong(), 2997924580L);
}
TEST_F(BaseInstructionsTest, pushOperandStack_float) {
  runtime::OperandStack operand_stack(10);
  instructions::PushOperandStack(operand_stack, 3.1415926f);
  EXPECT_EQ(operand_stack.PopFloat(), 3.1415926f);
}
TEST_F(BaseInstructionsTest, pushOperandStack_double) {
  runtime::OperandStack operand_stack(10);
  instructions::PushOperandStack(operand_stack, 2.71828182845);
  EXPECT_EQ(operand_stack.PopDouble(), 2.71828182845);
}

}// namespace unit_test