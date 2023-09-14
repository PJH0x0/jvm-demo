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
Heap::Heap(size_t maxSize) : absolute_max_size_(std::max(kMinHeapSize, RoundUp(maxSize, kObjectAlignment))),
                             bytes_allocated_(0),
                             objects_allocated_(0) {
  //heap_base_ = malloc(absolute_max_size_);
  heap_base_ = (uint8_t*)std::calloc(absolute_max_size_, sizeof(uint8_t));
  heap_base_ = (uint8_t*)RoundUp(reinterpret_cast<intptr_t>(heap_base_), kObjectAlignment);
  fresh_pos_ = fresh_start_ = heap_base_;

  survivor_size_ = RoundDown((absolute_max_size_ >> 2) / 10, kObjectAlignment);
  fresh_size_ = RoundDown(survivor_size_ << 3, kObjectAlignment);
  from_ptr_ = fresh_start_ + fresh_size_;
  to_ptr_ = from_ptr_ + survivor_size_;
  old_start_ = to_ptr_ + survivor_size_;
}
runtime::Object* Heap::AllocObject(runtime::Thread* self, runtime::Class* clazz, size_t objSize) {
  objSize = RoundUp(objSize, kObjectAlignment);
  if ((fresh_pos_ + objSize) > (fresh_start_ + fresh_size_)) {
    //TODO minor gc
    return nullptr;
  }
  runtime::Object* obj = (runtime::Object*)std::realloc(fresh_pos_, objSize);
  fresh_pos_ += objSize;
  //memset(obj, 0, objSize);
  obj->SetAge(0);
  obj->SetForwarded(false);
  obj->SetRemembered(false);
  obj->SetClass(clazz);
  bytes_allocated_ += objSize;
  objects_allocated_++;
  return obj;
}

size_t Heap::GetAbsoluteMaxSize() const {
  return absolute_max_size_;
}

void Heap::SetAbsoluteMaxSize(size_t absolute_max_size) {
  absolute_max_size_ = absolute_max_size;
}

size_t Heap::GetBytesAllocated() const {
  return bytes_allocated_;
}

void Heap::SetBytesAllocated(size_t bytes_allocated) {
  bytes_allocated_ = bytes_allocated;
}

size_t Heap::GetObjectsAllocated() const {
  return objects_allocated_;
}

void Heap::SetObjectsAllocated(size_t objects_allocated) {
  objects_allocated_ = objects_allocated;
}

uint8_t* Heap::GetHeapBase() const {
  return heap_base_;
}

void Heap::SetHeapBase(uint8_t* heap_base) {
  heap_base_ = heap_base;
}

uint8_t* Heap::GetFreshStart() const {
  return fresh_start_;
}

void Heap::SetFreshStart(uint8_t* fresh_start) {
  fresh_start_ = fresh_start;
}

uint8_t* Heap::GetFreshPos() const {
  return fresh_pos_;
}

void Heap::SetFreshPos(uint8_t* fresh_pos) {
  fresh_pos_ = fresh_pos;
}

size_t Heap::GetFreshSize() const {
  return fresh_size_;
}

void Heap::SetFreshSize(size_t fresh_size) {
  fresh_size_ = fresh_size;
}

uint8_t* Heap::GetFromPtr() const {
  return from_ptr_;
}

void Heap::SetFromPtr(uint8_t* from_ptr) {
  from_ptr_ = from_ptr;
}

uint8_t* Heap::GetToPtr() const {
  return to_ptr_;
}

void Heap::SetToPtr(uint8_t* to_ptr) {
  to_ptr_ = to_ptr;
}

size_t Heap::GetSurvivorSize() const {
  return survivor_size_;
}

void Heap::SetSurvivorSize(size_t survivor_size) {
  survivor_size_ = survivor_size;
}

uint8_t* Heap::GetOldStart() const {
  return old_start_;
}

void Heap::SetOldStart(uint8_t* old_start) {
  old_start_ = old_start;
}

uint8_t* Heap::GetOldPos() const {
  return old_pos_;
}

void Heap::SetOldPos(uint8_t* old_pos) {
  old_pos_ = old_pos;
}
}//namespace heap