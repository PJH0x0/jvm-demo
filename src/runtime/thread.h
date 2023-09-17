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
  static Thread* Create();
  static Thread* Current();
  int32_t GetPc() const {return pc_;}
  void SetPc(int pc) { this->pc_ = pc;}
  const void* GetSp();
  Frame* CreateFrame(const Method* method);
  const Frame* GetFrames() const;
  Frame* PopFrame() const ;
  Frame* CurrentFrame() const ;
  bool IsStackEmpty() const;
  void ClearStack();
private:
  static void ThreadExitCallback(void* arg);
  static pthread_key_t pthread_key_self_;
  static thread_local Thread* self_tls_;
  Thread();
  bool is_started_;
  int32_t pc_;
  Stack* stack_;
  std::string thread_name_;
  ~Thread();
};
}