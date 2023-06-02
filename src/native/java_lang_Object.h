#pragma once
#include <rtda/frame.h>
#include <memory>
namespace native {
void getClass(std::shared_ptr<rtda::Frame> frame);
void hashCode(std::shared_ptr<rtda::Frame> frame);
} // namespace rtda