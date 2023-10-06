#include "jvm.h"
#include "command.h"
#include <instructions/instruction_factory.h>
#include <runtime/native/native_method.h>
#include <instructions/base/bytecode_reader.h>
#include <instructions/base/base_insts.h>
#include <classpath/class_path_parser.h>
#include <runtime/frame.h>
#include <runtime/thread.h>
#include <runtime/oo/class.h>
#include <runtime/alloc/heap.h>
#include <runtime/oo/object.h>
#include <runtime/oo/method.h>
#include <runtime/class_loader.h>

JVM* JVM::jvm_current_ = nullptr;
static std::once_flag jvm_once_flag;

void JVM::CreateVM(const std::shared_ptr<Command>& cmd) {
  std::call_once(jvm_once_flag, [&]{
    jvm_current_ = new JVM(cmd);
  });
}
JVM* JVM::Current() {
  return jvm_current_;
}

JVM::JVM(const std::shared_ptr<Command>& cmd) : command_(cmd) {
  //Start();
  auto class_path_parser = std::make_shared<classpath::ClassPathParser>(command_->GetJrePath(), command_->GetAppClassPath());
  runtime::ClassLoader::CreateBootClassLoader(class_path_parser->GetBootClassReader());
  runtime::ClassLoader::CreateExtClassLoader(class_path_parser->GetExtClassReader());
  runtime::ClassLoader::CreateBaseClassLoader(class_path_parser->GetAppClassReader());
  heap_ = new heap::Heap(1024 * 1024 * 512);
  native::init();
}
void JVM::Start() {
  //InitVm();
  auto main_thread = runtime::Thread::Create();
  auto boot_class_loader = runtime::ClassLoader::GetBootClassLoader();
  boot_class_loader->LoadBasicClass();
  boot_class_loader->LoadPrimitiveClasses();
  std::string class_name = command_->GetClassName();
  runtime::Class* main_class = runtime::ClassLoader::GetBaseClassLoader()->LoadClass(class_name);
  if (main_class == nullptr) {
    LOG(ERROR) << "main class not found";
    return;
  }
  auto main_method = main_class->GetMainMethod();
  if (main_method == nullptr) {
    LOG(ERROR) << "main method not found";
    return;
  }
  runtime::Frame* frame = runtime::Thread::Current()->CreateFrame(main_method);
  frame->GetLocalVars().SetRef(0, CreateArgs());
  Interpret(main_thread);
}
runtime::Object* JVM::CreateArgs() {
  auto string_class = runtime::ClassLoader::GetBootClassLoader()->LoadClass("java/lang/String");
  size_t size = command_->GetArgs().size();
  auto args_arr = string_class->GetArrayClass()->NewArray(size);
  for (size_t i = 0; i < size; i++) {
    //args_arr->setArrayElement<runtime::Object*>(i, runtime::Class::NewJString(cmd->args[i]));
  }
  return args_arr;
}
void JVM::InitVm() {
//  std::string class_name = "sun/misc/VM";
//  runtime::Class* vm_class_ptr = boot_class_loader_->LoadClass(class_name);
//  std::shared_ptr<runtime::Method> initialize_method = vm_class_ptr->GetStaticMethod("initialize", "()V");
//  std::shared_ptr<runtime::Frame> frame = std::make_shared<runtime::Frame>(main_thread_,
//                                                                           initialize_method->GetMaxLocals(),
//                                                                           initialize_method->GetMaxStack(),
//                                                                           initialize_method);
//  main_thread_->PushFrame(frame);
//  Interpret(main_thread_);
}

static void LoopExecute(runtime::Thread* thread) {

  runtime::Frame* frame;

  int32_t pc = 0;
  std::shared_ptr<instructions::BytecodeReader> codeReader = std::make_shared<instructions::BytecodeReader>();

  while (true) {
    frame = thread->CurrentFrame();
    if (frame == nullptr) {
      LOG(ERROR) << "get frame failed";
    }
    pc = frame->NextPc();
    thread->SetPc(pc);
    auto codes = frame->GetMethod()->GetCodes();
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
void JVM::Interpret(runtime::Thread* thread) {
  LoopExecute(thread);
}




