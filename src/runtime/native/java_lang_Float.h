#pragma once

#include <cstring>
#include <memory>
#include <runtime/frame.h>

namespace native {
void FloatToRawIntBits(runtime::Frame* frame) {
  auto value = frame->GetLocalVars().GetFloat(0);
  int32_t bits;
  std::memcpy(&bits, &value, sizeof(value));
  frame->GetOperandStack().PushInt(bits);
}
}