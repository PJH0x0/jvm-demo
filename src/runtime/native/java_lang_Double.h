#pragma once

#include <cstring>
#include <memory>
#include <runtime/frame.h>

namespace native {
void DoubleToRawLongBits(std::shared_ptr<runtime::Frame> frame) {
  auto value = frame->GetLocalVars().GetDouble(0);
  int64_t bits;
  std::memcpy(&bits, &value, sizeof(value));
  frame->GetOperandStack().PushLong(bits);
}
void LongBitsToDouble(std::shared_ptr<runtime::Frame> frame) {
  auto bits = frame->GetLocalVars().GetLong(0);
  double value;
  std::memcpy(&value, &bits, sizeof(value));
  frame->GetOperandStack().PushDouble(value);
}
} // namespace native