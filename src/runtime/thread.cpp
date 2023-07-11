
#include "thread.h"
#include "stack.h"
#include <memory>

namespace runtime {
  void Thread::pushFrame(std::shared_ptr<Frame> frame) {
    stack->push(frame);
  }
  std::shared_ptr<Frame> Thread::popFrame() {
    return stack->pop();
  }
  std::shared_ptr<Frame> Thread::currentFrame() {
    return stack->top();
  }
  bool Thread::isStackEmpty() {
    return stack->isEmpty();
  }
  std::shared_ptr<std::vector<std::shared_ptr<Frame>>> Thread::getFrames() {
    return stack->getFrames();
  }
  void Thread::clearStack() {
    stack->clear();
  }
}