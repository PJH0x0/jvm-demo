#include "control_instructions.h"
#include <cstdint>

namespace instructions {
void TABLE_SWITCH::fetchOperands(std::shared_ptr<BytecodeReader> reader) {
  reader->skipPadding();
  defaultOffset = reader->readInt32();
  low = reader->readInt32();
  high = reader->readInt32();
  reader->readInt32s(offsetTable, high - low + 1);
  currentPc = reader->currentPc();
}
void TABLE_SWITCH::execute(std::shared_ptr<rtda::Frame> frame) {
  int32_t index = frame->getOperandStack().popInt();
  int32_t offset;
  if (index >= low && index <= high) {
    offset = offsetTable[index-low];
  } else {
    offset = defaultOffset;
  }
  branch(frame);
}

void LOOKUP_SWITCH::fetchOperands(std::shared_ptr<BytecodeReader> reader) {
  reader->skipPadding();
  defaultOffset = reader->readInt32();
  npairs = reader->readInt32();
  reader->readInt32s(offsetTable, npairs * 2);
  currentPc = reader->currentPc();
}

void LOOKUP_SWITCH::execute(std::shared_ptr<rtda::Frame> frame) {
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