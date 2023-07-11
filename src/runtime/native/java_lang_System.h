#pragma once

#include <runtime/frame.h>
#include <memory>

namespace native {
void arraycopy(std::shared_ptr<runtime::Frame> frame);
} // namespace runtime