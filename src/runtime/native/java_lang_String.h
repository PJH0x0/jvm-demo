#pragma once

#include <memory>
#include <runtime/frame.h>
#include <runtime/string_pool.h>

namespace native {
void intern(std::shared_ptr<runtime::Frame> frame) {
  auto thisObj = frame->getLocalVars().getThis();
  auto internedStr = runtime::StringPool::internString(thisObj);
  frame->getOperandStack().pushRef(internedStr);
}
}