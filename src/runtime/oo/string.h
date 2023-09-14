#pragma once
#include "array.h"

class Thread;

namespace runtime {
class StringObject final : public Object {
  private:
  int32_t count_;
  uint32_t hash_code_;
  PrimitiveArray<char16_t>* value_;
  public:
  PrimitiveArray<char16_t>* GetValue() {
    return value_;
  };
  void setValue(PrimitiveArray<char16_t>* value) {
    this->value_ = value;
  };
};
};