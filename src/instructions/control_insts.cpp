#include "control_insts.h"
#include <cstdint>

namespace instructions {
void TABLE_SWITCH::FetchOperands(std::shared_ptr<BytecodeReader> reader) {
  reader->skipPadding();
  default_offset_ = reader->readInt32();
  low_ = reader->readInt32();
  high_ = reader->readInt32();
  reader->readInt32s(offset_table_, high_ - low_ + 1);
  current_pc_ = reader->CurrentPc();
}
void TABLE_SWITCH::Execute(std::shared_ptr<runtime::Frame> frame) {
  int32_t index = frame->GetOperandStack().PopInt();
  int32_t offset;
  if (index >= low_ && index <= high_) {
    offset = offset_table_[index - low_];
  } else {
    offset = default_offset_;
  }
  Branch(frame);
}

void LOOKUP_SWITCH::FetchOperands(std::shared_ptr<BytecodeReader> reader) {
  reader->skipPadding();
  default_offset_ = reader->readInt32();
  npairs_ = reader->readInt32();
  reader->readInt32s(offset_table_, npairs_ * 2);
  current_pc_ = reader->CurrentPc();
}

void LOOKUP_SWITCH::Execute(std::shared_ptr<runtime::Frame> frame) {
  int32_t key = frame->GetOperandStack().PopInt();
  int32_t offset;
  for (int i = 0; i < npairs_ * 2; i += 2) {
    if (offset_table_[i] == key) {
      offset = offset_table_[i + 1];
      Branch(frame);
      return;
    }
  }
  offset = default_offset_;
  Branch(frame);
}
}