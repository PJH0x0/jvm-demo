#pragma once
#include <runtime/frame.h>
#include <memory>
namespace native {
void GetClass(std::shared_ptr<runtime::Frame> frame);
void HashCode(std::shared_ptr<runtime::Frame> frame);
void Clone(std::shared_ptr<runtime::Frame> frame);
} // namespace runtime