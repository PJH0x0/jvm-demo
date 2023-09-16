#pragma once

#include "base/base_insts.h"
#include "runtime/frame.h"
#include <cstdint>
#include <memory>
namespace instructions {
class WIDE : public Instruction {
  private:
  std::shared_ptr<Instruction> modified_ins_;
  public:
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override;
  void Execute(runtime::Frame* frame) override;
};

class IFNULL : public BranchInstruction {
  public:
  void Execute(runtime::Frame* frame) override;
};
class IFNONNULL : public BranchInstruction {
  public:
  void Execute(runtime::Frame* frame) override;
};

class GOTO_W : public BranchInstruction {
  public:
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override;
  void Execute(runtime::Frame* frame) override;
};
}