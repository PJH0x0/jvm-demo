#include "extend_insts.h"
#include "load_insts.h"
#include "store_insts.h"
#include "arithmetic_insts.h"
#include <cstdint>
#include <memory>

namespace instructions {
void WIDE::FetchOperands(std::shared_ptr<BytecodeReader> reader) {
  uint8_t op_code = reader->ReadUnsignedInt8();
  switch (op_code) {
    case 0x15: {//iload
      std::shared_ptr<LOAD<int32_t>> iload = std::make_shared<LOAD<int32_t>>();
      iload->SetIndex(uint32_t(reader->ReadUnsignedInt16()));
      modified_ins_ = iload;
    }
    
    break;
    case 0x16: {//lload
      std::shared_ptr<LOAD<int64_t>> lload = std::make_shared<LOAD<int64_t>>();
      lload->SetIndex(uint32_t(reader->ReadUnsignedInt16()));
      modified_ins_ = lload;
    }
    break;
    case 0x17: {//fload
      std::shared_ptr<LOAD<float>> fload = std::make_shared<LOAD<float>>();
      fload->SetIndex(uint32_t(reader->ReadUnsignedInt16()));
      modified_ins_ = fload;
    }
    break;
    case 0x18: {//dload
      std::shared_ptr<LOAD<double>> dload = std::make_shared<LOAD<double>>();
      dload->SetIndex(uint32_t(reader->ReadUnsignedInt16()));
      modified_ins_ = dload;
    }
    break;
    case 0x19: {//aload
      std::shared_ptr<LOAD<void*>> aload = std::make_shared<LOAD<void*>>();
      aload->SetIndex(uint32_t(reader->ReadUnsignedInt16()));
      modified_ins_ = aload;
    }
    break;
    case 0x36: {//istore
      std::shared_ptr<STORE<int32_t>> istore = std::make_shared<STORE<int32_t>>();
      istore->SetIndex(uint32_t(reader->ReadUnsignedInt16()));
      modified_ins_ = istore;
    }
    break;
    case 0x37: {//lstore
      std::shared_ptr<STORE<int64_t>> lstore = std::make_shared<STORE<int64_t>>();
      lstore->SetIndex(uint32_t(reader->ReadUnsignedInt16()));
      modified_ins_ = lstore;
    }
    break;
    case 0x38: {//fstore
      std::shared_ptr<STORE<float>> fstore = std::make_shared<STORE<float>>();
      fstore->SetIndex(uint32_t(reader->ReadUnsignedInt16()));
      modified_ins_ = fstore;
    }
    break;
    case 0x39: {//dstore
      std::shared_ptr<STORE<double>> dstore = std::make_shared<STORE<double>>();
      dstore->SetIndex(uint32_t(reader->ReadUnsignedInt16()));
      modified_ins_ = dstore;
    }
    break;
    case 0x3a: {//astore
      std::shared_ptr<STORE<void*>> astore = std::make_shared<STORE<void*>>();
      astore->SetIndex(uint32_t(reader->ReadUnsignedInt16()));
      modified_ins_ = astore;
    }
    break;
    case 84: {//iinc
      std::shared_ptr<IINC> iinc = std::make_shared<IINC>();
      iinc->SetIndex(uint32_t(reader->ReadUnsignedInt16()));
      iinc->SetConst(int32_t(reader->ReadInt16()));
      modified_ins_ = iinc;
    }
    break;
    case 0xa9: //ret
    default:
    break;
  }
}
void WIDE::Execute(std::shared_ptr<runtime::Frame> frame) {
  modified_ins_->Execute(frame);
}

void IFNULL::Execute(std::shared_ptr<runtime::Frame> frame) {
  void* ref = frame->GetOperandStack().PopRef();
  if (ref == nullptr) {
    Branch(frame);
  }
}

void IFNONNULL::Execute(std::shared_ptr<runtime::Frame> frame) {
  void* ref = frame->GetOperandStack().PopRef();
  if (ref != nullptr) {
    Branch(frame);
  }
}
void GOTO_W::FetchOperands(std::shared_ptr<BytecodeReader> reader) {
  offset_ = reader->readInt32();
  current_pc_ = reader->CurrentPc();
}
void GOTO_W::Execute(std::shared_ptr<runtime::Frame> frame) {
  Branch(frame);
}
}