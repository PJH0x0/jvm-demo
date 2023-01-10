#pragma once

#include <cstdint>
#include <stack>
#include <memory>
namespace rtda {
class Frame;
class Stack;
class Thread {
  private:
  int32_t pc;
  std::shared_ptr<Stack> stack;
  public:
  Thread() : pc(0) {}
  int32_t getPC() {return pc;}
  void setPC(int pc) {this->pc = pc;}
  void pushFrame(std::shared_ptr<Frame> frame);
  std::shared_ptr<Frame> popFrame();
  std::shared_ptr<Frame> currentFrame();
};
}