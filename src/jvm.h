#pragma once

#include "command.h"
#include <memory>
#include "runtime/class_loader.h"
#include <runtime/oo/object.h>
class JVM {
  private:
  std::shared_ptr<Command> cmd;
  std::shared_ptr<runtime::ClassLoader> classLoader;
  std::shared_ptr<runtime::Thread> mainThread;
  public:
  JVM(std::shared_ptr<Command> cmd);
  void start();
  void initVM();
  runtime::Object* createArgsArray();
};