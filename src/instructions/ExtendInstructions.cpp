#include "ExtendInstructions.h"
#include "LoadInstructions.h"
#include "StoreInstructions.h"
#include "ArithmeticInstructions.h"
#include <cstdint>
#include <memory>

namespace instructions {
void WIDE::fetchOperands(std::shared_ptr<BytecodeReader> reader) {
  uint8_t opCode = reader->readUInt8();
  switch (opCode) {
    case 0x15: {//iload
      std::shared_ptr<LOAD<int32_t>> iload = std::make_shared<LOAD<int32_t>>();
      iload->setIndex(uint32_t(reader->readUInt16()));
      modifiedIns = iload;
    }
    
    break;
    case 0x16: {//lload
      std::shared_ptr<LOAD<int64_t>> lload = std::make_shared<LOAD<int64_t>>();
      lload->setIndex(uint32_t(reader->readUInt16()));
      modifiedIns = lload;
    }
    break;
    case 0x17: {//fload
      std::shared_ptr<LOAD<float>> fload = std::make_shared<LOAD<float>>();
      fload->setIndex(uint32_t(reader->readUInt16()));
      modifiedIns = fload;
    }
    break;
    case 0x18: {//dload
      std::shared_ptr<LOAD<double>> dload = std::make_shared<LOAD<double>>();
      dload->setIndex(uint32_t(reader->readUInt16()));
      modifiedIns = dload;
    }
    break;
    case 0x19: {//aload
      std::shared_ptr<LOAD<void*>> aload = std::make_shared<LOAD<void*>>();
      aload->setIndex(uint32_t(reader->readUInt16()));
      modifiedIns = aload;
    }
    break;
    case 0x36: {//istore
      std::shared_ptr<STORE<int32_t>> istore = std::make_shared<STORE<int32_t>>();
      istore->setIndex(uint32_t(reader->readUInt16()));
      modifiedIns = istore;
    }
    break;
    case 0x37: {//lstore
      std::shared_ptr<STORE<int64_t>> lstore = std::make_shared<STORE<int64_t>>();
      lstore->setIndex(uint32_t(reader->readUInt16()));
      modifiedIns = lstore;
    }
    break;
    case 0x38: {//fstore
      std::shared_ptr<STORE<float>> fstore = std::make_shared<STORE<float>>();
      fstore->setIndex(uint32_t(reader->readUInt16()));
      modifiedIns = fstore;
    }
    break;
    case 0x39: {//dstore
      std::shared_ptr<STORE<double>> dstore = std::make_shared<STORE<double>>();
      dstore->setIndex(uint32_t(reader->readUInt16()));
      modifiedIns = dstore;
    }
    break;
    case 0x3a: {//astore
      std::shared_ptr<STORE<void*>> astore = std::make_shared<STORE<void*>>();
      astore->setIndex(uint32_t(reader->readUInt16()));
      modifiedIns = astore;
    }
    break;
    case 84: {//iinc
      std::shared_ptr<IINC> iinc = std::make_shared<IINC>();
      iinc->setIndex(uint32_t(reader->readUInt16()));
      iinc->setConst(int32_t(reader->readInt16()));
      modifiedIns = iinc;
    }
    break;
    case 0xa9: //ret
    break;
  }
}
void WIDE::execute(std::shared_ptr<rtda::Frame> frame) {
  modifiedIns->execute(frame);
}

void IFNULL::execute(std::shared_ptr<rtda::Frame> frame) {
  void* ref = frame->getOperandStack().popRef();
  if (ref == nullptr) {
    branch();
  }
}

void IFNONNULL::execute(std::shared_ptr<rtda::Frame> frame) {
  void* ref = frame->getOperandStack().popRef();
  if (ref != nullptr) {
    branch();
  }
}
void GOTO_W::fetchOperands(std::shared_ptr<BytecodeReader> reader) {
  offset = reader->readInt32();
  currentPc = reader->currentPc();
}
void GOTO_W::execute(std::shared_ptr<rtda::Frame> frame) {
  branch();
}
}