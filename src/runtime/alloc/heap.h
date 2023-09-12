
#include "runtime/oo/object.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
class Thread;
class Object;
class Class;

namespace heap {
class Heap {
  public:
  size_t absoluteMaxSize;
  size_t bytesAllocated;
  size_t objectsAllocated;
  uint8_t* heapBase;
  uint8_t* freshStart;
  uint8_t* freshPos;
  size_t freshSize;
  uint8_t* fromPtr;
  uint8_t* toPtr;
  size_t survivorSize;
  uint8_t* oldStart;
  uint8_t* oldPos;
  
  Heap(size_t maxSize);
  runtime::Object* AllocObject(runtime::Thread* self, runtime::Class* clazz, size_t objSize);
};

}