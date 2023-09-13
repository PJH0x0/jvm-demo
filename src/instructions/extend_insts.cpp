#include "extend_insts.h"
#include "load_insts.h"
#include "store_insts.h"
#include "arithmetic_insts.h"
#include <cstdint>
#include <memory>

namespace instructions {
void WIDE::FetchOperands(std::shared_ptr<BytecodeReader> reader) {
  uint8_t opCode = reader->ReadUnsignedInt8();
  switch (opCode) {
    case 0x15: {//iload
      std::shared_ptr<LOAD<int32_t>> iload = std::make_shared<LOAD<int32_t>>();
      iload->SetIndex(uint32_t(reader->ReadUInt16()));
      modifiedIns = iload;
    }
    
    break;
    case 0x16: {//lload
      std::shared_ptr<LOAD<int64_t>> lload = std::make_shared<LOAD<int64_t>>();
      lload->SetIndex(uint32_t(reader->ReadUInt16()));
      modifiedIns = lload;
    }
    break;
    case 0x17: {//fload
      std::shared_ptr<LOAD<float>> fload = std::make_shared<LOAD<float>>();
      fload->SetIndex(uint32_t(reader->ReadUInt16()));
      modifiedIns = fload;
    }
    break;
    case 0x18: {//dload
      std::shared_ptr<LOAD<double>> dload = std::make_shared<LOAD<double>>();
      dload->SetIndex(uint32_t(reader->ReadUInt16()));
      modifiedIns = dload;
    }
    break;
    case 0x19: {//aload
      std::shared_ptr<LOAD<void*>> aload = std::make_shared<LOAD<void*>>();
      aload->SetIndex(uint32_t(reader->ReadUInt16()));
      modifiedIns = aload;
    }
    break;
    case 0x36: {//istore
      std::shared_ptr<STORE<int32_t>> istore = std::make_shared<STORE<int32_t>>();
      istore->SetIndex(uint32_t(reader->ReadUInt16()));
      modifiedIns = istore;
    }
    break;
    case 0x37: {//lstore
      std::shared_ptr<STORE<int64_t>> lstore = std::make_shared<STORE<int64_t>>();
      lstore->SetIndex(uint32_t(reader->ReadUInt16()));
      modifiedIns = lstore;
    }
    break;
    case 0x38: {//fstore
      std::shared_ptr<STORE<float>> fstore = std::make_shared<STORE<float>>();
      fstore->SetIndex(uint32_t(reader->ReadUInt16()));
      modifiedIns = fstore;
    }
    break;
    case 0x39: {//dstore
      std::shared_ptr<STORE<double>> dstore = std::make_shared<STORE<double>>();
      dstore->SetIndex(uint32_t(reader->ReadUInt16()));
      modifiedIns = dstore;
    }
    break;
    case 0x3a: {//astore
      std::shared_ptr<STORE<void*>> astore = std::make_shared<STORE<void*>>();
      astore->SetIndex(uint32_t(reader->ReadUInt16()));
      modifiedIns = astore;
    }
    break;
    case 84: {//iinc
      std::shared_ptr<IINC> iinc = std::make_shared<IINC>();
      iinc->setIndex(uint32_t(reader->ReadUInt16()));
      iinc->setConst(int32_t(reader->ReadInt16()));
      modifiedIns = iinc;
    }
    break;
    case 0xa9: //ret
    break;
  }
}
void WIDE::Execute(std::shared_ptr<runtime::Frame> frame) {
  modifiedIns->Execute(frame);
}

void IFNULL::Execute(std::shared_ptr<runtime::Frame> frame) {
  void* ref = frame->getOperandStack().popRef();
  if (ref == nullptr) {
    branch(frame);
  }
}

void IFNONNULL::Execute(std::shared_ptr<runtime::Frame> frame) {
  void* ref = frame->getOperandStack().popRef();
  if (ref != nullptr) {
    branch(frame);
  }
}
void GOTO_W::FetchOperands(std::shared_ptr<BytecodeReader> reader) {
  offset_ = reader->readInt32();
  current_pc_ = reader->CurrentPc();
}
void GOTO_W::Execute(std::shared_ptr<runtime::Frame> frame) {
  branch(frame);
}
}