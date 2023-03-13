#include "classfile/MemberInfo.h"
#include "classfile/AttributeInfo.h"
#include "instructions/base/Instruction.h"
#include "rtda/Frame.h"
#include "rtda/Thread.h"
#include "instructions/base/BytecodeReader.h"
#include "instructions/InstructionCreator.h"
#include <glog/logging.h>
#include <memory>
#include <vector>
using namespace classfile;

void interpret(std::shared_ptr<MemberInfo> methodInfo) {
  std::shared_ptr<CodeAttributeInfo> codeAttr = methodInfo->getCodeAttribute();
  u2 maxLocals = codeAttr->maxLocals;
  u2 maxStack = codeAttr->maxOperandStack;
  std::vector<u1>& codes = codeAttr->codes;
  std::shared_ptr<rtda::Thread> thread = std::make_shared<rtda::Thread>();
  std::shared_ptr<rtda::Frame> frame = std::make_shared<rtda::Frame>(thread, maxLocals, maxStack);
  thread->pushFrame(frame);
}
void loop(std::shared_ptr<rtda::Thread> thread, std::vector<u1>& byteCodes) {
  std::shared_ptr<rtda::Frame> frame = thread->popFrame();
  int32_t pc = 0;
  std::shared_ptr<instructions::BytecodeReader> codeReader = std::make_shared<instructions::BytecodeReader>(byteCodes, pc);
  while (true) {
    pc = frame->nextPC();
    thread->setPC(pc);

    codeReader->reset(byteCodes, pc);
    uint8_t opcode = codeReader->readUInt8();
    std::shared_ptr<instructions::Instruction> inst = instructions::createInstruction(opcode);
    inst->fetchOperands(codeReader);
    frame->setNextPC(codeReader->currentPc());
    LOG(INFO) << "pc = " << pc << " opcode = " << opcode;
    inst->execute(frame);
  }
}