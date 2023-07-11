#pragma once

#include "base/base_instructions.h"
#include <vector>
#include <runtime/oo/class.h>

namespace instructions {

class ATHROW : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override;
};

class NEW : public Index16Instruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override;
};
class NEW_ARRAY : public Instruction {
  private:
  uint8_t mAtype;
  public:
  void fetchOperands(std::shared_ptr<BytecodeReader> reader) override;
  void execute(std::shared_ptr<runtime::Frame> frame) override;
};
class ANEW_ARRAY : public Index16Instruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override;
};

class ARRAY_LENGTH : public NoOperandsInstruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override;
};
class MULTI_ANEW_ARRAY : public Instruction {
  private:
  uint16_t mIndex;
  uint8_t mDimensions;
  public:
  void fetchOperands(std::shared_ptr<BytecodeReader> reader) override;
  void execute(std::shared_ptr<runtime::Frame> frame) override;

};
class PUT_STATIC : public Index16Instruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override;
};
class GET_STATIC : public Index16Instruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override;
};
class GET_FIELD : public Index16Instruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override;
};
class PUT_FIELD : public Index16Instruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override;
};
class INSTANCE_OF : public Index16Instruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override;
};
class CHECK_CAST : public Index16Instruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override;
};
class LDC : public Index8Instruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override;
};
class LDC_W : public Index16Instruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override;
};
class LDC2_W : public Index16Instruction {
  public:
  void execute(std::shared_ptr<runtime::Frame> frame) override;
};

static void _ldc(std::shared_ptr<runtime::Frame> frame, uint32_t index);
static void popAndCheckCounts(runtime::OperandStack&, uint32_t, std::vector<int32_t>&);
static runtime::Object* newMultiDimensionalArray(std::vector<int32_t>&, std::shared_ptr<runtime::Class>);

}