#pragma once

#include <memory>
#include <runtime/frame.h>
namespace native {
void Initialize(std::shared_ptr<runtime::Frame> frame);
} // namespace native