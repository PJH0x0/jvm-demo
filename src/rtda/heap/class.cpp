#include "class.h"
#include <memory>
#include "class_member.h"
#include "constant_pool.h"
#include "method.h"
#include "field.h"
#include "object.h"
#include "class_loader.h"
#include <rtda/frame.h>
#include <glog/logging.h>
#include <stdint.h>
#include <string>
#include <unordered_map>

namespace rtda {
std::unordered_map<std::string, std::string> Class::mPrimitiveTypes = {
  {"void", "V"},
  {"boolean", "Z"},
  {"byte", "B"},
  {"short", "S"},
  {"int", "I"},
  {"long", "J"},
  {"char", "C"},
  {"float", "F"},
  {"double", "D"}
};
std::unordered_map<std::string, Object*> Class::mStringPool;
Class::Class(std::shared_ptr<classfile::ClassFile> classfile) 
  : mClassfile(classfile),
    mAccessFlags(0), 
    mLoaded(false),
    mClinitStarted(false),
    mLoader(nullptr), 
    mInstanceSlotCount(0),
    mStaticSlotCount(0){}
Class::Class(std::string name) : mName(name) {}
void Class::startLoad() {
  mLoader = ClassLoader::getBootClassLoader(nullptr);
  mAccessFlags = mClassfile->accessFlags;
  std::shared_ptr<classfile::ConstantPool> constantPool = mClassfile->constantPool;
  mName = mClassfile->getClassName();

  mSuperClassName = mClassfile->getSuperClassName();
  mSuperClass = mLoader->resolveSuperClass(this);

  mClassfile->getInterfaceNames(mInterfaceNames);
  mLoader->resolveInterfaces(this, mInterfaces);
  
  std::shared_ptr<Class> thisptr = std::shared_ptr<Class>(this);
  //TODO: init fileds
  createFields(thisptr, mClassfile->fields, mFields);
  //TODO: init constant pool
  mConstantPool = std::make_shared<ConstantPool>(thisptr, constantPool);
  //TODO: init methods
  createMethods(thisptr, mClassfile->methods, mMethods);
  mLoaded = true;
}
void Class::startLoadArrayClass() {
  mLoader = ClassLoader::getBootClassLoader(nullptr);
  mAccessFlags = ACC_PUBLIC;
  mSuperClassName = "java/lang/Object";
  mSuperClass = mLoader->loadClass(mSuperClassName);
  mInterfaces.push_back(mLoader->loadClass("java/lang/Cloneable"));
  mInterfaces.push_back(mLoader->loadClass("java/io/Serializable"));
  mLoaded = true;
}
void Class::initClass(std::shared_ptr<Thread> thread, std::shared_ptr<Class> klass) {
  klass->startClinit();
  scheduleClinit(thread, klass);
  initSuperClass(thread, klass);
}
void Class::scheduleClinit(std::shared_ptr<Thread> thread, std::shared_ptr<Class> klass) {
  std::shared_ptr<Method> clinitMethod = klass->getClinitMethod();
  if (clinitMethod != nullptr && clinitMethod->getClass() == klass) {
    std::shared_ptr<Frame> newFrame = std::make_shared<Frame>(thread, clinitMethod->getMaxLocals(), 
                                                              clinitMethod->getMaxStack(), clinitMethod);
    thread->pushFrame(newFrame);
  }
}
void Class::initSuperClass(std::shared_ptr<Thread> thread, std::shared_ptr<Class> klass) {
  if (!klass->isInterface()) {
    if (klass->getSuperClass() != nullptr && !klass->getSuperClass()->isClinitStarted()) {
      initClass(thread, klass->getSuperClass());
    }
  }
  
}
std::shared_ptr<Field> Class::getField(std::string name, std::string descriptor, bool isStatic) {
  std::shared_ptr<Field> field = lookupField(name, descriptor);
  if (field == nullptr) {
    LOG(ERROR) << "java.lang.NoSuchFieldError";
  }
  if (isStatic != field->isStatic()) {
    LOG(ERROR) << "java.lang.IncompatibleClassChangeError";
  }
  return field;
}
std::shared_ptr<Field> Class::lookupField(std::string name, std::string descriptor) {
  for (auto field : mFields) {
    if (field->getName() == name && field->getDescriptor() == descriptor) {
      return field;
    }
  }
  for (auto interface : mInterfaces) {
    std::shared_ptr<Field> field = interface->lookupField(name, descriptor);
    if (field != nullptr) {
      return field;
    }
  }
  if (mSuperClass != nullptr) {
    std::shared_ptr<Field> field = mSuperClass->lookupField(name, descriptor);
    if (field != nullptr) {
      return field;
    }
  }
  return nullptr;
}

std::shared_ptr<Method> Class::lookupMethod(std::string name, std::string descriptor) {
  std::shared_ptr<Method> method = lookupMethodInClass(name, descriptor);
  if (method == nullptr) {
    method = lookupMethodInInterfaces(name, descriptor);
  }
  return method;
}

std::shared_ptr<Method> Class::lookupMethodInClass(std::string name, std::string descriptor) {
  for (auto method : mMethods) {
    if (method->getName() == name && method->getDescriptor() == descriptor) {
      return method;
    }
  }
  if (mSuperClass != nullptr) {
    return mSuperClass->lookupMethodInClass(name, descriptor);
  }
  return nullptr;
}

std::shared_ptr<Method> Class::lookupMethodInInterfaces(std::string name, std::string descriptor) {
  for (auto interface : mInterfaces) {
    std::shared_ptr<Method> method = interface->lookupMethodInClass(name, descriptor);
    if (method != nullptr) {
      return method;
    }
  }
  return nullptr;
}

Object* Class::newObject() {
  return new Object(std::shared_ptr<Class>(this));
}

bool Class::isSubClassOf(std::shared_ptr<Class> s, std::shared_ptr<Class> t) {
  std::shared_ptr<Class> c = s->getSuperClass();
  while (c != nullptr) {
    if (c == t) {
      return true;
    }
    c = c->mSuperClass;
  }
  return false;
}

bool Class::isSuperClassOf(std::shared_ptr<Class> s, std::shared_ptr<Class> t) {
  std::shared_ptr<Class> c = t->mSuperClass;
  while (c != nullptr) {
    if (c == s) {
      return true;
    }
    c = c->mSuperClass;
  }
  return false;
}

bool Class::isImplements(std::shared_ptr<Class> s, std::shared_ptr<Class> t) {
  for (auto interface : s->getInterfaces()) {
    if (interface == t || isSubInterfaceOf(interface, t)) {
      return true;
    }
  }
  return false;
}
bool Class::isSubInterfaceOf(std::shared_ptr<Class> s, std::shared_ptr<Class> t) {
  for (auto interface : s->getInterfaces()) {
    if (interface == t || isSubInterfaceOf(interface, t)) {
      return true;
    }
  }
  return false;
}
bool Class::isSuperInterfaceOf(std::shared_ptr<Class> s, std::shared_ptr<Class> t) {
  return isSubInterfaceOf(t, s);
}
bool Class::isAssignableFrom(std::shared_ptr<Class> s, std::shared_ptr<Class> t) {
  if (t == nullptr) {
    return false;
  }
  if (s == t) {
    return true;
  }
  /**
   * @brief 
   * 1. if s is normal class and t is normal class, s is subclass of t
   * 2. if s is interface and t is normal class, t implements s
   * 3. if s is normal class and t is interface, s is java.lang.Object
   * 4. if s is interface and t is interface, s is super interface of t
   *  
   * 5. if s is normal class and t is array class, s is java.lang.Object
   * 6. if s is interface and t is array class, s is java.lang.Cloneable or java.io.Serializable
   * 7. if s is array class and t is array class, s's component class is assignable from t's component class
   */
  if (!t->isArrayClass()) {
    if (!t->isInterface()) {
      if (!s->isInterface()) {
        return isSubClassOf(s, t);
      } else {
        return isImplements(s, t);
      }
    } else {
      if (!s->isInterface()) {
        return isJlObject(s);
      } else {
        return isSuperInterfaceOf(s, t);
      }
    }
  } else {
    if (!s->isArrayClass()) {
      if (!s->isInterface()) {
        return isJlObject(s);
      } else {
        return isJlCloneable(s) || isJioSerializable(s);
      }
    } else {
      std::shared_ptr<Class> sc = s->getComponentClass();
      std::shared_ptr<Class> tc = t->getComponentClass();
      return isAssignableFrom(sc, tc);
    }
  }
  return false;
}
bool Class::isJlObject(std::shared_ptr<Class> c) {
  return c->mName == "java/lang/Object";
}
bool Class::isJlCloneable(std::shared_ptr<Class> c) {
  return c->mName == "java/lang/Cloneable";
}
bool Class::isJioSerializable(std::shared_ptr<Class> c) {
  return c->mName == "java/io/Serializable";
}
std::shared_ptr<Method> Class::getMainMethod() {
  return getStaticMethod("main", "([Ljava/lang/String;)V");
}
std::shared_ptr<Method> Class::getStaticMethod(std::string name, std::string descriptor) {
  for (auto method : mMethods) {
    if (method->getName() == name && method->getDescriptor() == descriptor && method->isStatic()) {
      return method;
    }
  }
  return nullptr;
}
std::shared_ptr<Method> Class::getClinitMethod() {
  return getStaticMethod("<clinit>", "()V");
}
Object* Class::newArray(uint32_t count) {
  if (!isArrayClass()) {
    LOG(FATAL) << "Not array class";
  }
  std::shared_ptr<Class> thisPtr = std::shared_ptr<Class>(this);
  switch (mName[1]) {
    case 'Z':
      return new Object(thisPtr, count, AT_BOOLEAN);
    case 'B':
      return new Object(thisPtr, count, AT_BYTE);
    case 'C':
      return new Object(thisPtr, count, AT_CHAR);
    case 'S':
      return new Object(thisPtr, count, AT_SHORT);
    case 'I':
      return new Object(thisPtr, count, AT_INT);
    case 'J':
      return new Object(thisPtr, count, AT_LONG);
    case 'F':
      return new Object(thisPtr, count, AT_FLOAT);
    case 'D':
      return new Object(thisPtr, count, AT_DOUBLE);
    default:
      return new Object(thisPtr, count, AT_OBJECT);
  }
}
std::shared_ptr<Class> Class::getPrimitiveArrayClass(
                                                     uint8_t atype) {
  std::shared_ptr<ClassLoader> classLoader = ClassLoader::getBootClassLoader(nullptr);
  switch (atype) {
    case AT_BOOLEAN:
      return classLoader->loadClass("[Z");
    case AT_BYTE:
      return classLoader->loadClass("[B");
    case AT_CHAR:
      return classLoader->loadClass("[C");
    case AT_SHORT:
      return classLoader->loadClass("[S");
    case AT_INT:
      return classLoader->loadClass("[I");
    case AT_LONG:
      return classLoader->loadClass("[J");
    case AT_FLOAT:
      return classLoader->loadClass("[F");
    case AT_DOUBLE:
      return classLoader->loadClass("[D");
    default:
      LOG(FATAL) << "Invalid atype: " << atype;
  }
}

std::shared_ptr<Class> Class::getArrayClass() {
  std::string arrayClassName = getArrayClassName(mName);
  return mLoader->loadClass(arrayClassName);
}
std::string Class::toDescriptor(std::string className) {
  if (className[0] == '[') {
    return className;
  }
  for (auto& pair : mPrimitiveTypes) {
    if (pair.first == className) {
      return pair.second;
    }
  }
  return "L" + className + ";";
  LOG(FATAL) << "Invalid class name: " << className;
}
std::string Class::getArrayClassName(std::string className) {
  return "[" + toDescriptor(className);
}
std::string Class::toClassName(std::string descriptor) {
  if (descriptor[0] == '[') {
    return descriptor;
  }
  if (descriptor[0] == 'L') {
    return descriptor.substr(1, descriptor.size() - 2);
  }
  for (auto& pair : mPrimitiveTypes) {
    if (pair.second == descriptor) {
      return pair.first;
    }
  }
  LOG(FATAL) << "Invalid descriptor: " << descriptor;
}
std::string Class::getComponentClassName(std::string className) {
  if (className[0] == '[') {
    std::string componentTypeDescriptor = className.substr(1, className.size() - 1);
    return toClassName(componentTypeDescriptor);
  }
  LOG(FATAL) << "Not array class: " << className;
}
std::shared_ptr<Class> Class::getComponentClass() {
  std::string componentClassName = getComponentClassName(mName);
  return mLoader->loadClass(componentClassName);
}
Object* Class::newJString(std::string str) {
  auto it = mStringPool.find(str);
  if (it != mStringPool.end()) {
    return it->second;
  }
  std::shared_ptr<ClassLoader> classLoader = ClassLoader::getBootClassLoader(nullptr);
  std::u16string u16str = StringConstant::utf8ToUtf16(str);
  size_t utf16Size = u16str.size();
  std::shared_ptr<Class> stringClass = classLoader->loadClass("java/lang/String");
  Object* jstr = stringClass->newObject();
  Object* jChars = new Object(classLoader->loadClass("[C"), utf16Size, AT_CHAR);
  for (uint32_t i = 0; i < utf16Size; i++) {
    jChars->setArrayElement<char16_t>(i, u16str[i]);
  }
  jstr->setRefVar("value", "[C", jChars);
  mStringPool[str] = jstr;
  return jstr;
}                                            
}