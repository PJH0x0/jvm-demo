#pragma once
#include "base/base_insts.h"
namespace instructions {
void invokeMethod(std::shared_ptr<runtime::Frame> frame, std::shared_ptr<runtime::Method> method);
struct INVOKE_STATIC : public Index16Instruction {
  void Execute(std::shared_ptr<runtime::Frame> frame) override;
};
struct INVOKE_SPECIAL : public Index16Instruction {
  void Execute(std::shared_ptr<runtime::Frame> frame) override;
};
struct INVOKE_VIRTUAL : public Index16Instruction {
  void Execute(std::shared_ptr<runtime::Frame> frame) override;
};
struct INVOKE_INTERFACE : public Instruction {
  private:
  uint32_t index;
  public:
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override;
  void Execute(std::shared_ptr<runtime::Frame> frame) override;
};

struct INVOKE_NATIVE : public NoOperandsInstruction {
  void Execute(std::shared_ptr<runtime::Frame> frame) override;
};

void hackPrintln(std::shared_ptr<runtime::Method>, std::shared_ptr<runtime::Frame> frame);

} // namespace instructions