#pragma once
#include <runtime/frame.h>
#include <memory>
namespace native {
void GetPrimitiveClass(std::shared_ptr<runtime::Frame> frame);
void GetName0(std::shared_ptr<runtime::Frame> frame);
void DesiredAssertionStatus0(std::shared_ptr<runtime::Frame> frame);
} // namespace runtime