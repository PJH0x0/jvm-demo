#pragma once

#include <cstdint>

namespace rtda {
union Slot {
  int32_t num;
  uintptr_t ref;
};
}