#pragma once


#include <memory>

class Command;
namespace runtime {
  class ClassLoader;
  class Thread;
  class Object;
}
namespace classpath {
  class ClassPathParser;
}
namespace heap {
  class Heap;
}
class JVM {
private:
  std::shared_ptr<Command> command_;
  heap::Heap* heap_{};
  static JVM* jvm_current_;
  explicit JVM(const std::shared_ptr<Command>& cmd);
  void InitVm();
  runtime::Object* CreateArgs();
  static void Interpret(runtime::Thread* thread);
public:
  static JVM* Current();
  static void CreateVM(const std::shared_ptr<Command>& cmd);
  void Start();
  inline heap::Heap* GetHeap() {
    return heap_;
  }
};
