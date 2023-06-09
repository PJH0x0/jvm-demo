#pragma once

#include "command.h"
#include <memory>
#include "rtda/heap/class_loader.h"
#include "rtda/heap/object.h"
class JVM {
  private:
  std::shared_ptr<Command> cmd;
  std::shared_ptr<rtda::ClassLoader> classLoader;
  std::shared_ptr<rtda::Thread> mainThread;
  public:
  JVM(std::shared_ptr<Command> cmd);
  void start();
  void initVM();
  rtda::Object* createArgsArray();
};