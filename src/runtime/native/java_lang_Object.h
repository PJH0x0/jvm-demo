#pragma once
#include <runtime/frame.h>
#include <memory>
namespace native {
void getClass(std::shared_ptr<runtime::Frame> frame);
void hashCode(std::shared_ptr<runtime::Frame> frame);
void clone(std::shared_ptr<runtime::Frame> frame);
} // namespace runtime