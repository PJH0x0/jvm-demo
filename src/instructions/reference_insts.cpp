#include "reference_insts.h"
#include <_types/_uint32_t.h>
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
static bool findAndGotoExceptionHandler(std::shared_ptr<runtime::Thread> thread, 
                                        runtime::Object* ex) {
  while (true) {
    auto frame = thread->currentFrame();
    auto pc = frame->nextPC() - 1;
    auto handlerPc = frame->getMethod()->findExceptionHandler(ex->getClass(), pc);
    if (handlerPc > 0) {
      auto& stack = frame->getOperandStack();
      stack.clear();
      stack.pushRef(ex);
      frame->setNextPC(handlerPc);
      return true;
    }
    thread->popFrame();
    if (thread->isStackEmpty()) {
      break;
    }
  }
  return false;
}

static void  handleUncaughtException(std::shared_ptr<runtime::Thread> thread, 
                                     runtime::Object* ex) {
  thread->clearStack();
  auto jMsg = ex->getRefVar("detailMessage", "Ljava/lang/String;");
  auto cMsg = runtime::StringPool::javaStringToString(jMsg);
  std::cout << ex->getClass()->getJavaName() << ": " << cMsg << std::endl;
  //auto stes = ex->getRefVar("stackTrace", "[Ljava/lang/StackTraceElement;");
  //auto stesArr = static_cast<std::vector<std::shared_ptr<native::StackTraceElement>>*>(ex->getExtra());
  auto stesArr = new std::vector<std::shared_ptr<native::StackTraceElement>>();
  if (stesArr == nullptr) {
    //stesArr = native::createStackTraceElements(ex, thread);
    LOG(FATAL) << "stesArr is nullptr";
  }
  for (auto ste : *stesArr) {
    auto fileName = ste->getFileName();
    auto className = ste->getClassName();
    auto methodName = ste->getMethodName();
    auto lineNumber = ste->getLineNumber();
    std::cout << "\tat " << className << "." 
              << methodName << "("
              << fileName << ":"
              << lineNumber << ")" << std::endl;
  }

}

void ATHROW::execute(std::shared_ptr<runtime::Frame> frame) {
  auto ex = frame->getOperandStack().popRef();
  if (ex == nullptr) {
    throw std::runtime_error("java.lang.NullPointerException");
  }
  auto thread = frame->getThread();
  if (!findAndGotoExceptionHandler(thread, ex)) {
    handleUncaughtException(thread, ex);
  }
}
void NEW::execute(std::shared_ptr<runtime::Frame> frame) {
  auto method = frame->getMethod();
  auto cp = method->getClass()->getConstantPool();
  auto classRef = std::static_pointer_cast<runtime::ClassRefConstant>(cp->getConstant(index));
  auto classPtr = classRef->resolveClass();
  //TODO check class init
  if (!classPtr->isClinitStarted()) {
    frame->revertNextPC();
    runtime::Class::initClass(frame->getThread(), classPtr);
    return;
  }
  if (classPtr->isInterface() || classPtr->isAbstract()) {
    throw std::runtime_error("java.lang.InstantiationError");
  }
  
  runtime::Object* ref = JVM::current()->getHeap()->allocObject(frame->getThread().get(), classPtr, classPtr->objectSize());
  frame->getOperandStack().pushRef(ref);
}
void NEW_ARRAY::fetchOperands(std::shared_ptr<BytecodeReader> reader) {
  mAtype = reader->readUInt8();
}
void NEW_ARRAY::execute(std::shared_ptr<runtime::Frame> frame) {
  auto& stack = frame->getOperandStack();
  auto count = stack.popInt();
  if (count < 0) {
    throw std::runtime_error("java.lang.NegativeArraySizeException");
  }
  auto arrClass = runtime::Class::getPrimitiveArrayClass(mAtype);
  auto arr = arrClass->newArray(count);
  stack.pushRef(arr);
}
void ANEW_ARRAY::execute(std::shared_ptr<runtime::Frame> frame) {
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto classRef = std::static_pointer_cast<runtime::ClassRefConstant>(cp->getConstant(index));
  auto classPtr = classRef->resolveClass();
  auto& stack = frame->getOperandStack();
  auto count = stack.popInt();
  if (count < 0) {
    throw std::runtime_error("java.lang.NegativeArraySizeException");
  }
  auto arrClass = classPtr->getArrayClass();
  auto arr = arrClass->newArray(count);
  stack.pushRef(arr);
}
void ARRAY_LENGTH::execute(std::shared_ptr<runtime::Frame> frame) {
  auto& stack = frame->getOperandStack();
  runtime::Object* arrRef = stack.popRef();
  if (arrRef == nullptr) {
    throw std::runtime_error("java.lang.NullPointerException");
  }
  // auto arrLen = arrRef->arrayLength();
  // stack.pushInt(arrLen);
}

