
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
  Heap(size_t maxSize);
  size_t GetAbsoluteMaxSize() const;

  void SetAbsoluteMaxSize(size_t absolute_max_size);

  size_t GetBytesAllocated() const;

  void SetBytesAllocated(size_t bytes_allocated);

  size_t GetObjectsAllocated() const;

  void SetObjectsAllocated(size_t objects_allocated);

  uint8_t* GetHeapBase() const;

  void SetHeapBase(uint8_t* heap_base);

  uint8_t* GetFreshStart() const;

  void SetFreshStart(uint8_t* fresh_start);

  uint8_t* GetFreshPos() const;

  void SetFreshPos(uint8_t* fresh_pos);

  size_t GetFreshSize() const;

  void SetFreshSize(size_t fresh_size);

  uint8_t* GetFromPtr() const;

  void SetFromPtr(uint8_t* from_ptr);

  uint8_t* GetToPtr() const;

  void SetToPtr(uint8_t* to_ptr);

  size_t GetSurvivorSize() const;

  void SetSurvivorSize(size_t survivor_size);

  uint8_t* GetOldStart() const;

  void SetOldStart(uint8_t* old_start);

  uint8_t* GetOldPos() const;

  void SetOldPos(uint8_t* old_pos);
  runtime::Object* AllocObject(runtime::Thread* self, runtime::Class* clazz, size_t objSize);
private:
  size_t absolute_max_size_;
  size_t bytes_allocated_;
  size_t objects_allocated_;
  uint8_t* heap_base_;
  uint8_t* fresh_start_;
  uint8_t* fresh_pos_;
  size_t fresh_size_;
  uint8_t* from_ptr_;
  uint8_t* to_ptr_;
  size_t survivor_size_;
  uint8_t* old_start_;
  uint8_t* old_pos_;
};

}