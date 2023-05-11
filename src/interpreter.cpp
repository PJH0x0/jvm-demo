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
    //LOG(INFO) << "current pc = " << codeReader->currentPc();
    //will update pc
    uint8_t opcode = codeReader->readUInt8();
    //LOG(INFO) << "opcode = " << std::hex << static_cast<int32_t>(opcode);
    std::shared_ptr<instructions::Instruction> inst = nullptr;
    try{
      inst = instructions::createInstruction(opcode);
    } catch (instructions::InstNotFoundException& e) {
      rtda::LocalVars& vars = frame->getLocalVars();
      rtda::OperandStack& stack = frame->getOperandStack();
      LOG(INFO) << "localvars = " << vars.getInt(0);
      LOG(INFO) << "localvars = " << vars.getInt(1);
      LOG(INFO) << "localvars = " << vars.getInt(2);
      //LOG(INFO) << "operandStack = " << stack.popInt();
      LOG(FATAL) << "Unsupported opcode: 0x" << std::hex << e.opcode();
    }
    
    inst->fetchOperands(codeReader);
    frame->setNextPC(codeReader->currentPc());
    //LOG(INFO) << "next pc = " << codeReader->currentPc() << " stack = " << &(frame->getOperandStack()) << " frame = " << frame.get();
    inst->execute(frame);
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
void invokeMethod(std::shared_ptr<rtda::Frame> frame, std::shared_ptr<rtda::Method> method) {
  std::shared_ptr<rtda::Thread> thread = frame->getThread();
  std::shared_ptr<rtda::Frame> newFrame = std::make_shared<rtda::Frame>(thread, method->maxLocals, method->maxStack, method);
  thread->pushFrame(newFrame);
  rtda::LocalVars& vars = newFrame->getLocalVars();
  for (int32_t i = method->mArgSlotCount - 1; i >= 0; i--) {
    vars.setSlot(i, frame->getOperandStack().popSlot());
  }
  //LOG(INFO) << "method name = " << method->name;
  //LOG(INFO) << "method descriptor = " << method->descriptor;
  //LOG(INFO) << "method maxLocals = " << method->maxLocals;
  //LOG(INFO) << "method maxStack = " << method->maxStack;
  //LOG(INFO) << "method accessFlags = " << method->accessFlags;
  //LOG(INFO) << "method argSlotCount = " << method->argSlotCount();
  //LOG(INFO) << "method code length = " << method->codes.size();
}