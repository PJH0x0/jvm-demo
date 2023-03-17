#include "interpreter.h"
#include <glog/logging.h>
#include <memory>
#include <vector>
using namespace classfile;
void loop_execute(std::shared_ptr<rtda::Thread> thread, std::vector<u1>& byteCodes) {
  
  std::shared_ptr<rtda::Frame> frame = thread->popFrame();
  if (frame == nullptr) {
    LOG(ERROR) << "popFrame failed";
  }
  int32_t pc = 0;
  std::shared_ptr<instructions::BytecodeReader> codeReader = std::make_shared<instructions::BytecodeReader>(byteCodes, pc);
  
  while (true) {
    pc = frame->nextPC();
    thread->setPC(pc);

    codeReader->reset(byteCodes, pc);
    LOG(INFO) << "current pc = " << codeReader->currentPc();
    //will update pc
    uint8_t opcode = codeReader->readUInt8();
    std::shared_ptr<instructions::Instruction> inst = nullptr;
    try{
      inst = instructions::createInstruction(opcode);
    } catch (instructions::InstNotFoundException& e) {
      rtda::LocalVars& vars = frame->getLocalVars();
      rtda::OperandStack& stack = frame->getOperandStack();
      LOG(INFO) << "Unsupported opcode: 0x" << e.opcode();
      LOG(INFO) << "localvars = " << vars.getInt(0);
      LOG(INFO) << "localvars = " << vars.getInt(1);
      LOG(INFO) << "localvars = " << vars.getInt(2);
      LOG(INFO) << "operandStack = " << stack.popInt();
      LOG(FATAL) << "Unsupported opcode: 0x" << e.opcode();
    }
    
    inst->fetchOperands(codeReader);
    frame->setNextPC(codeReader->currentPc());
    LOG(INFO) << "next pc = " << codeReader->currentPc() << " opcode = " << std::hex << static_cast<int32_t>(opcode);
    inst->execute(frame);
  }
}
void interpret(std::shared_ptr<MemberInfo> methodInfo) {
  
  std::shared_ptr<CodeAttributeInfo> codeAttr = methodInfo->getCodeAttribute();
  LOG(INFO) << "getCodeAttribute success";
  if (nullptr == codeAttr) {
    LOG(FATAL) << "getCodeAttribute nullptr";
  }
  uint16_t maxLocals = codeAttr->maxLocals;
  uint16_t maxStack = codeAttr->maxOperandStack;
  LOG(INFO) << "maxLocals = " << maxLocals << " maxStack = " << maxStack;
  std::vector<u1>& codes = codeAttr->codes;
  
  std::shared_ptr<rtda::Thread> thread = std::make_shared<rtda::Thread>();
  
  std::shared_ptr<rtda::Frame> frame = std::make_shared<rtda::Frame>(thread, maxLocals, maxStack);
  
  thread->pushFrame(frame);
  LOG(INFO) << "codes size = " << codes.size();

  loop_execute(thread, codes);
}