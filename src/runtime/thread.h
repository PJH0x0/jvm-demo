#pragma once

#include "stack.h"
#include <cstdint>
#include <stack>
#include <memory>
namespace runtime {
class Frame;
//class Stack;
class Thread {
public:
  Thread() : pc_(0) {
    stack_ = std::make_shared<Stack>(1024);
  }
  int32_t GetPc() {return pc_;}
  void SetPc(int pc) { this->pc_ = pc;}
  void PushFrame(std::shared_ptr<Frame> frame);
  std::shared_ptr<Frame> CreateFrame();
  std::shared_ptr<std::vector<std::shared_ptr<Frame>>> GetFrames();
  std::shared_ptr<Frame> PopFrame();
  std::shared_ptr<Frame> CurrentFrame();
  bool IsStackEmpty();
  void ClearStack();
private:
  int32_t pc_;
  std::shared_ptr<Stack> stack_;
};
}