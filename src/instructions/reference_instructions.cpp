#include "reference_instructions.h"
#include "rtda/heap/object.h"
#include <memory>
#include <rtda/heap/class_member.h>
#include <rtda/heap/constant_pool.h>

namespace instructions {
void NEW::execute(std::shared_ptr<rtda::Frame> frame) {
  auto method = frame->getMethod();
  auto cp = method->mClassPtr->mConstantPool;
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
  frame->getOperandStack().pushRef((void*)ref);
}
void PUT_STATIC::execute(std::shared_ptr<rtda::Frame> frame) {
  auto cp = frame->getMethod()->mClassPtr->mConstantPool;
  auto fieldRef = std::static_pointer_cast<rtda::FieldRefConstant>(cp->getConstant(index));
  auto field = fieldRef->resolveField();
  auto classPtr = field->mClassPtr;

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
    if (classPtr != frame->getMethod()->mClassPtr || frame->getMethod()->mName != "<clinit>") {
      throw std::runtime_error("java.lang.IllegalAccessError");
    }
  }
  auto descriptor = field->mDescriptor;
  auto slotId = field->mSlotId;
  auto slots = classPtr->mStaticVars;
  auto stack = frame->getOperandStack();
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
  auto cp = frame->getMethod()->mClassPtr->mConstantPool;
  auto fieldRef = std::static_pointer_cast<rtda::FieldRefConstant>(cp->getConstant(index));
  auto field = fieldRef->resolveField();
  auto classPtr = field->mClassPtr;
  //TODO check class init
  // if (!classPtr->isInitStarted()) {
  //   frame->revertNextPC();
  //   rtda::initClass(frame->getThread(), classPtr);
  //   return;
  // }
  if (!field->isStatic()) {
    throw std::runtime_error("java.lang.IncompatibleClassChangeError");
  }
  auto descriptor = field->mDescriptor;
  auto slotId = field->mSlotId;
  auto slots = classPtr->mStaticVars;
  auto stack = frame->getOperandStack();
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
  auto cp = frame->getMethod()->mClassPtr->mConstantPool;
  auto fieldRef = std::static_pointer_cast<rtda::FieldRefConstant>(cp->getConstant(index));
  auto field = fieldRef->resolveField();
  if (field->isStatic()) {
    throw std::runtime_error("java.lang.IncompatibleClassChangeError");
  }
  auto& stack = frame->getOperandStack();
  LOG(INFO) << "stack = " << &stack;
  auto ref = stack.popRef();
  if (ref == nullptr) {
    throw std::runtime_error("java.lang.NullPointerException");
  }
  std::shared_ptr<rtda::Object> objRef = std::make_shared<rtda::Object>(*static_cast<rtda::Object*>(ref));

