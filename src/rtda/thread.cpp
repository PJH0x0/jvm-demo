
#include "thread.h"
#include "stack.h"
#include <memory>

namespace rtda {
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
  void Thread::getFrames(std::vector<std::shared_ptr<Frame>> &frames) {
    for (auto frame : stack->getFrames()) {
      frames.push_back(frame);
    }
  }
}