#pragma once
#include <runtime/frame.h>
#include <memory>
namespace native {
void getPrimitiveClass(std::shared_ptr<runtime::Frame> frame);
void getName0(std::shared_ptr<runtime::Frame> frame);
void desiredAssertionStatus0(std::shared_ptr<runtime::Frame> frame);
} // namespace runtime