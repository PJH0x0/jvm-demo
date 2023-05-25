#pragma once

#include "base/base_instructions.h"

namespace instructions {
class NEW : public Index16Instruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};
class NEW_ARRAY : public Instruction {
  private:
  uint8_t mAtype;
  public:
  void fetchOperands(std::shared_ptr<BytecodeReader> reader) override;
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};
class ANEW_ARRAY : public Index16Instruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};

class ARRAY_LENGTH : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};
class PUT_STATIC : public Index16Instruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};
class GET_STATIC : public Index16Instruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};
class GET_FIELD : public Index16Instruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};
class PUT_FIELD : public Index16Instruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};
class INSTANCE_OF : public Index16Instruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};
class CHECK_CAST : public Index16Instruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};
class LDC : public Index8Instruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};
class LDC_W : public Index16Instruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};
class LDC2_W : public Index16Instruction {
  public:
  void execute(std::shared_ptr<rtda::Frame> frame) override;
};

static void _ldc(std::shared_ptr<rtda::Frame> frame, uint32_t index);

}