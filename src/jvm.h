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
  std::shared_ptr<Command> cmd;
  std::shared_ptr<runtime::ClassLoader> classLoader;
  std::shared_ptr<runtime::Thread> mainThread;
  heap::Heap* gHeap;
  static JVM* jvmCurrent;
  public:
  static inline JVM* current() {
    return jvmCurrent;
  }
  heap::Heap* getHeap() {
    return gHeap;
  }
  JVM(std::shared_ptr<Command> cmd);
  void start();
  void initVM();
  runtime::Object* createArgsArray();
};
