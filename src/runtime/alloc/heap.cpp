#include "heap.h"
#include "heap_util.h"
#include <cstdlib>
#include <sys/mman.h>
#include <string.h>
#include <runtime/oo/object.h>
#include <runtime/thread.h>
#include <runtime/oo/class.h>

namespace heap {
static constexpr size_t kDefaultHeapSize = 128 << 20;
static constexpr size_t kMinHeapSize = 1 << 12;
static constexpr size_t kObjectAlignmentShift = 3;
static constexpr size_t kObjectAlignment = 1u << kObjectAlignmentShift;
Heap::Heap(size_t maxSize) : absoluteMaxSize(std::max(kMinHeapSize, RoundUp(maxSize, kObjectAlignment))), 
                         bytesAllocated(0), 
                         objectsAllocated(0) {
  //heapBase = malloc(absoluteMaxSize);
  heapBase = (uint8_t*)std::calloc(absoluteMaxSize, sizeof(uint8_t));
  heapBase = (uint8_t*)RoundUp(reinterpret_cast<intptr_t>(heapBase), kObjectAlignment);
  freshPos = freshStart = heapBase;

  survivorSize = RoundDown((absoluteMaxSize >> 2)/10, kObjectAlignment);
  freshSize = RoundDown(survivorSize << 3, kObjectAlignment);
  fromPtr = freshStart + freshSize;
  toPtr = fromPtr + survivorSize;
  oldStart = toPtr + survivorSize;
}
runtime::Object* Heap::allocObject(runtime::Thread* self, runtime::Class* clazz, size_t objSize) {
  objSize = RoundUp(objSize, kObjectAlignment);
  if ((freshPos + objSize) > (freshStart + freshSize)) {
    //TODO minor gc
    return nullptr;
  }
  runtime::Object* obj = (runtime::Object*)std::realloc(freshPos, objSize);
  freshPos += objSize;
  //memset(obj, 0, objSize);
  obj->setAge(0);
  obj->setForwarded(false);
  obj->setRemembered(false);
  obj->setClass(clazz);
  bytesAllocated += objSize;
  objectsAllocated++;
  return obj;
}
}//namespace heap