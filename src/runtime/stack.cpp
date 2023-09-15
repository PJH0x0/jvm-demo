#include "stack.h"
#include <glog/logging.h>

namespace runtime {
Stack::Stack(uint32_t max_size) : max_size_(max_size) {
    base_ = malloc(max_size);
    sp_ = reinterpret_cast<intptr_t>(base_);
    fp_ = nullptr;
}

void Stack::CheckStackOverflow(uint32_t size) {
  if ((sp_ - reinterpret_cast<intptr_t>(base_) + size ) > max_size_) {
    LOG(FATAL) << "java.lang.StackOverflowError";
  }
}

bool Stack::IsEmpty() {
  return fp_ == nullptr;
}

void Stack::Clear() {
  fp_ = nullptr;
  sp_ = reinterpret_cast<intptr_t>(base_);
}

void Stack::PushFrame(Frame* frame) {
  sp_ += sizeof(*frame);
  fp_ = frame;
}

Frame* Stack::PopFrame() {
  sp_ = reinterpret_cast<intptr_t>(fp_);
  Frame* fp = fp_;
  fp_ = fp->GetPrevFrame();
  return fp;
}

Stack::~Stack() {
  free(base_);
}

}//namespace runtime
