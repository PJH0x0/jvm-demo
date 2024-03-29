#include "class.h"
#include "array.h"
#include <memory>
#include "class_member.h"
#include <runtime/constant_pool.h>
#include "method.h"
#include "field.h"
#include "object.h"
#include <jvm.h>
#include <runtime/alloc/heap.h>
#include <runtime/class_loader.h>
#include <runtime/string_pool.h>
#include <runtime/frame.h>
#include <glog/logging.h>
#include <string>
#include <unordered_map>
#include <utility>

namespace runtime {
Class::PrimitiveTypes Class::primitive_types_ = {
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
Class::Class() : Object() {
  Init();
}
//Class::Class(std::string name) : Object(), name_(std::move(name)) {
//  Init();
//}

void Class::Init() {
  interface_names_ = new std::vector<std::string>();
  fields_ = new std::vector<Field*>();
  methods_ = new std::vector<Method*>();
  interfaces_ = new std::vector<Class*>();
}
void Class::StartLoad(const std::shared_ptr<const classfile::ClassFile>& class_file) {
  loader_ = ClassLoader::GetBootClassLoader();
  access_flags_ = class_file->GetAccessFlags();
  std::shared_ptr<classfile::ConstantPool> cf_constant_pool = class_file->GetConstantPool();
  name_ = class_file->GetClassName();

  super_class_name_ = class_file->GetSuperClassName();
  super_class_ = loader_->ResolveSuperClass(this);
  source_file_ = class_file->GetSourceFile();

  class_file->GetInterfaceNames(interface_names_);
  loader_->ResolveInterfaces(this, interfaces_);
  
  CreateFields(this, class_file->GetFields(), fields_);
  constant_pool_ = new ConstantPool(this, cf_constant_pool);
  CreateMethods(this, class_file->GetMethods(), methods_);
  loaded_ = true;
}
void Class::StartLoadArrayClass() {
  loader_ = ClassLoader::GetBootClassLoader();
  access_flags_ = ACC_PUBLIC;
  super_class_name_ = "java/lang/Object";
  super_class_ = loader_->LoadClass(super_class_name_);
  interfaces_->push_back(loader_->LoadClass("java/lang/Cloneable"));
  interfaces_->push_back(loader_->LoadClass("java/io/Serializable"));
  loaded_ = true;
}
void Class::InitClass(Class* klass) {
  klass->StartClinit();
  ScheduleClinit(klass);
  InitSuperClass(klass);
}
void Class::ScheduleClinit(Class* klass) {
  const Method* clinit_method = klass->GetClinitMethod();
  if (clinit_method != nullptr && clinit_method->GetClass() == klass) {
    Frame* newFrame = Thread::Current()->CreateFrame(clinit_method);
    LOG_IF(INFO, INST_DEBUG) << "invoke clinit method: "
        << clinit_method->GetName() << " in class: " << klass->GetName();
  }
}
void Class::InitSuperClass(Class* klass) {
  if (!klass->IsInterface()) {
    if (klass->GetSuperClass() != nullptr && !klass->GetSuperClass()->IsClinitStarted()) {
      InitClass(klass->GetSuperClass());
    }
  }
  
}
const Field* Class::GetField(std::string name, std::string descriptor, bool is_static) {
  const Field* field = LookupField(name, descriptor);
  if (field == nullptr) {
    LOG(FATAL) << "java.lang.NoSuchFieldError";
  }
  if (is_static != field->IsStatic()) {
    LOG(FATAL) << "java.lang.IncompatibleClassChangeError";
  }
  return field;
}
const Method* Class::GetMethod(const std::string& name, const std::string& descriptor, bool is_static) {
  for (auto method : *methods_) {
    if (method->GetName() == name && method->GetDescriptor() == descriptor
        && method->IsStatic() == is_static) {
      return method;
    }
  }
  if (super_class_ != nullptr) {
    return super_class_->GetMethod(name, descriptor, is_static);
  }
  return nullptr;
}
const Field* Class::LookupField(std::string name, std::string descriptor) {
  for (const auto* field : *fields_) {
    if (field->GetName() == name && field->GetDescriptor() == descriptor) {
      return field;
    }
  }
  for (auto interface : *interfaces_) {
    const Field* field = interface->LookupField(name, descriptor);
    if (field != nullptr) {
      return field;
    }
  }
  if (super_class_ != nullptr) {
    const Field* field = super_class_->LookupField(name, descriptor);
    if (field != nullptr) {
      return field;
    }
  }
  return nullptr;
}

const Method* Class::LookupMethod(const std::string& name, const std::string& descriptor) {
  const Method* method = LookupMethodInClass(name, descriptor);
  if (method == nullptr) {
    method = LookupMethodInInterfaces(name, descriptor);
  }
  return method;
}

const Method* Class::LookupMethodInClass(const std::string& name, const std::string& descriptor) {
  for (auto method : *methods_) {
    if (method->GetName() == name && method->GetDescriptor() == descriptor) {
      return method;
    }
  }
  if (super_class_ != nullptr) {
    return super_class_->LookupMethodInClass(name, descriptor);
  }
  return nullptr;
}

const Method* Class::LookupMethodInInterfaces(const std::string& name, const std::string& descriptor) {
  const Method* method = nullptr;
  for (auto interface : *interfaces_) {
    method = interface->LookupMethodInClass(name, descriptor);
  }
  return method;
}

Object* Class::NewObject() {
  return new Object();
}

bool Class::IsSubClassOf(Class* s, Class* t) {
  Class* c = s->GetSuperClass();
  while (c != nullptr) {
    if (c == t) {
      return true;
    }
    c = c->super_class_;
  }
  return false;
}

bool Class::IsSuperClassOf(Class* s, Class* t) {
  Class* c = t->super_class_;
  while (c != nullptr) {
    if (c == s) {
      return true;
    }
    c = c->super_class_;
  }
  return false;
}

bool Class::IsImplements(Class* s, Class* t) {
  for (auto interface : *(s->GetInterfaces())) {
    if (interface == t || IsSubInterfaceOf(interface, t)) {
      return true;
    }
  }
  return false;
}
bool Class::IsSubInterfaceOf(Class* s, Class* t) {
  for (auto interface : *(s->GetInterfaces())) {
    if (interface == t || IsSubInterfaceOf(interface, t)) {
      return true;
    }
  }
  return false;
}
bool Class::IsSuperInterfaceOf(Class* s, Class* t) {
  return IsSubInterfaceOf(t, s);
}
bool Class::IsAssignableFrom(Class* s, Class* t) {
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
  if (!t->IsArrayClass()) {
    if (!t->IsInterface()) {
      if (!s->IsInterface()) {
        return IsSubClassOf(s, t);
      } else {
        return IsImplements(s, t);
      }
    } else {
      if (!s->IsInterface()) {
        return IsJlObject(s);
      } else {
        return IsSuperInterfaceOf(s, t);
      }
    }
  } else {
    if (!s->IsArrayClass()) {
      if (!s->IsInterface()) {
        return IsJlObject(s);
      } else {
        return IsJlCloneable(s) || IsJioSerializable(s);
      }
    } else {
      Class* sc = s->GetComponentClass();
      Class* tc = t->GetComponentClass();
      return IsAssignableFrom(sc, tc);
    }
  }
  return false;
}
bool Class::IsJlObject(Class* c) {
  return c->name_ == "java/lang/Object";
}
bool Class::IsJlCloneable(Class* c) {
  return c->name_ == "java/lang/Cloneable";
}
bool Class::IsJioSerializable(Class* c) {
  return c->name_ == "java/io/Serializable";
}
const Method* Class::GetMainMethod() {
  return GetMethod("main", "([Ljava/lang/String;)V", true);
}
const Method* Class::GetClinitMethod() {
  return GetMethod("<clinit>", "()V", true);
}
Object* Class::NewArray(uint32_t count) {
  if (!IsArrayClass()) {
    LOG(FATAL) << "Not array class";
  }
  switch (name_[1]) {
    case 'Z':
      return new Object();
    case 'B':
      return new Object();
    case 'C':
      return new Object();
    case 'S':
      return new Object();
    case 'I':
      return new Object();
    case 'J':
      return new Object();
    case 'F':
      return new Object();
    case 'D':
      return new Object();
    default:
      return new Object();
  }
}
Class* Class::GetPrimitiveArrayClass(uint8_t arr_type) {
  const auto classLoader = ClassLoader::GetBootClassLoader();
  switch (arr_type) {
    case kBoolean:
      return classLoader->LoadClass("[Z");
    case kByte:
      return classLoader->LoadClass("[B");
    case kChar:
      return classLoader->LoadClass("[C");
    case kShort:
      return classLoader->LoadClass("[S");
    case kInt:
      return classLoader->LoadClass("[I");
    case kLong:
      return classLoader->LoadClass("[J");
    case kFloat:
      return classLoader->LoadClass("[F");
    case kDouble:
      return classLoader->LoadClass("[D");
    default:
      LOG(FATAL) << "Invalid atype: " << arr_type;
  }
}

Class* Class::GetArrayClass() {
  std::string arrayClassName = GetArrayClassName(name_);
  return loader_->LoadClass(arrayClassName);
}
std::string Class::ToDescriptor(std::string class_name) {
  if (class_name[0] == '[') {
    return class_name;
  }
  for (auto& pair : primitive_types_) {
    if (pair.first == class_name) {
      return pair.second;
    }
  }
  return "L" + class_name + ";";
  LOG(FATAL) << "Invalid class name: " << class_name;
}
std::string Class::GetArrayClassName(std::string class_name) {
  return "[" + ToDescriptor(class_name);
}
std::string Class::ToClassName(std::string descriptor) {
  if (descriptor[0] == '[') {
    return descriptor;
  }
  if (descriptor[0] == 'L') {
    return descriptor.substr(1, descriptor.size() - 2);
  }
  for (auto& pair : primitive_types_) {
    if (pair.second == descriptor) {
      return pair.first;
    }
  }
  LOG(FATAL) << "Invalid descriptor: " << descriptor;
}
std::string Class::GetComponentClassName(std::string class_name) {
  if (class_name[0] == '[') {
    std::string componentTypeDescriptor = class_name.substr(1, class_name.size() - 1);
    return ToClassName(componentTypeDescriptor);
  }
  LOG(FATAL) << "Not array class: " << class_name;
}
Class* Class::GetComponentClass() {
  std::string componentClassName = GetComponentClassName(name_);
  return loader_->LoadClass(componentClassName);
}
Class* Class::NewClassObject() {
  auto object = JVM::Current()->GetHeap()->AllocObject(runtime::Thread::Current(), nullptr, sizeof(Class));
  return dynamic_cast<Class*>(object);
}
Object* Class::NewJString(std::string str) {
  auto& stringPool = StringPool::getStringPool();
  auto it = stringPool.find(str);
  if (it != stringPool.end()) {
    return it->second;
  }
  ClassLoader* classLoader = ClassLoader::GetBootClassLoader();
  std::u16string u16str = StringConstant::Utf8ToUtf16(str);
  size_t utf16Size = u16str.size();
  Class* stringClass = classLoader->LoadClass("java/lang/String");
  Object* jstr = stringClass->NewObject();
  //Object* jChars = new Object(boot_class_loader_->LoadClass("[C"), utf16Size, kChar);
  Object* jChars = new Object();
  const char16_t* u16strPtr = u16str.c_str();
  for (uint32_t i = 0; i < utf16Size; i++) {
    //jChars->setArrayElement<char16_t>(i, u16strPtr[i]);
  }
  //Set utf16 terminator '\0', u'\0' == 0xFEFF0000
  //jChars->setArrayElement<char16_t>(utf16Size, u'\0');
  jstr->SetRefVar("value", "[C", jChars);
  stringPool[str] = jstr;
  return jstr;
}
void Class::CreateFields(Class* class_ptr, const std::vector<std::shared_ptr<classfile::MemberInfo>>& cf_fields,
                         std::vector<Field*>* fields) {
  for (auto& cf_field : cf_fields) {
    auto* field = new Field(cf_field, class_ptr);
    fields->push_back(field);
  }
}
void Class::CreateMethods(Class* class_ptr, const std::vector<std::shared_ptr<classfile::MemberInfo>>& cf_methods,
                          std::vector<Method*>* methods) {
  for (auto& cf_method: cf_methods) {
    auto* method = new Method(cf_method, class_ptr);
    methods->push_back(method);
  }
}

const Class::PrimitiveTypes& Class::GetPrimitiveTypes() {
  return primitive_types_;
}

}