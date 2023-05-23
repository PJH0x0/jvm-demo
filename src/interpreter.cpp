#include "interpreter.h"
#include <rtda/heap/class.h>
#include <rtda/heap/class_member.h>
#include <rtda/heap/method.h>
#include <glog/logging.h>
#include <ios>
#include <memory>
#include <vector>
using namespace classfile;
void loop_execute(std::shared_ptr<rtda::Thread> thread, std::vector<u1>& byteCodes) {
  
  std::shared_ptr<rtda::Frame> frame;
  
  int32_t pc = 0;
  std::shared_ptr<instructions::BytecodeReader> codeReader = std::make_shared<instructions::BytecodeReader>(byteCodes, pc);
  
  while (true) {
    frame = thread->currentFrame();
    if (frame == nullptr) {
      LOG(ERROR) << "get frame failed";
    }
    pc = frame->nextPC();
    thread->setPC(pc);

    codeReader->reset(frame->getMethod()->codes, pc);
    if (frame->getMethod()->mName == "main") {
      LOG(ERROR) << "main pc = " << codeReader->currentPc();
    }
    //LOG(INFO) << "current pc = " << codeReader->currentPc();
    //will update pc
    uint8_t opcode = codeReader->readUInt8();
    if (frame->getMethod()->mName == "main") {
      LOG(ERROR) << "main opcode = " << std::hex << static_cast<int32_t>(opcode);
    }
    //LOG(INFO) << "opcode = " << std::hex << static_cast<int32_t>(opcode);
    std::shared_ptr<instructions::Instruction> inst = nullptr;
    try{
      inst = instructions::createInstruction(opcode);
    } catch (instructions::InstNotFoundException& e) {
      LOG(FATAL) << "Unsupported opcode: 0x" << std::hex << e.opcode();
    }
    
    inst->fetchOperands(codeReader);
    frame->setNextPC(codeReader->currentPc());
    //LOG(INFO) << "next pc = " << codeReader->currentPc() << " stack = " << &(frame->getOperandStack()) << " frame = " << frame.get();
    inst->execute(frame);
    if (thread->isStackEmpty()) {
      break;
    }
  }
}
void interpret(std::shared_ptr<rtda::Method> method) {
  std::vector<u1>& codes = method->codes;
  
  std::shared_ptr<rtda::Thread> thread = std::make_shared<rtda::Thread>();
  
  //TODO create frame with method
  std::shared_ptr<rtda::Frame> frame = std::make_shared<rtda::Frame>(thread, method->maxLocals, method->maxStack, method);
  
  thread->pushFrame(frame);
  LOG(INFO) << "codes size = " << codes.size();

  loop_execute(thread, codes);
}
