#pragma once


#include <memory>

class Command;
namespace runtime {
  class ClassLoader;
  class Thread;
  class Object;
}
namespace heap {
  class Heap;
}
class JVM {
  private:
  std::shared_ptr<Command> command_;
  runtime::ClassLoader* class_loader_;
  runtime::Thread* main_thread_;
  heap::Heap* heap_;
  static JVM* jvm_current_;
  public:
  static inline JVM* current() {
    return jvm_current_;
  }
  heap::Heap* GetHeap() {
    return heap_;
  }
  JVM(std::shared_ptr<Command> cmd);
  void Start();
  void InitVm();
  runtime::Object* CreateArgsArray();
};
