#pragma once

#include <cstring>
#include <memory>
#include <runtime/frame.h>

namespace native {
void DoubleToRawLongBits(std::shared_ptr<runtime::Frame> frame) {
  auto value = frame->getLocalVars().getDouble(0);
  int64_t bits;
  std::memcpy(&bits, &value, sizeof(value));
  frame->getOperandStack().pushLong(bits);
}
void LongBitsToDouble(std::shared_ptr<runtime::Frame> frame) {
  auto bits = frame->getLocalVars().getLong(0);
  double value;
  std::memcpy(&value, &bits, sizeof(value));
  frame->getOperandStack().pushDouble(value);
}
} // namespace native