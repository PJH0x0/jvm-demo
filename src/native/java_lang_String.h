#pragma once

#include <memory>
#include <rtda/frame.h>
#include <rtda/heap/string_pool.h>

namespace native {
void intern(std::shared_ptr<rtda::Frame> frame) {
  auto thisObj = frame->getLocalVars().getThis();
  auto internedStr = rtda::StringPool::internString(thisObj);
  frame->getOperandStack().pushRef(internedStr);
}
}