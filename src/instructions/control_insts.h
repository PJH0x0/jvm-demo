#pragma once

#include "base/base_insts.h"
#include <cstdint>
#include <vector>

namespace instructions {
class GOTO : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    branch(frame);
  }
};

class TABLE_SWITCH : public BranchInstruction {
  private:
  int32_t defaultOffset;
  int32_t low;//low is lowest of case index table value
  int32_t high;// highest of case index table value
  std::vector<int32_t> offsetTable;
  
  public:
  TABLE_SWITCH() {}
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override;
  void Execute(std::shared_ptr<runtime::Frame> frame) override;
};

class LOOKUP_SWITCH : public BranchInstruction {
  private:
  int32_t defaultOffset;
  int32_t npairs;
  std::vector<int32_t> offsetTable;
  public:
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override;
  void Execute(std::shared_ptr<runtime::Frame> frame) override;
};
}