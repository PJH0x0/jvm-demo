#pragma once

#include <rtda/frame.h>
#include <memory>

namespace native {
void arraycopy(std::shared_ptr<rtda::Frame> frame);
} // namespace rtda