#include "invoke_insts.h"

#include <array>
#include <runtime/oo/method.h>
#include <runtime/oo/class.h>
#include <runtime/oo/class_member.h>
#include <runtime/constant_pool.h>
#include <runtime/oo/object.h>
#include <runtime/string_pool.h>
#include "base/bytecode_reader.h"
#include "runtime/frame.h"
#include <runtime/native/native_method.h>

namespace instructions {
void InvokeMethod(runtime::Frame* frame, runtime::Method* method) {
  runtime::Thread* thread = runtime::Thread::Current();
  runtime::Frame* new_frame = thread->CreateFrame(method);
  runtime::LocalVars& vars = new_frame->GetLocalVars();
  LOG_IF(INFO, INST_DEBUG) << "method argSlotCount = " << method->GetArgSlotCount();
  for (int32_t i = method->GetArgSlotCount() - 1; i >= 0; i--) {
    vars.SetSlot(i, frame->GetOperandStack().PopSlot());
  }
  LOG_IF(INFO, INST_DEBUG) << "method name = " << method->GetName();
  //LOG_IF(INFO, INST_DEBUG) << "method descriptor = " << method->descriptor;
  LOG_IF(INFO, INST_DEBUG) << "method max_locals_ = " << method->GetMaxLocals();
  LOG_IF(INFO, INST_DEBUG) << "method max_stack_ = " << method->GetMaxStack();
  //LOG_IF(INFO, INST_DEBUG) << "method access_flags_ = " << method->access_flags_;
  //LOG_IF(INFO, INST_DEBUG) << "method code length = " << method->codes_.size();
  // if (method->IsNative()) {
  //   if (method->GetName() == "registerNatives") {
  //     native::init();
  //   }
  // }
}
void INVOKE_STATIC::Execute(runtime::Frame* frame) {
  
  const runtime::Method* method_ptr = frame->GetMethod();
  std::shared_ptr<runtime::ConstantPool> cp = method_ptr->GetClass()->GetConstantPool();
  std::shared_ptr<runtime::Constant> constant = cp->GetConstant(index_);
  std::shared_ptr<runtime::MethodRefConstant> method_ref_info = std::dynamic_pointer_cast<runtime::MethodRefConstant>(constant);
  
  runtime::Method* resolved_method = method_ref_info->ResolveMethod();
  if (!resolved_method->IsStatic()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }

  LOG_IF(INFO, INST_DEBUG) << "INVOKE_STATIC " << resolved_method->GetName()
                           << " " << resolved_method->GetDescriptor()
                           << " " << resolved_method->GetClass()->GetName()
                           << " caller " << method_ptr->GetName() << " "
                           << method_ptr->GetDescriptor() << " " << method_ptr->GetClass()->GetName();
  
  //Check class initialization
  runtime::Class* resolved_class = resolved_method->GetClass();
  if (!resolved_class->IsClinitStarted()) {
    frame->RevertNextPc();
    runtime::Class::InitClass(resolved_class);
    return;
  }
  if (resolved_method->GetName() == "println") {
    //LOG_IF(INFO, INST_DEBUG) << "hack println";
    HackPrintln(resolved_method, frame);
    return;
  }

  InvokeMethod(frame, resolved_method);
}
void INVOKE_SPECIAL::Execute(runtime::Frame* frame) {
  std::shared_ptr<runtime::Method> method_ptr = frame->GetMethod();
  std::shared_ptr<runtime::ConstantPool> cp = method_ptr->GetClass()->GetConstantPool();
  std::shared_ptr<runtime::Constant> constant = cp->GetConstant(index_);
  std::shared_ptr<runtime::MethodRefConstant> method_ref_info = std::dynamic_pointer_cast<runtime::MethodRefConstant>(constant);
  std::shared_ptr<runtime::Method> resolved_method = method_ref_info->ResolveMethod();
  runtime::Class* resolved_class = method_ref_info->ResolveClass();
  if (resolved_method->GetName() == "<init>" && resolved_method->GetClass() != resolved_class) {
    LOG(FATAL) << "java.lang.NoSuchMethodError";
  }
  if (resolved_method->IsStatic()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  void* ref = frame->GetOperandStack().GetRefFromTop(resolved_method->GetArgSlotCount() - 1);
  if (ref == nullptr) {
    LOG(FATAL) << "java.lang.NullPointerException";
  }
  runtime::Object* ref_obj = static_cast<runtime::Object*>(ref);
  if (resolved_method->IsProtected()
      && runtime::Class::IsSuperClassOf(resolved_method->GetClass(), method_ptr->GetClass())
      && resolved_method->GetClass()->GetPackageName() != method_ptr->GetClass()->GetPackageName()
      && ref_obj->GetClass() != method_ptr->GetClass()
      && !runtime::Class::IsSubClassOf(ref_obj->GetClass(), method_ptr->GetClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  std::shared_ptr<runtime::Method> method_to_be_invoked = resolved_method;
  if (method_ptr->GetClass()->IsSuper()
      && runtime::Class::IsSuperClassOf(resolved_class, method_ptr->GetClass())
      && resolved_method->GetName() != "<init>") {
    runtime::Class* super_class_ptr = method_ptr->GetClass()->GetSuperClass();
    method_to_be_invoked = super_class_ptr->LookupMethodInClass(method_ref_info->GetName(),
                                                                method_ref_info->GetDescriptor());
  }
  if (method_to_be_invoked == nullptr || method_to_be_invoked->IsAbstract()) {
    LOG(FATAL) << "java.lang.AbstractMethodError";
  }
  LOG_IF(INFO, INST_DEBUG) << "INVOKE_SPECIAL " << method_to_be_invoked->GetName()
                           << " " << method_to_be_invoked->GetDescriptor()
                           << " " << method_to_be_invoked->GetClass()->GetName();
  InvokeMethod(frame, method_to_be_invoked);
}

void INVOKE_VIRTUAL::Execute(runtime::Frame* frame) {
  std::shared_ptr<runtime::Method> method_ptr = frame->GetMethod();
  std::shared_ptr<runtime::ConstantPool> cp = method_ptr->GetClass()->GetConstantPool();
  std::shared_ptr<runtime::Constant> constant = cp->GetConstant(index_);
  std::shared_ptr<runtime::MethodRefConstant> method_ref_info = std::dynamic_pointer_cast<runtime::MethodRefConstant>(constant);
  std::shared_ptr<runtime::Method> resolved_method = method_ref_info->ResolveMethod();
  if (resolved_method->IsStatic()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  LOG_IF(INFO, INST_DEBUG) << "INVOKE_VIRTUAL " << resolved_method->GetName()
                           << " " << resolved_method->GetDescriptor()
                           << " " << resolved_method->GetClass()->GetName();
  if (resolved_method->GetName() == "println") {
    //LOG_IF(INFO, INST_DEBUG) << "hack println";
    //LOG(WARNING) << "hack println "<< frame->GetOperandStack().PopInt();
    HackPrintln(resolved_method, frame);
    return;
  }
  void* ref = frame->GetOperandStack().GetRefFromTop(resolved_method->GetArgSlotCount() - 1);
  if (ref == nullptr) {
    
    LOG(FATAL) << "java.lang.NullPointerException";
  }
  runtime::Object* ref_obj = static_cast<runtime::Object*>(ref);
  if (resolved_method->IsProtected()
      && runtime::Class::IsSuperClassOf(resolved_method->GetClass(), method_ptr->GetClass())
      && resolved_method->GetClass()->GetPackageName() != method_ptr->GetClass()->GetPackageName()
      && ref_obj->GetClass() != method_ptr->GetClass()
      && !runtime::Class::IsSubClassOf(ref_obj->GetClass(), method_ptr->GetClass())) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  std::shared_ptr<runtime::Method> method_to_be_invoked = ref_obj->GetClass()->LookupMethodInClass(
      method_ref_info->GetName(), method_ref_info->GetDescriptor());
  if (method_to_be_invoked == nullptr || method_to_be_invoked->IsAbstract()) {
    LOG(FATAL) << "java.lang.AbstractMethodError";
  }
  LOG_IF(INFO, INST_DEBUG) << "INVOKE_VIRTUAL " << method_to_be_invoked->GetName()
                           << " " << method_to_be_invoked->GetDescriptor()
                           << " " << method_to_be_invoked->GetClass()->GetName();
  InvokeMethod(frame, method_to_be_invoked);
}

void INVOKE_INTERFACE::FetchOperands(std::shared_ptr<BytecodeReader> reader) {
  index_ = uint32_t(reader->ReadUnsignedInt16());
  reader->ReadUnsignedInt8();
  reader->ReadUnsignedInt8();
}

void INVOKE_INTERFACE::Execute(runtime::Frame* frame) {
  std::shared_ptr<runtime::Method> method_ptr = frame->GetMethod();
  std::shared_ptr<runtime::ConstantPool> cp = method_ptr->GetClass()->GetConstantPool();
  std::shared_ptr<runtime::Constant> constant = cp->GetConstant(index_);
  std::shared_ptr<runtime::InterfaceMethodRefConstant> method_ref_info = std::dynamic_pointer_cast<runtime::InterfaceMethodRefConstant>(constant);
  std::shared_ptr<runtime::Method> resolved_method = method_ref_info->ResolveInterfaceMethod();
  if (resolved_method->IsStatic() || resolved_method->IsPrivate()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  void* ref = frame->GetOperandStack().GetRefFromTop(resolved_method->GetArgSlotCount() - 1);
  if (ref == nullptr) {
    LOG(FATAL) << "java.lang.NullPointerException";
  }
  auto* ref_obj = static_cast<runtime::Object*>(ref);
  if (!runtime::Class::IsImplements(ref_obj->GetClass(), method_ref_info->ResolveClass())) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  std::shared_ptr<runtime::Method> method_to_be_invoked = ref_obj->GetClass()->LookupMethodInClass(
      method_ref_info->GetName(), method_ref_info->GetDescriptor());
  if (method_to_be_invoked == nullptr || method_to_be_invoked->IsAbstract()) {
    LOG(FATAL) << "java.lang.AbstractMethodError";
  }
  if (!method_to_be_invoked->IsPublic()) {
    LOG(FATAL) << "java.lang.IllegalAccessError";
  }
  LOG_IF(INFO, INST_DEBUG) << "INVOKE_INTERFACE " << method_to_be_invoked->GetName()
                           << " " << method_to_be_invoked->GetDescriptor()
                           << " " << method_to_be_invoked->GetClass()->GetName();
  InvokeMethod(frame, method_to_be_invoked);
}

void INVOKE_NATIVE::Execute(runtime::Frame* frame) {
  std::shared_ptr<runtime::Method> method_ptr = frame->GetMethod();
  std::string class_name = method_ptr->GetClass()->GetName();
  std::string method_name = method_ptr->GetName();
  std::string method_descriptor = method_ptr->GetDescriptor();
  auto native_method = native::FindNativeMethod(class_name, method_name, method_descriptor);
  if (native_method == nullptr) {
    std::string method_info = class_name + "." + method_name + method_descriptor;
    LOG(FATAL) << "java.lang.UnsatisfiedLinkError: " << method_info;
  }
  LOG_IF(INFO, INST_DEBUG) << "INVOKE_NATIVE " << method_ptr->GetName()
                           << " " << method_ptr->GetDescriptor()
                           << " " << method_ptr->GetClass()->GetName();
  native_method(frame);
}

void HackPrintln(std::shared_ptr<runtime::Method> resolved_method, std::shared_ptr<runtime::Frame> frame) {
  auto& op_stack = frame->GetOperandStack();
  auto descriptor = resolved_method->GetDescriptor();
  if (descriptor == "(Ljava/lang/String;)V") {
    auto jStr = op_stack.PopRef();
    
    LOG(WARNING) << "hack println String "<< runtime::StringPool::javaStringToString(jStr);
    if (!resolved_method->IsStatic()) {
      //LOG(FATAL) << "hack println "<< descriptor;
      //LOG(INFO) << "popref for hack System.out.println() ";
      op_stack.PopRef();//pop out this pointer
    }
    return;
  }
  
  switch (descriptor[1]) {
    case 'Z': {
      int32_t val = op_stack.PopInt();
      if (val == 0) {
        LOG(WARNING) << "hack println "<< "false";
      } else {
        LOG(WARNING) << "hack println "<< "true";
      }
      break;
    }
    case 'B': {
      int32_t val = op_stack.PopInt();
      LOG(WARNING) << "hack println "<< int8_t(val);
      break;
    }
    case 'C': {
      int32_t val = op_stack.PopInt();
      char16_t c = static_cast<char16_t>(val);
      char16_t buf[2] = {c, u'\0'};
      LOG(WARNING) << "hack println "<< runtime::StringConstant::utf16ToUtf8(buf);
      break;
    }
    case 'S': {
      int32_t val = op_stack.PopInt();
      LOG(WARNING) << "hack println "<< int16_t(val);
      break;
    }
    case 'I': {
      LOG(WARNING) << "hack println " << op_stack.PopInt();
      break;
    }
    case 'F': {
      LOG(WARNING) << "hack println " << op_stack.PopFloat();
      break;
    }
    case 'J': {
      LOG(WARNING) << "hack println " << op_stack.PopLong();
      break;
    }
    case 'D': {
      LOG(WARNING) << "hack println " << op_stack.PopDouble();
      break;
    }
    case 'L':
    case '[': {
      LOG(WARNING) << "hack println " << op_stack.PopRef();
      break;
    }
    default: {
      LOG(FATAL) << "hack println "<< descriptor;
    }
    
  }
  if (!resolved_method->IsStatic()) {
    //LOG(FATAL) << "hack println "<< descriptor;
    //LOG(INFO) << "popref for hack System.out.println() ";
    op_stack.PopRef();//pop out this pointer
  }
}

} // namespace instructions