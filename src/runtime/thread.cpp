
#include "thread.h"
#include "stack.h"
#include "frame.h"
#include <runtime/oo/method.h>
#include <memory>
#include <mutex>
#include <glog/logging.h>

namespace runtime {
#ifndef CHECK_PTHREAD_CALL
#define CHECK_PTHREAD_CALL(call, args, what)                           \
  do {                                                                 \
    int rc = call args;                                                \
    if (rc != 0) {                                                     \
      errno = rc;                                                      \
      PLOG(FATAL) << #call << " failed for " << (what);                \
    }                                                                  \
  } while (false)
#endif
pthread_key_t Thread::pthread_key_self_;
thread_local Thread* Thread::self_tls_ = nullptr;

Thread* Thread::Create() {
  auto* self = new Thread();
  CHECK_PTHREAD_CALL(pthread_key_create, (&pthread_key_self_, Thread::ThreadExitCallback),
                     "self key");
  if (pthread_getspecific(pthread_key_self_) != nullptr) {
    LOG(FATAL) << "Newly-created pthread TLS slot is not nullptr";
  }
  CHECK_PTHREAD_CALL(pthread_setspecific, (pthread_key_self_, self), "create self");
  self_tls_ = self;
  self->is_started_ = true;
  return self;
}

Thread* Thread::Current() {
  if (!self_tls_ || !self_tls_->is_started_) return nullptr;
  return self_tls_;
}

void Thread::ThreadExitCallback(void* arg) {

}

Thread::Thread() : pc_(0), is_started_(false) {
  stack_ = new Stack(1024);
}

Frame* Thread::PopFrame() const {
  return stack_->PopFrame();
}
Frame* Thread::CurrentFrame() const {
  return stack_->GetCurrentFrame();
}
bool Thread::IsStackEmpty() const {
  return stack_->IsEmpty();
}
const Frame* Thread::GetFrames() const {
  return stack_->GetCurrentFrame();
}

void Thread::ClearStack() {
  stack_->Clear();
}

Frame* Thread::CreateFrame(const Method* method) {
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