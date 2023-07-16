#pragma once
#include "array.h"

class Thread;

namespace runtime {
class StringObject final : public Object {
  private:
  int32_t count;
  uint32_t hash_code;
  PrimitiveArray<char16_t>* value;
  public:
  PrimitiveArray<char16_t>* getValue() {
    return value;
  };
  void setValue(PrimitiveArray<char16_t>* value) {
    this->value = value;
  };
};
};