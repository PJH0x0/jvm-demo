#pragma once

#include <memory>
#include <runtime/frame.h>
#include <runtime/string_pool.h>

namespace native {
void Intern(runtime::Frame* frame) {
  auto this_obj = frame->GetLocalVars().GetThis();
  auto interned_str = runtime::StringPool::internString(this_obj);
  frame->GetOperandStack().PushRef(interned_str);
}
}