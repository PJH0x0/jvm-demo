#pragma once

#include <cstdint>
#include <memory>
#include <stack>
#include <glog/logging.h>

namespace runtime {
class Frame;
class Stack {
public:
  explicit Stack(uint32_t max_size);
  void CheckStackOverflow(uint32_t size);
  void PushFrame(Frame* frame);
  Frame* PopFrame();
  bool IsEmpty();
  void Clear();
  void* GetSp() const {
    return reinterpret_cast<void*>(sp_);
  }
  Frame* GetCurrentFrame() const {
    return fp_;
  }
  ~Stack();
private:
  void* base_;
  intptr_t sp_;
  Frame* fp_;
  uint32_t max_size_;
};
}