  auto descriptor = field->mDescriptor;
  auto slotId = field->mSlotId;
  if (descriptor == "Z" || descriptor == "B" || descriptor == "C" || descriptor == "S" || descriptor == "I") {
    auto val = objRef->mSlots->getInt(slotId);
    //pushOperandStack<int32_t>(stack, val);
    stack.pushInt(val);
    stack.dump();
  } else if (descriptor == "F") {
    auto val = objRef->mSlots->getFloat(slotId);
    //pushOperandStack<float>(stack, val);
    stack.pushFloat(val);
  } else if (descriptor == "J") {
    auto val = objRef->mSlots->getLong(slotId);
    //pushOperandStack<int64_t>(stack, val);
    stack.pushLong(val);
  } else if (descriptor == "D") {
    auto val = objRef->mSlots->getDouble(slotId);
    //pushOperandStack<double>(stack, val);
    stack.pushDouble(val);
  } else if (descriptor == "L" || descriptor == "[") {
    auto val = objRef->mSlots->getRef(slotId);
    //pushOperandStack<void*>(stack, val);
    stack.pushRef(val);
  }
}
void PUT_FIELD::execute(std::shared_ptr<rtda::Frame> frame) {
  auto cp = frame->getMethod()->mClassPtr->mConstantPool;
  auto fieldRef = std::static_pointer_cast<rtda::FieldRefConstant>(cp->getConstant(index));
  auto field = fieldRef->resolveField();
  if (field->isStatic()) {
    throw std::runtime_error("java.lang.IncompatibleClassChangeError");
  }
  if (field->isFinal()) {
    if (frame->getMethod()->mClassPtr != field->mClassPtr || frame->getMethod()->mName != "<init>") {
      throw std::runtime_error("java.lang.IllegalAccessError");
    }
  }
  auto& stack = frame->getOperandStack();
  
  auto descriptor = field->mDescriptor;
  auto slotId = field->mSlotId;
  if (descriptor == "Z" || descriptor == "B" || descriptor == "C" || descriptor == "S" || descriptor == "I") {
    //auto val = popOperandStack<int32_t>(stack);
    LOG(INFO) << "PUT_FIELD before popInt";
    stack.dump();
    auto val = stack.popInt();
    auto ref = stack.popRef();
    if (ref == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    LOG(INFO) << "PUT_FIELD after popInt";
    stack.dump();
    rtda::Object* objRef = static_cast<rtda::Object*>(ref);
    //ref->getFields()->setInt(slotId, val);
    objRef->mSlots->setInt(slotId, val);
  } else if (descriptor == "F") {
    //auto val = popOperandStack<float>(stack);
    auto val = stack.popFloat();
    auto ref = stack.popRef();
    if (ref == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    rtda::Object* objRef = static_cast<rtda::Object*>(ref);
    objRef->mSlots->setFloat(slotId, val);
  } else if (descriptor == "J") {
    //auto val = popOperandStack<int64_t>(stack);
    auto val = stack.popLong();
    auto ref = stack.popRef();
    if (ref == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    rtda::Object* objRef = static_cast<rtda::Object*>(ref);
    objRef->mSlots->setLong(slotId, val);
  } else if (descriptor == "D") {
    //auto val = popOperandStack<double>(stack);
    auto val = stack.popDouble();
    auto ref = stack.popRef();
    if (ref == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    rtda::Object* objRef = static_cast<rtda::Object*>(ref);
    objRef->mSlots->setDouble(slotId, val);
  } else if (descriptor == "L" || descriptor == "[") {
    //auto val = popOperandStack<void*>(stack);
    auto val = stack.popRef();
    auto ref = stack.popRef();
    if (ref == nullptr) {
      throw std::runtime_error("java.lang.NullPointerException");
    }
    rtda::Object* objRef = static_cast<rtda::Object*>(ref);
    objRef->mSlots->setRef(slotId, val);
  }
}
void INSTANCE_OF::execute(std::shared_ptr<rtda::Frame> frame) {
  auto cp = frame->getMethod()->mClassPtr->mConstantPool;
  auto classRef = std::static_pointer_cast<rtda::ClassRefConstant>(cp->getConstant(index));
  auto classPtr = classRef->resolveClass();
  auto stack = frame->getOperandStack();
  auto ref = stack.popRef();
  if (ref == nullptr) {
    stack.pushInt(0);
  } else {
    auto objRef = std::make_shared<rtda::Object>(*static_cast<rtda::Object*>(ref));
    if (objRef->isInstanceOf(classPtr)) {
      stack.pushInt(1);
    } else {
      stack.pushInt(0);
    }
  }
}
void CHECK_CAST::execute(std::shared_ptr<rtda::Frame> frame) {
  auto cp = frame->getMethod()->mClassPtr->mConstantPool;
  auto classRef = std::static_pointer_cast<rtda::ClassRefConstant>(cp->getConstant(index));
  auto classPtr = classRef->resolveClass();
  auto stack = frame->getOperandStack();
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
  auto stack = frame->getOperandStack();
  auto cp = frame->getMethod()->mClassPtr->mConstantPool;
  auto c = cp->getConstant(index);
  switch (c->mTag) {
    case rtda::CONSTANT_Integer: {
      auto intC = std::static_pointer_cast<rtda::IntegerConstant>(c);
      stack.pushInt(intC->mValue);
      break;
    }
    case rtda::CONSTANT_Float: {
      auto floatC = std::static_pointer_cast<rtda::FloatConstant>(c);
      stack.pushFloat(floatC->mValue);
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
  auto stack = frame->getOperandStack();
  auto cp = frame->getMethod()->mClassPtr->mConstantPool;
  auto c = cp->getConstant(index);
  switch (c->mTag) {
    case rtda::CONSTANT_Long: {
      auto longC = std::static_pointer_cast<rtda::LongConstant>(c);
      stack.pushLong(longC->mValue);
      break;
    }
    case rtda::CONSTANT_Double: {
      auto doubleC = std::static_pointer_cast<rtda::DoubleConstant>(c);
      stack.pushDouble(doubleC->mValue);
      break;
    }
    default:
      throw std::runtime_error("todo: ldc2_w!");
  }
}
void INVOKE_SPECIAL::execute(std::shared_ptr<rtda::Frame> frame) {
  frame->getOperandStack().popRef();
  // auto cp = frame->getMethod()->mClassPtr->mConstantPool;
  // auto methodRef = std::static_pointer_cast<rtda::MethodRefConstant>(cp->getConstant(index));
  // auto resolvedMethod = methodRef->resolveMethod();
  // auto methodClass = resolvedMethod->mClassPtr;
  // auto stack = frame->getOperandStack();
  // auto ref = stack.popRef();
  // if (ref == nullptr) {
  //   throw std::runtime_error("java.lang.NullPointerException");
  // }
  // if (resolvedMethod->mName == "<init>" && resolvedMethod->mClassPtr != methodClass) {
  //   throw std::runtime_error("java.lang.NoSuchMethodError");
  // }
  // if (resolvedMethod->isStatic()) {
  //   throw std::runtime_error("java.lang.IncompatibleClassChangeError");
  // }
  // auto methodToBeInvoked = resolvedMethod;
  // if (methodClass->mJClass->isInterface() || methodToBeInvoked->mClassPtr->mJClass->isInterface()) {
  //   throw std::runtime_error("java.lang.IncompatibleClassChangeError");
  // }
  // auto methodToBeInvoked = rtda::lookupMethodInClass(methodClass, methodRef->mName, methodRef->mDescriptor);
  // if (methodToBeInvoked == nullptr || methodToBeInvoked->isAbstract()) {
  //   throw std::runtime_error("java.lang.AbstractMethodError");
  // }
  // auto argSlotCount = methodToBeInvoked->mArgSlotCount;
  // if (argSlotCount > 0) {
  //   for (int32_t i = argSlotCount - 1; i >= 0; i--) {
  //     auto slot = stack.popSlot();
  //     frame->getLocalVars()->setSlot(i, slot);
  //   }
  // }
  // auto newFrame = std::make_shared<rtda::Frame>(methodToBeInvoked, argSlotCount, frame->getOperandStack());
  // frame->getThread()->pushFrame(newFrame);
}
void INVOKE_VIRTUAL::execute(std::shared_ptr<rtda::Frame> frame) {
  auto cp = frame->getMethod()->mClassPtr->mConstantPool;
  auto methodRef = std::static_pointer_cast<rtda::MethodRefConstant>(cp->getConstant(index));
  if (methodRef->mName == "println") {
    auto& stack = frame->getOperandStack();
    LOG(INFO) << "stack = " << &stack;
    auto descriptor = methodRef->mDescriptor;
    if (descriptor == "(Z)V") {
      auto val = stack.popInt();
      std::cout << val << std::endl;
    } else if (descriptor == "(C)V") {
      auto val = stack.popInt();
      std::cout << (char)val << std::endl;
    } else if (descriptor == "(B)V") {
      auto val = stack.popInt();
      std::cout << val << std::endl;
    } else if (descriptor == "(S)V") {
      auto val = stack.popInt();
      std::cout << val << std::endl;
    } else if (descriptor == "(I)V") {
      auto val = stack.popInt();
      std::cout << val << std::endl;
    } else if (descriptor == "(J)V") {
      auto val = stack.popLong();
      std::cout << val << std::endl;
    } else if (descriptor == "(F)V") {
      auto val = stack.popFloat();
      std::cout << val << std::endl;
    } else if (descriptor == "(D)V") {
      auto val = stack.popDouble();
      std::cout << val << std::endl;
    } else if (descriptor == "(Ljava/lang/String;)V") {
      // auto ref = stack.popRef();
      // auto jString = std::static_pointer_cast<heap::JString>(ref->mExtra);
      // std::cout << jString->mString << std::endl;
    } else {
      throw std::runtime_error("println: " + descriptor);
    }
   stack.popRef();
  }
  // auto resolvedMethod = methodRef->resolveMethod();
  // auto methodClass = resolvedMethod->mClassPtr;
  // auto stack = frame->getOperandStack();
  // auto ref = stack.popRef();
  // if (ref == nullptr) {
  //   throw std::runtime_error("java.lang.NullPointerException");
  // }
  // if (resolvedMethod->mName == "<init>" && resolvedMethod->mClassPtr != methodClass) {
  //   throw std::runtime_error("java.lang.NoSuchMethodError");
  // }
  // if (resolvedMethod->isStatic()) {
  //   throw std::runtime_error("java.lang.IncompatibleClassChangeError");
  // }
  // auto methodToBeInvoked = resolvedMethod;
  // if (methodClass->mJClass->isInterface() || methodToBeInvoked->mClassPtr->mJClass->isInterface()) {
  //   throw std::runtime_error("java.lang.IncompatibleClassChangeError");
  // }
  // auto methodToBeInvoked = rtda::lookupMethodInClass(methodClass, methodRef->mName, methodRef->mDescriptor);
  // if (methodToBeInvoked == nullptr || methodToBeInvoked->isAbstract()) {
  //   throw std::runtime_error("java.lang.AbstractMethodError");
  // }
  // auto argSlotCount = methodToBeInvoked->mArgSlotCount;
  // if (argSlotCount > 0) {
  //   for (int32_t i = argSlotCount - 1; i >= 0; i--) {
  //     auto slot = stack.popSlot();
  //     frame->getLocalVars()->setSlot(i, slot);
  //   }
  // }
  // auto newFrame = std::make_shared<rtda::Frame>(methodToBeInvoked, argSlotCount, frame->getOperandStack());
  // frame->getThread()->pushFrame(newFrame);
}

}//namespace instructions