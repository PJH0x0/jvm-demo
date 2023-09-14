#pragma once

#include "base/base_insts.h"
#include <cstdint>
#include <vector>

namespace instructions {
class GOTO : public BranchInstruction {
  public:
  void Execute(std::shared_ptr<runtime::Frame> frame) override {
    Branch(frame);
  }
};

class TABLE_SWITCH : public BranchInstruction {
  private:
  int32_t default_offset_;
  int32_t low_;//low is lowest of case index table value
  int32_t high_;// highest of case index table value
  std::vector<int32_t> offset_table_;
  
  public:
  TABLE_SWITCH() : default_offset_(0), low_(0), high_(0){}
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override;
  void Execute(std::shared_ptr<runtime::Frame> frame) override;
};

class LOOKUP_SWITCH : public BranchInstruction {
  private:
  int32_t default_offset_;
  int32_t npairs_;
  std::vector<int32_t> offset_table_;
  public:
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override;
  void Execute(std::shared_ptr<runtime::Frame> frame) override;
};
}