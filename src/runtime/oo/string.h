#include "array.h"

class Thread;

namespace runtime {
class String final : public Object {
  private:
  int32_t count;
  uint32_t hash_code;
  char16_t value[0];
  public:
  String* alloc(Thread* self, char16_t* bytes);
};
};