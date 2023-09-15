
#include "thread.h"
#include "stack.h"
#include <runtime/oo/method.h>
#include <memory>
#include <mutex>

namespace runtime {
static std::once_flag thread_single_flag;
static Thread* thread_;
Thread* Thread::CurrentThread() {
  std::call_once(thread_single_flag, [&]{
    thread_ = new Thread();
  });
  return thread_;
}

Thread::Thread() : pc_(0) {
  stack_ = new Stack(1024);
}

Frame* Thread::PopFrame() {
  return stack_->PopFrame();
}
Frame* Thread::CurrentFrame() {
  return stack_->GetCurrentFrame();
}
bool Thread::IsStackEmpty() {
  return stack_->IsEmpty();
}
const Frame* Thread::GetFrames() {
  return stack_->GetCurrentFrame();
}

void Thread::ClearStack() {
  stack_->Clear();
}

const Frame* Thread::CreateFrame(const Method* method) {
  void* sp = stack_->GetSp();
  stack_->CheckStackOverflow(method->GetMaxStack() + method->GetMaxLocals() + Frame::GetHeaderSize());
  auto* frame = new (sp) Frame(method, stack_->GetCurrentFrame());
  stack_->PushFrame(frame);
  return frame;
}

const void* Thread::GetSp() {
  return stack_->GetSp();
}
Thread::~Thread() {
  if (stack_) {
    free(stack_);
  }
}
}