#pragma once
#include "base/base_insts.h"
#include "runtime/frame.h"

namespace instructions {
void InvokeMethod(runtime::Frame* frame, const runtime::Method* method);
struct INVOKE_STATIC : public Index16Instruction {
  void Execute(runtime::Frame* frame) override;
};
struct INVOKE_SPECIAL : public Index16Instruction {
  void Execute(runtime::Frame* frame) override;
};
struct INVOKE_VIRTUAL : public Index16Instruction {
  void Execute(runtime::Frame* frame) override;
};
struct INVOKE_INTERFACE : public Instruction {
  private:
  uint32_t index_;
  public:
  void FetchOperands(std::shared_ptr<BytecodeReader> reader) override;
  void Execute(runtime::Frame* frame) override;
};

struct INVOKE_NATIVE : public NoOperandsInstruction {
  void Execute(runtime::Frame* frame) override;
};

void HackPrintln(const runtime::Method* method, runtime::Frame* frame);

} // namespace instructions