#pragma once

#include <cstring>
#include <memory>
#include <rtda/frame.h>

namespace native {
void floatToRawIntBits(std::shared_ptr<rtda::Frame> frame) {
  auto value = frame->getLocalVars().getFloat(0);
  int32_t bits;
  std::memcpy(&bits, &value, sizeof(value));
  frame->getOperandStack().pushInt(bits);
}
}