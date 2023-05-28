#pragma once
#include "base/base_instructions.h"
namespace instructions {
void invokeMethod(std::shared_ptr<rtda::Frame> frame, std::shared_ptr<rtda::Method> method);
struct INVOKE_STATIC : public Index16Instruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};
struct INVOKE_SPECIAL : public Index16Instruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};
struct INVOKE_VIRTUAL : public Index16Instruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};
struct INVOKE_INTERFACE : public Instruction {
  private:
  uint32_t index;
  public:
  void fetchOperands(std::shared_ptr<BytecodeReader> reader) override;
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};

struct INVOKE_NATIVE : public NoOperandsInstruction {
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};

void hackPrintln(std::shared_ptr<rtda::Method>, std::shared_ptr<rtda::Frame> frame);

} // namespace instructions