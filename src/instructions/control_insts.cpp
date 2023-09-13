#include "control_insts.h"
#include <cstdint>

namespace instructions {
void TABLE_SWITCH::FetchOperands(std::shared_ptr<BytecodeReader> reader) {
  reader->skipPadding();
  defaultOffset = reader->readInt32();
  low = reader->readInt32();
  high = reader->readInt32();
  reader->readInt32s(offsetTable, high - low + 1);
  current_pc_ = reader->CurrentPc();
}
void TABLE_SWITCH::Execute(std::shared_ptr<runtime::Frame> frame) {
  int32_t index = frame->getOperandStack().popInt();
  int32_t offset;
  if (index >= low && index <= high) {
    offset = offsetTable[index-low];
  } else {
    offset = defaultOffset;
  }
  branch(frame);
}

void LOOKUP_SWITCH::FetchOperands(std::shared_ptr<BytecodeReader> reader) {
  reader->skipPadding();
  defaultOffset = reader->readInt32();
  npairs = reader->readInt32();
  reader->readInt32s(offsetTable, npairs * 2);
  current_pc_ = reader->CurrentPc();
}

void LOOKUP_SWITCH::Execute(std::shared_ptr<runtime::Frame> frame) {
  int32_t key = frame->getOperandStack().popInt();
  int32_t offset;
  for (int i = 0; i < npairs * 2; i += 2) {
    if (offsetTable[i] == key) {
      offset = offsetTable[i+1];
      branch(frame);
      return;
    }
  }
  offset = defaultOffset;
  branch(frame);
}
}