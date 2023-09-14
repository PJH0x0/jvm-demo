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
    frame = thread->CurrentFrame();
    if (frame == nullptr) {
      LOG(ERROR) << "get frame failed";
    }
    pc = frame->NextPc();
    thread->SetPc(pc);
    std::shared_ptr<std::vector<u1>> codes = frame->GetMethod()->GetCodes();
    codeReader->reset(codes, pc);
    //LOG(INFO) << "current pc = " << codeReader->current_pc_();
    //will update pc
    uint8_t opcode = codeReader->ReadUnsignedInt8();
    //LOG(INFO) << "opcode = " << std::hex << static_cast<int32_t>(OpCode);
    std::shared_ptr<instructions::Instruction> inst = nullptr;
    try{
      inst = instructions::CreateInstruction(opcode);
    } catch (instructions::InstNotFoundException& e) {
      LOG(FATAL) << "Unsupported OpCode: 0x" << std::hex << e.OpCode();
    }

    inst->FetchOperands(codeReader);
    frame->SetNextPc(codeReader->CurrentPc());
    //LOG(INFO) << "next pc = " << codeReader->current_pc_() << " stack = " << &(frame->GetOperandStack()) << " frame = " << frame.get();
    inst->Execute(frame);
    if (thread->IsStackEmpty()) {
      break;
    }
  }
}
void Interpret(std::shared_ptr<runtime::Method> method, const std::vector<std::string>& args) {
  
  std::shared_ptr<runtime::Thread> thread = std::make_shared<runtime::Thread>();
  
  //TODO create frame with method
  std::shared_ptr<runtime::Frame> frame = std::make_shared<runtime::Frame>(thread, method->GetMaxLocals(),
                                                                           method->GetMaxStack(), method);

  thread->PushFrame(frame);
  frame->GetLocalVars().SetRef(0, CreateArgsArray(args));

  loop_execute(thread);
}
void Interpret(std::shared_ptr<runtime::Thread> thread) {
  loop_execute(thread);
}
runtime::Object* CreateArgsArray(const std::vector<std::string> &args) {
  auto stringCls = runtime::ClassLoader::GetBootClassLoader(nullptr)->LoadClass("java/lang/String");
  auto argsArr = stringCls->GetArrayClass()->NewArray(args.size());
  size_t size = args.size();
  for (size_t i = 0; i < size; i++) {
    //argsArr->setArrayElement<runtime::Object*>(i, runtime::Class::NewJString(args[i]));
  }
  return argsArr;
}

