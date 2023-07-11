#pragma once

#include <cstring>
#include <memory>
#include <runtime/frame.h>

namespace native {
void floatToRawIntBits(std::shared_ptr<runtime::Frame> frame) {
  auto value = frame->getLocalVars().getFloat(0);
  int32_t bits;
  std::memcpy(&bits, &value, sizeof(value));
  frame->getOperandStack().pushInt(bits);
}
}