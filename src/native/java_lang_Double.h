#pragma once

#include <cstring>
#include <memory>
#include <rtda/frame.h>

namespace native {
void doubleToRawLongBits(std::shared_ptr<rtda::Frame> frame) {
  auto value = frame->getLocalVars().getDouble(0);
  int64_t bits;
  std::memcpy(&bits, &value, sizeof(value));
  frame->getOperandStack().pushLong(bits);
}
void longBitsToDouble(std::shared_ptr<rtda::Frame> frame) {
  auto bits = frame->getLocalVars().getLong(0);
  double value;
  std::memcpy(&value, &bits, sizeof(value));
  frame->getOperandStack().pushDouble(value);
}
} // namespace native