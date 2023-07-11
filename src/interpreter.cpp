#include "interpreter.h"
#include "runtime/slots.h"
#include <runtime/class_loader.h>
#include <runtime/oo/class.h>
#include <runtime/oo/class_member.h>
#include <runtime/oo/method.h>
#include <glog/logging.h>
#include <ios>
#include <memory>
#include <vector>
using namespace classfile;
void loop_execute(std::shared_ptr<runtime::Thread> thread) {
  
  std::shared_ptr<runtime::Frame> frame;
  
  int32_t pc = 0;
  std::shared_ptr<instructions::BytecodeReader> codeReader = std::make_shared<instructions::BytecodeReader>();
  
  while (true) {
    frame = thread->currentFrame();
    if (frame == nullptr) {
      LOG(ERROR) << "get frame failed";
    }
    pc = frame->nextPC();
    thread->setPC(pc);
    std::shared_ptr<std::vector<u1>> codes = frame->getMethod()->getCodes();
    codeReader->reset(codes, pc);
    //LOG(INFO) << "current pc = " << codeReader->currentPc();
    //will update pc
    uint8_t opcode = codeReader->readUInt8();
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
void interpret(std::shared_ptr<runtime::Method> method, const std::vector<std::string>& args) {
  
  std::shared_ptr<runtime::Thread> thread = std::make_shared<runtime::Thread>();
  
  //TODO create frame with method
  std::shared_ptr<runtime::Frame> frame = std::make_shared<runtime::Frame>(thread, method->getMaxLocals(), method->getMaxStack(), method);
  
  thread->pushFrame(frame);
  frame->getLocalVars().setRef(0, createArgsArray(args));

  loop_execute(thread);
}
void interpret(std::shared_ptr<runtime::Thread> thread) {
  loop_execute(thread);
}
runtime::Object* createArgsArray(const std::vector<std::string> &args) {
  auto stringCls = runtime::ClassLoader::getBootClassLoader(nullptr)->loadClass("java/lang/String");
  auto argsArr = stringCls->getArrayClass()->newArray(args.size());
  size_t size = args.size();
  for (size_t i = 0; i < size; i++) {
    //argsArr->setArrayElement<runtime::Object*>(i, runtime::Class::newJString(args[i]));
  }
  return argsArr;
}

