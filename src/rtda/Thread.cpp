
#include "Thread.h"
#include "Stack.h"
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
}