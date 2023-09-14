
#include "thread.h"
#include "stack.h"
#include <memory>

namespace runtime {
  void Thread::PushFrame(std::shared_ptr<Frame> frame) {
    stack_->push(frame);
  }
  std::shared_ptr<Frame> Thread::PopFrame() {
    return stack_->pop();
  }
  std::shared_ptr<Frame> Thread::CurrentFrame() {
    return stack_->top();
  }
  bool Thread::IsStackEmpty() {
    return stack_->isEmpty();
  }
  std::shared_ptr<std::vector<std::shared_ptr<Frame>>> Thread::GetFrames() {
    return stack_->getFrames();
  }
  void Thread::ClearStack() {
    stack_->clear();
  }

std::shared_ptr<Frame> Thread::CreateFrame() {
  return std::shared_ptr<Frame>();
}
}