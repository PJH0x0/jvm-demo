#pragma once
#include <runtime/frame.h>
#include <memory>
namespace native {
void GetClass(runtime::Frame* frame);
void HashCode(runtime::Frame* frame);
void Clone(runtime::Frame* frame);
} // namespace runtime