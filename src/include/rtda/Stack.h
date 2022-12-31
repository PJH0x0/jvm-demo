#pragma once

#include "Thread.h"
#include <cstdint>
#include <memory>
#include <stack>
#include <glog/logging.h>

namespace rtda {
class Frame;
class Stack {
  private:
  uint32_t maxSize;
  std::shared_ptr<std::stack<std::shared_ptr<Frame>>> stack;
  public:
  Stack(uint32_t _maxSize) : maxSize(_maxSize){
    stack = std::make_shared<std::stack<std::shared_ptr<Frame>>>();
  }
  void push(std::shared_ptr<Frame> frame) {
    if (stack->size() >= maxSize) {
      LOG(FATAL) << "java.lang.StackOverflowError";
    }
    stack->push(frame);
  }
  std::shared_ptr<Frame> pop() {
    std::shared_ptr<Frame> frame = top();
    stack->pop();
    return frame;
  }
  std::shared_ptr<Frame> top() {
    if (stack->size() <= 0) {
      LOG(FATAL) << "jvm stack is empty";
    }
    std::shared_ptr<Frame> frame = stack->top();
    if (frame == nullptr) {
      LOG(FATAL) << "jvm stack is empty";
    }
    return frame;
  }
};
}