void MULTI_ANEW_ARRAY::fetchOperands(std::shared_ptr<BytecodeReader> reader) {
  mIndex = reader->readUInt16();
  mDimensions = reader->readUInt8();
}
void MULTI_ANEW_ARRAY::execute(std::shared_ptr<runtime::Frame> frame) {
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto classRef = std::static_pointer_cast<runtime::ClassRefConstant>(cp->getConstant(mIndex));
  auto classPtr = classRef->resolveClass();
  auto& stack = frame->getOperandStack();
  std::vector<int32_t> counts;
  popAndCheckCounts(stack, mDimensions, counts);
  auto arr = newMultiDimensionalArray(counts, classPtr);
  stack.pushRef(arr);
}
void PUT_STATIC::execute(std::shared_ptr<runtime::Frame> frame) {
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto fieldRef = std::static_pointer_cast<runtime::FieldRefConstant>(cp->getConstant(index));
  auto field = fieldRef->resolveField();
  auto classPtr = field->getClass();

  //TODO check class init
  if (!classPtr->isClinitStarted()) {
    frame->revertNextPC();
    runtime::Class::initClass(frame->getThread(), classPtr);
    return;
  }
  if (!field->isStatic()) {
    throw std::runtime_error("java.lang.IncompatibleClassChangeError");
  }
  if (field->isFinal()) {
    if (classPtr != frame->getMethod()->getClass() || frame->getMethod()->getName() != "<clinit>") {
      throw std::runtime_error("java.lang.IllegalAccessError");
    }
  }
  auto descriptor = field->getDescriptor();
  auto slotId = field->getSlotId();
  auto slots = classPtr->getStaticVars();
  auto& stack = frame->getOperandStack();
  if (descriptor == "Z" || descriptor == "B" || descriptor == "C" || descriptor == "S" || descriptor == "I") {
    //auto val = popOperandStack<int32_t>(stack);
    auto val = stack.popInt();
    slots->setInt(slotId, val);
  } else if (descriptor == "F") {
    //auto val = popOperandStack<float>(stack);
    auto val = stack.popFloat();
    slots->setFloat(slotId, val);
  } else if (descriptor == "J") {
    //auto val = popOperandStack<int64_t>(stack);
    auto val = stack.popLong();
    slots->setLong(slotId, val);
  } else if (descriptor == "D") {
    //auto val = popOperandStack<double>(stack);
    auto val = stack.popDouble();
    slots->setDouble(slotId, val);
  } else if (descriptor[0] == 'L' || descriptor[0] == '[') {
    //auto val = popOperandStack<void*>(stack);
    auto val = stack.popRef();
    slots->setRef(slotId, val);
  }
}
void GET_STATIC::execute(std::shared_ptr<runtime::Frame> frame) {
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto fieldRef = std::static_pointer_cast<runtime::FieldRefConstant>(cp->getConstant(index));
  auto field = fieldRef->resolveField();
  auto classPtr = field->getClass();
  //TODO check class init
  if (!classPtr->isClinitStarted()) {
    frame->revertNextPC();
    runtime::Class::initClass(frame->getThread(), classPtr);
    return;
  }
  if (!field->isStatic()) {
    throw std::runtime_error("java.lang.IncompatibleClassChangeError");
  }
  auto descriptor = field->getDescriptor();
  auto slotId = field->getSlotId();
  auto slots = classPtr->getStaticVars();
  auto& stack = frame->getOperandStack();
  if (descriptor == "Z" || descriptor == "B" || descriptor == "C" || descriptor == "S" || descriptor == "I") {
    auto val = slots->getInt(slotId);
    //pushOperandStack<int32_t>(stack, val);
    stack.pushInt(val);
  } else if (descriptor == "F") {
    auto val = slots->getFloat(slotId);
    //pushOperandStack<float>(stack, val);
    stack.pushFloat(val);
  } else if (descriptor == "J") {
    auto val = slots->getLong(slotId);
    //pushOperandStack<int64_t>(stack, val);
    stack.pushLong(val);
  } else if (descriptor == "D") {
    auto val = slots->getDouble(slotId);
    //pushOperandStack<double>(stack, val);
    stack.pushDouble(val);
  } else if (descriptor[0] == 'L' || descriptor[0] == '[') {
    auto val = slots->getRef(slotId);
    //pushOperandStack<void*>(stack, val);
    stack.pushRef(val);
  }
}
void GET_FIELD::execute(std::shared_ptr<runtime::Frame> frame) {
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto fieldRef = std::static_pointer_cast<runtime::FieldRefConstant>(cp->getConstant(index));
  auto field = fieldRef->resolveField();
  if (field->isStatic()) {
    throw std::runtime_error("java.lang.IncompatibleClassChangeError");
  }
  auto& stack = frame->getOperandStack();
  auto ref = reinterpret_cast<runtime::DataObject*>(stack.popRef());
  if (ref == nullptr) {
    throw std::runtime_error("java.lang.NullPointerException");
  }
  auto descriptor = field->getDescriptor();
  auto slotId = field->getSlotId();
  if (descriptor == "Z" || descriptor == "B" || descriptor == "C" || descriptor == "S" || descriptor == "I") {
    auto val = ref->getIntField(slotId);
    //pushOperandStack<int32_t>(stack, val);
    stack.pushInt(val);
  } else if (descriptor == "F") {
    auto val = ref->getFloatField(slotId);
    //pushOperandStack<float>(stack, val);
    stack.pushFloat(val);
  } else if (descriptor == "J") {
    auto val = ref->getLongField(slotId);
    //pushOperandStack<int64_t>(stack, val);
    stack.pushLong(val);
  } else if (descriptor == "D") {
    auto val = ref->getDoubleField(slotId);
    //pushOperandStack<double>(stack, val);
    stack.pushDouble(val);
  } else if (descriptor[0] == 'L' || descriptor[0] == '[') {
    auto val = ref->getRefField(slotId);
    //pushOperandStack<void*>(stack, val);
    stack.pushRef(val);
  } else {
    //throw std::runtime_error("todo");
    LOG(ERROR) << "getfield failed : " << descriptor ;
  }
}
void PUT_FIELD::execute(std::shared_ptr<runtime::Frame> frame) {
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto fieldRef = std::static_pointer_cast<runtime::FieldRefConstant>(cp->getConstant(index));
  auto field = fieldRef->resolveField();
  if (field->isStatic()) {
    throw std::runtime_error("java.lang.IncompatibleClassChangeError");
  }
  if (field->isFinal()) {
    if (frame->getMethod()->getClass() != field->getClass() || frame->getMethod()->getName() != "<init>") {
      throw std::runtime_error("java.lang.IllegalAccessError");
    }
  }
  auto& stack = frame->getOperandStack();
  
  auto descriptor = field->getDescriptor();
  auto slotId = field->getSlotId();
  if (descriptor == "Z" || descriptor == "B" || descriptor == "C" || descriptor == "S" || descriptor == "I") {
    //auto val = popOperandStack<int32_t>(stack);
    auto val = stack.popInt();
    auto objRef = reinterpret_cast<runtime::DataObject*>(stack.popRef());
    if (objRef == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    
    objRef->setIntField(slotId, val);
  } else if (descriptor == "F") {
    //auto val = popOperandStack<float>(stack);
    auto val = stack.popFloat();
    auto objRef = reinterpret_cast<runtime::DataObject*>(stack.popRef());
    if (objRef == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    objRef->setFloatField(slotId, val);
  } else if (descriptor == "J") {
    //auto val = popOperandStack<int64_t>(stack);
    auto val = stack.popLong();
    auto objRef = reinterpret_cast<runtime::DataObject*>(stack.popRef());
    if (objRef == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    objRef->setLongField(slotId, val);
  } else if (descriptor == "D") {
    //auto val = popOperandStack<double>(stack);
    auto val = stack.popDouble();
    auto objRef = reinterpret_cast<runtime::DataObject*>(stack.popRef());
    if (objRef == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    objRef->setDoubleField(slotId, val);
  } else if (descriptor[0] == 'L' || descriptor[0] == '[') {
    //auto val = popOperandStack<void*>(stack);
    auto val = stack.popRef();
    auto objRef = reinterpret_cast<runtime::DataObject*>(stack.popRef());
    if (objRef == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    objRef->setRefField(slotId, val);
  }
}
void INSTANCE_OF::execute(std::shared_ptr<runtime::Frame> frame) {
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto classRef = std::static_pointer_cast<runtime::ClassRefConstant>(cp->getConstant(index));
  auto classPtr = classRef->resolveClass();
  auto& stack = frame->getOperandStack();
  auto ref = stack.popRef();
  if (ref == nullptr) {
    stack.pushInt(0);
  } else {
    if (ref->isInstanceOf(classPtr)) {
      stack.pushInt(1);
    } else {
      stack.pushInt(0);
    }
  }
}
void CHECK_CAST::execute(std::shared_ptr<runtime::Frame> frame) {
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto classRef = std::static_pointer_cast<runtime::ClassRefConstant>(cp->getConstant(index));
  auto classPtr = classRef->resolveClass();
  auto& stack = frame->getOperandStack();
  auto ref = stack.popRef();
  if (ref != nullptr) {
    if (!ref->isInstanceOf(classPtr)) {
      throw std::runtime_error("java.lang.ClassCastException");
    }
  }
  stack.pushRef(ref);
}
void _ldc(std::shared_ptr<runtime::Frame> frame, uint32_t index) {
  auto& stack = frame->getOperandStack();
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto c = cp->getConstant(index);
  switch (c->tag()) {
    case runtime::CONSTANT_Integer: {
      auto intC = std::static_pointer_cast<runtime::IntegerConstant>(c);
      stack.pushInt(intC->value());
      break;
    }
    case runtime::CONSTANT_Float: {
      auto floatC = std::static_pointer_cast<runtime::FloatConstant>(c);
      stack.pushFloat(floatC->value());
      break;
    }
    case runtime::CONSTANT_String: {
      auto stringC = std::static_pointer_cast<runtime::StringConstant>(c);
      runtime::Object* jStringObj = runtime::Class::newJString(stringC->value());
      stack.pushRef(jStringObj);
      
      break;
    }
    case runtime::CONSTANT_Class: {
      auto classC = std::static_pointer_cast<runtime::ClassRefConstant>(c);
      auto classPtr = classC->resolveClass();
      auto classObj = static_cast<runtime::Object*>(classPtr);
      stack.pushRef(classObj);
      break;
    }
    default:
      throw std::runtime_error("todo: ldc!");
  }
}
void LDC::execute(std::shared_ptr<runtime::Frame> frame) {
  _ldc(frame, index);
}
void LDC_W::execute(std::shared_ptr<runtime::Frame> frame) {
  _ldc(frame, index);
}
void LDC2_W::execute(std::shared_ptr<runtime::Frame> frame) {
  auto& stack = frame->getOperandStack();
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto c = cp->getConstant(index);
  switch (c->tag()) {
    case runtime::CONSTANT_Long: {
      auto longC = std::static_pointer_cast<runtime::LongConstant>(c);
      stack.pushLong(longC->value());
      break;
    }
    case runtime::CONSTANT_Double: {
      auto doubleC = std::static_pointer_cast<runtime::DoubleConstant>(c);
      stack.pushDouble(doubleC->value());
      break;
    }
    default:
      throw std::runtime_error("todo: ldc2_w!");
  }
}

void popAndCheckCounts(runtime::OperandStack& stack, uint32_t dimensions, std::vector<int32_t>& counts) {
  counts.resize(dimensions);
  for (int i = dimensions - 1; i >= 0; i--) {
    counts[i] = stack.popInt();
    if (counts[i] < 0) {
      throw std::runtime_error("java.lang.NegativeArraySizeException");
    }
  }
}

runtime::Object* newMultiDimensionalArray(std::vector<int32_t>& counts, runtime::Class* arrClass) {
  auto countsLen = counts.size();
  auto count = counts[0];
  auto arr = arrClass->newArray(count);
  if (countsLen > 1) {
    //TODO: multi Array create
    //runtime::Object
    //auto refs = arr->getArray<runtime::Object*>();
    for (int i = 0; i < count; i++) {
      runtime::Class* componentClass = arrClass->getComponentClass();
      std::vector<int32_t> newCounts(counts.begin() + 1, counts.end());
      //refs[i] = newMultiDimensionalArray(newCounts, componentClass);
    }
  }
  return arr;
}



}//namespace instructions