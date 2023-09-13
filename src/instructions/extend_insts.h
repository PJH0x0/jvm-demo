#pragma once

#include "base/base_insts.h"
#include <cstdint>
#include <memory>
namespace instructions {
class WIDE : public Instruction {
  private:
  std::shared_ptr<Instruction> modifiedIns;
  public:
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override;
  void Execute(std::shared_ptr<runtime::Frame> frame) override;
};

class IFNULL : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override;
};
class IFNONNULL : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override;
};

class GOTO_W : public BranchInstruction {
  public:
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override;
  void Execute(std::shared_ptr<runtime::Frame> frame) override;
};
}