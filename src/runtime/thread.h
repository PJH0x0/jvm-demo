#pragma once

#include <cstdint>
#include <stack>
#include <memory>
namespace runtime {
class Frame;
class Stack;
class Method;
class Thread {
public:
  static Thread* CurrentThread();
  int32_t GetPc() const {return pc_;}
  void SetPc(int pc) { this->pc_ = pc;}
  void PushFrame(Frame* frame);
  const void* GetSp();
  const Frame* CreateFrame(const Method* method);
  const Frame* GetFrames();
  Frame* PopFrame();
  Frame* CurrentFrame();
  bool IsStackEmpty();
  void ClearStack();
private:
  static Thread* thread_;
  Thread();
  int32_t pc_;
  Stack* stack_;
  ~Thread();
};
}