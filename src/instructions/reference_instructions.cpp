#include "reference_instructions.h"
#include <rtda/heap/object.h>
#include <memory>
#include <rtda/heap/method.h>
#include <rtda/heap/field.h>
#include <rtda/heap/constant_pool.h>
#include <rtda/operand_stack.h>
#include <rtda/heap/class.h>

namespace instructions {
void NEW::execute(std::shared_ptr<rtda::Frame> frame) {
  auto method = frame->getMethod();
  auto cp = method->getClass()->getConstantPool();
  auto classRef = std::static_pointer_cast<rtda::ClassRefConstant>(cp->getConstant(index));
  auto classPtr = classRef->resolveClass();
  //TODO check class init
  // if (!classPtr->isInitStarted()) {
  //   frame->revertNextPC();
  //   rtda::initClass(frame->getThread(), classPtr);
  //   return;
  // }
  if (classPtr->isInterface() || classPtr->isAbstract()) {
    throw std::runtime_error("java.lang.InstantiationError");
  }
  rtda::Object* ref = new rtda::Object(classPtr);
  frame->getOperandStack().pushRef(ref);
}
void NEW_ARRAY::fetchOperands(std::shared_ptr<BytecodeReader> reader) {
  mAtype = reader->readUInt8();
}
void NEW_ARRAY::execute(std::shared_ptr<rtda::Frame> frame) {
  auto& stack = frame->getOperandStack();
  auto count = stack.popInt();
  if (count < 0) {
    throw std::runtime_error("java.lang.NegativeArraySizeException");
  }
  auto arrClass = rtda::Class::getPrimitiveArrayClass(mAtype);
  auto arr = arrClass->newArray(count);
  stack.pushRef(arr);
}
void ANEW_ARRAY::execute(std::shared_ptr<rtda::Frame> frame) {
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto classRef = std::static_pointer_cast<rtda::ClassRefConstant>(cp->getConstant(index));
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
void ARRAY_LENGTH::execute(std::shared_ptr<rtda::Frame> frame) {
  auto& stack = frame->getOperandStack();
  rtda::Object* arrRef = stack.popRef();
  if (arrRef == nullptr) {
    throw std::runtime_error("java.lang.NullPointerException");
  }
  auto arrLen = arrRef->arrayLength();
  stack.pushInt(arrLen);
}

void MULTI_ANEW_ARRAY::fetchOperands(std::shared_ptr<BytecodeReader> reader) {
  mIndex = reader->readUInt16();
  mDimensions = reader->readUInt8();
}
void MULTI_ANEW_ARRAY::execute(std::shared_ptr<rtda::Frame> frame) {
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto classRef = std::static_pointer_cast<rtda::ClassRefConstant>(cp->getConstant(mIndex));
  auto classPtr = classRef->resolveClass();
  auto& stack = frame->getOperandStack();
  std::vector<int32_t> counts;
  popAndCheckCounts(stack, mDimensions, counts);
  auto arr = newMultiDimensionalArray(counts, classPtr);
  stack.pushRef(arr);
}
void PUT_STATIC::execute(std::shared_ptr<rtda::Frame> frame) {
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto fieldRef = std::static_pointer_cast<rtda::FieldRefConstant>(cp->getConstant(index));
  auto field = fieldRef->resolveField();
  auto classPtr = field->getClass();

  //TODO check class init
  // if (!classPtr->isInitStarted()) {
  //   frame->revertNextPC();
  //   rtda::initClass(frame->getThread(), classPtr);
  //   return;
  // }
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
  } else if (descriptor == "L" || descriptor == "[") {
    //auto val = popOperandStack<void*>(stack);
    auto val = stack.popRef();
    slots->setRef(slotId, val);
  }
}
void GET_STATIC::execute(std::shared_ptr<rtda::Frame> frame) {
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto fieldRef = std::static_pointer_cast<rtda::FieldRefConstant>(cp->getConstant(index));
  auto field = fieldRef->resolveField();
  auto classPtr = field->getClass();
  //TODO check class init
  // if (!classPtr->isInitStarted()) {
  //   frame->revertNextPC();
  //   rtda::initClass(frame->getThread(), classPtr);
  //   return;
  // }
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
  } else if (descriptor == "L" || descriptor == "[") {
    auto val = slots->getRef(slotId);
    //pushOperandStack<void*>(stack, val);
    stack.pushRef(val);
  }
}
void GET_FIELD::execute(std::shared_ptr<rtda::Frame> frame) {
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto fieldRef = std::static_pointer_cast<rtda::FieldRefConstant>(cp->getConstant(index));
  auto field = fieldRef->resolveField();
  if (field->isStatic()) {
    throw std::runtime_error("java.lang.IncompatibleClassChangeError");
  }
  auto& stack = frame->getOperandStack();
  auto ref = stack.popRef();
  if (ref == nullptr) {
    throw std::runtime_error("java.lang.NullPointerException");
  }
  std::shared_ptr<rtda::Object> objRef = std::make_shared<rtda::Object>(*static_cast<rtda::Object*>(ref));

  auto descriptor = field->getDescriptor();
  auto slotId = field->getSlotId();
  if (descriptor == "Z" || descriptor == "B" || descriptor == "C" || descriptor == "S" || descriptor == "I") {
    auto val = objRef->getFields()->getInt(slotId);
    //pushOperandStack<int32_t>(stack, val);
    stack.pushInt(val);
  } else if (descriptor == "F") {
    auto val = objRef->getFields()->getFloat(slotId);
    //pushOperandStack<float>(stack, val);
    stack.pushFloat(val);
  } else if (descriptor == "J") {
    auto val = objRef->getFields()->getLong(slotId);
    //pushOperandStack<int64_t>(stack, val);
    stack.pushLong(val);
  } else if (descriptor == "D") {
    auto val = objRef->getFields()->getDouble(slotId);
    //pushOperandStack<double>(stack, val);
    stack.pushDouble(val);
  } else if (descriptor == "L" || descriptor == "[") {
    auto val = objRef->getFields()->getRef(slotId);
    //pushOperandStack<void*>(stack, val);
    stack.pushRef(val);
  }
}
void PUT_FIELD::execute(std::shared_ptr<rtda::Frame> frame) {
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto fieldRef = std::static_pointer_cast<rtda::FieldRefConstant>(cp->getConstant(index));
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
    auto objRef = stack.popRef();
    if (objRef == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    //ref->getFields()->setInt(slotId, val);
    objRef->getFields()->setInt(slotId, val);
  } else if (descriptor == "F") {
    //auto val = popOperandStack<float>(stack);
    auto val = stack.popFloat();
    auto objRef = stack.popRef();
    if (objRef == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    objRef->getFields()->setFloat(slotId, val);
  } else if (descriptor == "J") {
    //auto val = popOperandStack<int64_t>(stack);
    auto val = stack.popLong();
    auto objRef = stack.popRef();
    if (objRef == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    objRef->getFields()->setLong(slotId, val);
  } else if (descriptor == "D") {
    //auto val = popOperandStack<double>(stack);
    auto val = stack.popDouble();
    auto objRef = stack.popRef();
    if (objRef == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    objRef->getFields()->setDouble(slotId, val);
  } else if (descriptor == "L" || descriptor == "[") {
    //auto val = popOperandStack<void*>(stack);
    auto val = stack.popRef();
    auto objRef = stack.popRef();
    if (objRef == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    objRef->getFields()->setRef(slotId, val);
  }
}
void INSTANCE_OF::execute(std::shared_ptr<rtda::Frame> frame) {
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto classRef = std::static_pointer_cast<rtda::ClassRefConstant>(cp->getConstant(index));
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
void CHECK_CAST::execute(std::shared_ptr<rtda::Frame> frame) {
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto classRef = std::static_pointer_cast<rtda::ClassRefConstant>(cp->getConstant(index));
  auto classPtr = classRef->resolveClass();
  auto& stack = frame->getOperandStack();
  auto ref = stack.popRef();
  if (ref != nullptr) {
    auto objRef = std::make_shared<rtda::Object>(*static_cast<rtda::Object*>(ref));
    if (!objRef->isInstanceOf(classPtr)) {
      throw std::runtime_error("java.lang.ClassCastException");
    }
  }
  stack.pushRef(ref);
}
void _ldc(std::shared_ptr<rtda::Frame> frame, uint32_t index) {
  auto& stack = frame->getOperandStack();
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto c = cp->getConstant(index);
  switch (c->tag()) {
    case rtda::CONSTANT_Integer: {
      auto intC = std::static_pointer_cast<rtda::IntegerConstant>(c);
      stack.pushInt(intC->value());
      break;
    }
    case rtda::CONSTANT_Float: {
      auto floatC = std::static_pointer_cast<rtda::FloatConstant>(c);
      stack.pushFloat(floatC->value());
      break;
    }
    case rtda::CONSTANT_String: {
      // auto stringC = std::static_pointer_cast<rtda::StringConstant>(c);
      // auto jString = stringC->mString;
      // auto jStringObj = jString->mJClass->newObject();
      // jStringObj->mExtra = jString;
      // stack.pushRef(jStringObj);
      break;
    }
    case rtda::CONSTANT_Class: {
      // auto classC = std::static_pointer_cast<rtda::ClassConstant>(c);
      // auto classPtr = classC->mClassPtr;
      // auto classObj = classPtr->mJClass->newObject();
      // classObj->mExtra = classPtr;
      // stack.pushRef(classObj);
      break;
    }
    default:
      throw std::runtime_error("todo: ldc!");
  }
}
void LDC::execute(std::shared_ptr<rtda::Frame> frame) {
  _ldc(frame, index);
}
void LDC_W::execute(std::shared_ptr<rtda::Frame> frame) {
  _ldc(frame, index);
}
void LDC2_W::execute(std::shared_ptr<rtda::Frame> frame) {
  auto& stack = frame->getOperandStack();
  auto cp = frame->getMethod()->getClass()->getConstantPool();
  auto c = cp->getConstant(index);
  switch (c->tag()) {
    case rtda::CONSTANT_Long: {
      auto longC = std::static_pointer_cast<rtda::LongConstant>(c);
      stack.pushLong(longC->value());
      break;
    }
    case rtda::CONSTANT_Double: {
      auto doubleC = std::static_pointer_cast<rtda::DoubleConstant>(c);
      stack.pushDouble(doubleC->value());
      break;
    }
    default:
      throw std::runtime_error("todo: ldc2_w!");
  }
}

void popAndCheckCounts(rtda::OperandStack& stack, uint32_t dimensions, std::vector<int32_t>& counts) {
  counts.resize(dimensions);
  for (int i = dimensions - 1; i >= 0; i--) {
    counts[i] = stack.popInt();
    if (counts[i] < 0) {
      throw std::runtime_error("java.lang.NegativeArraySizeException");
    }
  }
}

rtda::Object* newMultiDimensionalArray(std::vector<int32_t>& counts, std::shared_ptr<rtda::Class> arrClass) {
  auto countsLen = counts.size();
  auto count = counts[0];
  auto arr = arrClass->newArray(count);
  if (countsLen > 1) {
    auto refs = arr->getArray<rtda::Object*>();
    for (int i = 0; i < count; i++) {
      std::shared_ptr<rtda::Class> componentClass = arrClass->getComponentClass();
      std::vector<int32_t> newCounts(counts.begin() + 1, counts.end());
      refs[i] = newMultiDimensionalArray(newCounts, componentClass);
    }
  }
  return arr;
}



}//namespace instructions