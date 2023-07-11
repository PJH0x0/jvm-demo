#pragma once

#include <memory>
#include <runtime/frame.h>
namespace native {
void initialize(std::shared_ptr<runtime::Frame> frame);
} // namespace native