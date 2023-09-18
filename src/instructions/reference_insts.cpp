#include "reference_insts.h"
#include "runtime/frame.h"
#include <cassert>
#include <cstdint>
#include <runtime/oo/object.h>
#include <memory>
#include <runtime/alloc/heap.h>
#include <runtime/oo/method.h>
#include <runtime/oo/field.h>
#include <runtime/constant_pool.h>
#include <runtime/operand_stack.h>
#include <runtime/oo/class.h>
#include <runtime/thread.h>
#include <runtime/string_pool.h>
#include <runtime/native/java_lang_Throwable.h>
#include <jvm.h>

namespace instructions {
static bool FindAndGotoExceptionHandler(const runtime::Thread* thread,
                                        runtime::Object* ex) {
  while (true) {
    auto frame = thread->CurrentFrame();
    auto pc = frame->NextPc() - 1;
    auto handler_pc = frame->GetMethod()->FindExceptionHandler(ex->GetClass(), pc);
    if (handler_pc > 0) {
      auto& stack = frame->GetOperandStack();
      stack.Clear();
      stack.PushRef(ex);
      frame->SetNextPc(handler_pc);
      return true;
    }
    thread->PopFrame();
    if (thread->IsStackEmpty()) {
      break;
    }
  }
  return false;
}

static void HandleUncaughtException(runtime::Thread* thread, runtime::Object* ex) {
  thread->ClearStack();
  auto j_msg = ex->GetRefVar("detailMessage", "Ljava/lang/String;");
  auto c_msg = runtime::StringPool::javaStringToString(j_msg);
  std::cout << ex->GetClass()->GetJavaName() << ": " << c_msg << std::endl;
  //auto stes = ex->GetRefVar("stackTrace", "[Ljava/lang/StackTraceElement;");
  //auto stes_arr = static_cast<std::vector<std::shared_ptr<native::StackTraceElement>>*>(ex->getExtra());
  auto stes_arr = new std::vector<std::shared_ptr<native::StackTraceElement>>();
  if (stes_arr == nullptr) {
    //stes_arr = native::CreateStackTraceElements(ex, thread);
    LOG(FATAL) << "stes_arr is nullptr";
  }
  for (auto ste : *stes_arr) {
    auto file_name = ste->GetFileName();
    auto class_name = ste->GetClassName();
    auto method_name = ste->GetMethodName();
    auto line_number = ste->GetLineNumber();
    std::cout << "\tat " << class_name << "."
              << method_name << "("
              << file_name << ":"
              << line_number << ")" << std::endl;
  }

}

void ATHROW::Execute(runtime::Frame* frame) {
  auto ex = frame->GetOperandStack().PopRef();
  if (ex == nullptr) {
    throw std::runtime_error("java.lang.NullPointerException");
  }
  auto thread = runtime::Thread::Current();
  if (!FindAndGotoExceptionHandler(thread, ex)) {
    HandleUncaughtException(thread, ex);
  }
}
void NEW::Execute(runtime::Frame* frame) {
  auto method = frame->GetMethod();
  auto cp = method->GetClass()->GetConstantPool();
  auto class_ref_constant = dynamic_cast<runtime::ClassRefConstant*>(cp->GetConstant(index_));
  assert(class_ref_constant == nullptr);
  auto class_ptr = class_ref_constant->ResolveClass();
  //TODO check class init
  if (!class_ptr->IsClinitStarted()) {
    frame->RevertNextPc();
    runtime::Class::InitClass(class_ptr);
    return;
  }
  if (class_ptr->IsInterface() || class_ptr->IsAbstract()) {
    throw std::runtime_error("java.lang.InstantiationError");
  }
  
  runtime::Object* ref = JVM::current()->GetHeap()->AllocObject(runtime::Thread::Current(), class_ptr,
                                                                class_ptr->ObjectSize());
  frame->GetOperandStack().PushRef(ref);
}
void NEW_ARRAY::FetchOperands(std::shared_ptr<BytecodeReader> reader) {
  array_type_ = reader->ReadUnsignedInt8();
}
void NEW_ARRAY::Execute(runtime::Frame* frame) {
  auto& stack = frame->GetOperandStack();
  auto count = stack.PopInt();
  if (count < 0) {
    throw std::runtime_error("java.lang.NegativeArraySizeException");
  }
  auto arr_class_ref = runtime::Class::GetPrimitiveArrayClass(array_type_);
  auto arr = arr_class_ref->NewArray(count);
  stack.PushRef(arr);
}
void ANEW_ARRAY::Execute(runtime::Frame* frame) {
  auto cp = frame->GetMethod()->GetClass()->GetConstantPool();
  auto class_constant = dynamic_cast<runtime::ClassRefConstant*>(cp->GetConstant(index_));
  assert(class_constant == nullptr);
  auto class_ref = class_constant->ResolveClass();
  auto& stack = frame->GetOperandStack();
  auto count = stack.PopInt();
  if (count < 0) {
    throw std::runtime_error("java.lang.NegativeArraySizeException");
  }
  auto arr_class_ref = class_ref->GetArrayClass();
  auto arr = arr_class_ref->NewArray(count);
  stack.PushRef(arr);
}
void ARRAY_LENGTH::Execute(runtime::Frame* frame) {
  auto& stack = frame->GetOperandStack();
  runtime::Object* arr_ref = stack.PopRef();
  if (arr_ref == nullptr) {
    throw std::runtime_error("java.lang.NullPointerException");
  }
  // auto arrLen = arr_ref->arrayLength();
  // stack.PushInt(arrLen);
}

void MULTI_ANEW_ARRAY::FetchOperands(std::shared_ptr<BytecodeReader> reader) {
  index_ = reader->ReadUnsignedInt16();
  dimensions_ = reader->ReadUnsignedInt8();
}
void MULTI_ANEW_ARRAY::Execute(runtime::Frame* frame) {
  auto cp = frame->GetMethod()->GetClass()->GetConstantPool();
  auto class_ref_constant = dynamic_cast<runtime::ClassRefConstant*>(cp->GetConstant(index_));
  assert(class_ref_constant == nullptr);
  auto class_ref = class_ref_constant->ResolveClass();
  auto& stack = frame->GetOperandStack();
  std::vector<int32_t> counts;
  PopAndCheckCounts(stack, dimensions_, counts);
  auto arr = NewMultiDimensionalArray(counts, class_ref);
  stack.PushRef(arr);
}
void PUT_STATIC::Execute(runtime::Frame* frame) {
  auto cp = frame->GetMethod()->GetClass()->GetConstantPool();
  auto field_ref_constant = dynamic_cast<runtime::FieldRefConstant*>(cp->GetConstant(index_));
  assert(field_ref_constant == nullptr);
  auto field = field_ref_constant->ResolveField();
  auto field_class_ref = field->GetClass();

  //TODO check class init
  if (!field_class_ref->IsClinitStarted()) {
    frame->RevertNextPc();
    runtime::Class::InitClass(field_class_ref);
    return;
  }
  if (!field->IsStatic()) {
    throw std::runtime_error("java.lang.IncompatibleClassChangeError");
  }
  if (field->IsFinal()) {
    if (field_class_ref != frame->GetMethod()->GetClass() || frame->GetMethod()->GetName() != "<clinit>") {
      throw std::runtime_error("java.lang.IllegalAccessError");
    }
  }
  auto descriptor = field->GetDescriptor();
  auto slot_id = field->GetSlotId();
  auto slots = field_class_ref->GetStaticVars();
  auto& stack = frame->GetOperandStack();
  if (descriptor == "Z" || descriptor == "B" || descriptor == "C" || descriptor == "S" || descriptor == "I") {
    //auto val = PopOperandStack<int32_t>(stack);
    auto val = stack.PopInt();
    slots->SetInt(slot_id, val);
  } else if (descriptor == "F") {
    //auto val = PopOperandStack<float>(stack);
    auto val = stack.PopFloat();
    slots->SetFloat(slot_id, val);
  } else if (descriptor == "J") {
    //auto val = PopOperandStack<int64_t>(stack);
    auto val = stack.PopLong();
    slots->SetLong(slot_id, val);
  } else if (descriptor == "D") {
    //auto val = PopOperandStack<double>(stack);
    auto val = stack.PopDouble();
    slots->SetDouble(slot_id, val);
  } else if (descriptor[0] == 'L' || descriptor[0] == '[') {
    //auto val = PopOperandStack<void*>(stack);
    auto val = stack.PopRef();
    slots->SetRef(slot_id, val);
  }
}
void GET_STATIC::Execute(runtime::Frame* frame) {
  auto cp = frame->GetMethod()->GetClass()->GetConstantPool();
  auto field_ref_constant = dynamic_cast<runtime::FieldRefConstant*>(cp->GetConstant(index_));
  assert(field_ref_constant == nullptr);
  auto field = field_ref_constant->ResolveField();
  auto field_class_ref = field->GetClass();
  //TODO check class init
  if (!field_class_ref->IsClinitStarted()) {
    frame->RevertNextPc();
    runtime::Class::InitClass(field_class_ref);
    return;
  }
  if (!field->IsStatic()) {
    throw std::runtime_error("java.lang.IncompatibleClassChangeError");
  }
  auto descriptor = field->GetDescriptor();
  auto slot_id = field->GetSlotId();
  auto slots = field_class_ref->GetStaticVars();
  auto& stack = frame->GetOperandStack();
  if (descriptor == "Z" || descriptor == "B" || descriptor == "C" || descriptor == "S" || descriptor == "I") {
    auto val = slots->GetInt(slot_id);
    //PushOperandStack<int32_t>(stack, val);
    stack.PushInt(val);
  } else if (descriptor == "F") {
    auto val = slots->GetFloat(slot_id);
    //PushOperandStack<float>(stack, val);
    stack.PushFloat(val);
  } else if (descriptor == "J") {
    auto val = slots->GetLong(slot_id);
    //PushOperandStack<int64_t>(stack, val);
    stack.PushLong(val);
  } else if (descriptor == "D") {
    auto val = slots->GetDouble(slot_id);
    //PushOperandStack<double>(stack, val);
    stack.PushDouble(val);
  } else if (descriptor[0] == 'L' || descriptor[0] == '[') {
    auto val = slots->GetRef(slot_id);
    //PushOperandStack<void*>(stack, val);
    stack.PushRef(val);
  }
}
void GET_FIELD::Execute(runtime::Frame* frame) {
  auto cp = frame->GetMethod()->GetClass()->GetConstantPool();
  auto field_ref_constant = dynamic_cast<runtime::FieldRefConstant*>(cp->GetConstant(index_));
  assert(field_ref_constant == nullptr);
  auto field = field_ref_constant->ResolveField();
  if (field->IsStatic()) {
    throw std::runtime_error("java.lang.IncompatibleClassChangeError");
  }
  auto& stack = frame->GetOperandStack();
  auto ref = reinterpret_cast<runtime::DataObject*>(stack.PopRef());
  if (ref == nullptr) {
    throw std::runtime_error("java.lang.NullPointerException");
  }
  auto descriptor = field->GetDescriptor();
  auto slot_id = field->GetSlotId();
  if (descriptor == "Z" || descriptor == "B" || descriptor == "C" || descriptor == "S" || descriptor == "I") {
    auto val = ref->GetIntField(slot_id);
    //PushOperandStack<int32_t>(stack, val);
    stack.PushInt(val);
  } else if (descriptor == "F") {
    auto val = ref->GetFloatField(slot_id);
    //PushOperandStack<float>(stack, val);
    stack.PushFloat(val);
  } else if (descriptor == "J") {
    auto val = ref->GetLongField(slot_id);
    //PushOperandStack<int64_t>(stack, val);
    stack.PushLong(val);
  } else if (descriptor == "D") {
    auto val = ref->GetDoubleField(slot_id);
    //PushOperandStack<double>(stack, val);
    stack.PushDouble(val);
  } else if (descriptor[0] == 'L' || descriptor[0] == '[') {
    auto val = ref->GetRefField(slot_id);
    //PushOperandStack<void*>(stack, val);
    stack.PushRef(val);
  } else {
    //throw std::runtime_error("todo");
    LOG(ERROR) << "getfield failed : " << descriptor ;
  }
}
void PUT_FIELD::Execute(runtime::Frame* frame) {
  auto cp = frame->GetMethod()->GetClass()->GetConstantPool();
  auto field_ref_constant = dynamic_cast<runtime::FieldRefConstant*>(cp->GetConstant(index_));
  auto field = field_ref_constant->ResolveField();
  if (field->IsStatic()) {
    throw std::runtime_error("java.lang.IncompatibleClassChangeError");
  }
  if (field->IsFinal()) {
    if (frame->GetMethod()->GetClass() != field->GetClass() || frame->GetMethod()->GetName() != "<init>") {
      throw std::runtime_error("java.lang.IllegalAccessError");
    }
  }
  auto& stack = frame->GetOperandStack();
  
  auto descriptor = field->GetDescriptor();
  auto slot_id = field->GetSlotId();
  if (descriptor == "Z" || descriptor == "B" || descriptor == "C" || descriptor == "S" || descriptor == "I") {
    //auto val = PopOperandStack<int32_t>(stack);
    auto val = stack.PopInt();
    auto obj_ref = reinterpret_cast<runtime::DataObject*>(stack.PopRef());
    if (obj_ref == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }

    obj_ref->SetIntField(slot_id, val);
  } else if (descriptor == "F") {
    //auto val = PopOperandStack<float>(stack);
    auto val = stack.PopFloat();
    auto obj_ref = reinterpret_cast<runtime::DataObject*>(stack.PopRef());
    if (obj_ref == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    obj_ref->SetFloatField(slot_id, val);
  } else if (descriptor == "J") {
    //auto val = PopOperandStack<int64_t>(stack);
    auto val = stack.PopLong();
    auto obj_ref = reinterpret_cast<runtime::DataObject*>(stack.PopRef());
    if (obj_ref == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    obj_ref->SetLongField(slot_id, val);
  } else if (descriptor == "D") {
    //auto val = PopOperandStack<double>(stack);
    auto val = stack.PopDouble();
    auto obj_ref = reinterpret_cast<runtime::DataObject*>(stack.PopRef());
    if (obj_ref == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    obj_ref->SetDoubleField(slot_id, val);
  } else if (descriptor[0] == 'L' || descriptor[0] == '[') {
    //auto val = PopOperandStack<void*>(stack);
    auto val = stack.PopRef();
    auto obj_ref = reinterpret_cast<runtime::DataObject*>(stack.PopRef());
    if (obj_ref == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    obj_ref->SetRefField(slot_id, val);
  }
}
void INSTANCE_OF::Execute(runtime::Frame* frame) {
  auto cp = frame->GetMethod()->GetClass()->GetConstantPool();
  auto class_ref_constant = dynamic_cast<runtime::ClassRefConstant*>(cp->GetConstant(index_));
  auto class_ref = class_ref_constant->ResolveClass();
  auto& stack = frame->GetOperandStack();
  auto ref = stack.PopRef();
  if (ref == nullptr) {
    stack.PushInt(0);
  } else {
    if (ref->IsInstanceOf(class_ref)) {
      stack.PushInt(1);
    } else {
      stack.PushInt(0);
    }
  }
}
void CHECK_CAST::Execute(runtime::Frame* frame) {
  auto cp = frame->GetMethod()->GetClass()->GetConstantPool();
  auto class_ref_constant = dynamic_cast<runtime::ClassRefConstant*>(cp->GetConstant(index_));
  auto class_ref = class_ref_constant->ResolveClass();
  auto& stack = frame->GetOperandStack();
  auto ref = stack.PopRef();
  if (ref != nullptr) {
    if (!ref->IsInstanceOf(class_ref)) {
      throw std::runtime_error("java.lang.ClassCastException");
    }
  }
  stack.PushRef(ref);
}
void LoadConstant(runtime::Frame* frame, uint32_t index) {
  auto& stack = frame->GetOperandStack();
  auto cp = frame->GetMethod()->GetClass()->GetConstantPool();
  auto c = cp->GetConstant(index);
  switch (c->GetConstantType()) {
    case runtime::kConstantInteger: {
      auto integer_constant = dynamic_cast<runtime::IntegerConstant*>(c);
      stack.PushInt(integer_constant->GetValue());
      break;
    }
    case runtime::kConstantFloat: {
      auto float_constant = dynamic_cast<runtime::FloatConstant*>(c);
      stack.PushFloat(float_constant->GetValue());
      break;
    }
    case runtime::kConstantUtf8: {
      auto string_constant = dynamic_cast<runtime::StringConstant*>(c);
      runtime::Object* j_string_obj = runtime::Class::NewJString(string_constant->GetValue());
      stack.PushRef(j_string_obj);
      
      break;
    }
    case runtime::kConstantClass: {
      auto class_ref_constant = dynamic_cast<runtime::ClassRefConstant*>(c);
      auto class_ref = class_ref_constant->ResolveClass();
      auto class_obj = static_cast<runtime::Object*>(class_ref);
      stack.PushRef(class_obj);
      break;
    }
    default:
      throw std::runtime_error("todo: ldc!");
  }
}
void LDC::Execute(runtime::Frame* frame) {
  LoadConstant(frame, index_);
}
void LDC_W::Execute(runtime::Frame* frame) {
  LoadConstant(frame, index_);
}
void LDC2_W::Execute(runtime::Frame* frame) {
  auto& stack = frame->GetOperandStack();
  auto cp = frame->GetMethod()->GetClass()->GetConstantPool();
  auto c = cp->GetConstant(index_);
  switch (c->GetConstantType()) {
    case runtime::kConstantLong: {
      auto long_constant = dynamic_cast<runtime::LongConstant*>(c);
      stack.PushLong(long_constant->GetValue());
      break;
    }
    case runtime::kConstantDouble: {
      auto double_constant = dynamic_cast<runtime::DoubleConstant*>(c);
      stack.PushDouble(double_constant->GetValue());
      break;
    }
    default:
      throw std::runtime_error("todo: ldc2_w!");
  }
}

void PopAndCheckCounts(runtime::OperandStack& stack, uint32_t dimensions, std::vector<int32_t>& counts) {
  counts.resize(dimensions);
  for (int i = dimensions - 1; i >= 0; i--) {
    counts[i] = stack.PopInt();
    if (counts[i] < 0) {
      throw std::runtime_error("java.lang.NegativeArraySizeException");
    }
  }
}

runtime::Object* NewMultiDimensionalArray(std::vector<int32_t>& counts, runtime::Class* arr_class) {
  auto counts_len = counts.size();
  auto count = counts[0];
  auto arr = arr_class->NewArray(count);
  if (counts_len > 1) {
    //TODO: multi Array create
    //runtime::Object
    //auto refs = arr->getArray<runtime::Object*>();
    for (int i = 0; i < count; i++) {
      runtime::Class* component_class = arr_class->GetComponentClass();
      std::vector<int32_t> new_counts(counts.begin() + 1, counts.end());
      //refs[i] = NewMultiDimensionalArray(new_counts, component_class);
    }
  }
  return arr;
}



}//namespace instructions