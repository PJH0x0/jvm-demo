#include "class_loader.h"
#include <classfile/class_parser.h>
#include "oo/field.h"
#include "oo/class.h"
#include "constant_pool.h"
#include "oo/object.h"
#include <memory>
#include <runtime/slots.h>
#include <stdint.h>
#include <glog/logging.h>
#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>


namespace runtime {
std::unordered_map<std::string, std::shared_ptr<ClassLoader>> ClassLoader::loaders_;
std::shared_ptr<ClassLoader> ClassLoader::boot_class_loader_;
std::once_flag class_loader_flag;
Class* ClassLoader::LoadClass(std::string name) {
  //LOG(INFO) << "load class " << name;
  if (loaded_classes_.find(name) != loaded_classes_.end()) {
    return loaded_classes_[name];
  }
  Class* clss_ptr = nullptr;
  if (name[0] == '[') {
    clss_ptr = LoadArrayClass(name);
  } else {
    clss_ptr = LoadNonArrayClass(name);
  }
  auto jlc_class_ptr = loaded_classes_["java/lang/Class"];
  if (jlc_class_ptr != nullptr) {
    //class object's class is java/lang/Class
    //clss_ptr->setJClass(jlc_class_ptr->NewObject());
    //Object's class object's extra is point to Object's class
    //clss_ptr->getJClass()->setExtra(clss_ptr.get());
  }
  return clss_ptr;
}

Class* ClassLoader::LoadArrayClass(std::string name) {
  auto* class_ptr = new Class(name);
  //class_ptr->startInit(this);
  class_ptr->StartLoadArrayClass();
  loaded_classes_[name] = class_ptr;
  return class_ptr;
}
Class* ClassLoader::LoadNonArrayClass(std::string name) {
  std::shared_ptr<classfile::ClassData> class_data = class_path_reader->ReadClass(name);
  Class* class_ptr = DefineClass(class_data);
  LinkClass(class_ptr);
  return class_ptr;
}
Class* ClassLoader::DefineClass(std::shared_ptr<classpath::ClassData> data) {
  std::shared_ptr<classfile::ClassFile> class_file = classfile::Parse(data);
  if (class_file == nullptr) {
    LOG(ERROR) << "parse class file failed";
  }
  auto* class_ptr = new Class(class_file);
  class_ptr->StartLoad();
  loaded_classes_[class_ptr->GetName()] = class_ptr;
  return class_ptr;
}
Class* ClassLoader::ResolveSuperClass(Class* class_ptr) {
  if (class_ptr->GetName() != "java/lang/Object") {
    return LoadClass(class_ptr->GetSuperClassName());
  }
  return nullptr;
}
void ClassLoader::ResolveInterfaces(Class* class_ptr, std::vector<Class*>& interfaces) {
  int interfaceCount = class_ptr->GetInterfaceNames().size();
  if (interfaceCount > 0) {
    interfaces.resize(interfaceCount);
    for (int i = 0; i < interfaceCount; i++) {
      interfaces[i] = LoadClass(class_ptr->GetInterfaceNames()[i]);
    }
  }
}
void LinkClass(Class* class_ptr) {
  VerifyClass(class_ptr);
  PrepareClass(class_ptr);
}
void VerifyClass(Class* class_ptr) {
  // todo
}
void PrepareClass(Class* class_ptr) {
  CalcInstanceFieldSlotIds(class_ptr);
  CalcStaticFieldSlotIds(class_ptr);
  AllocAndInitStaticVars(class_ptr);
}
void CalcInstanceFieldSlotIds(Class* class_ptr) {
  int slot_id = 0;
  if (class_ptr->GetSuperClass() != nullptr) {
    slot_id = class_ptr->GetSuperClass()->GetInstanceSlotCount();
  }
  for (auto field : class_ptr->GetFields()) {
    if (!field->IsStatic()) {
      //field->GetSlotId() = slot_id;
      field->SetSlotId(slot_id);
      slot_id++;
      if (field->IsLongOrDouble()) {
        slot_id++;
      }
    }
  }
  class_ptr->SetInstanceSlotCount(slot_id);
}
void CalcStaticFieldSlotIds(Class* class_ptr) {
  int slot_id = 0;
  for (auto field : class_ptr->GetFields()) {
    if (field->IsStatic()) {
      //field->GetSlotId() = slot_id;
      field->SetSlotId(slot_id);
      slot_id++;
      if (field->IsLongOrDouble()) {
        slot_id++;
      }
    }
  }
    class_ptr->SetStaticSlotCount(slot_id);
}
void AllocAndInitStaticVars(Class* class_ptr) {
  auto static_vars = std::make_shared<Slots>(class_ptr->GetStaticSlotCount());
  class_ptr->SetStaticVars(static_vars);
  for (auto field : class_ptr->GetFields()) {
    if (field->IsStatic() && field->IsFinal()) {
      InitStaticFinalVar(class_ptr, field);
    }
  }
}
void InitStaticFinalVar(Class* class_ptr, std::shared_ptr<Field> field) {
  // todo
  std::shared_ptr<ConstantPool> cp = class_ptr->GetConstantPool();
  const std::vector<std::shared_ptr<Constant>>& constants = cp->GetConstants();
  std::shared_ptr<Constant> constant = constants[field->GetConstValueIndex()];
  int slot_id = field->GetSlotId();
  std::string descriptor = field->GetDescriptor();
  switch (descriptor[0]) {
    case 'Z':
    case 'B':
    case 'C':
    case 'S':
    case 'I': {
      std::string name = field->GetName();
      //Skip $assertionsDisabled field, it is added by compiler
      if (name == "$assertionsDisabled") {
        //LOG(INFO) << "Skip $assertionsDisabled field";
        return;
      }
      //If field is static final primitive type, its' value is in constant pool or default value 0
      int32_t value = 0;
      if (nullptr != constant) {
        value = std::static_pointer_cast<IntegerConstant>(constant)->value();
      }

      class_ptr->GetStaticVars()->SetInt(field->GetSlotId(), value);
      break;
    }
    case 'F': {
      float value = 0.0f;
      if (nullptr != constant) {
        value = std::static_pointer_cast<FloatConstant>(constant)->value();
      }
      class_ptr->GetStaticVars()->SetFloat(field->GetSlotId(), value);
      break;
    }
    case 'J': {
      int64_t value = 0;
      if (nullptr != constant) {
        value = std::static_pointer_cast<LongConstant>(constant)->value();
      }
      class_ptr->GetStaticVars()->SetLong(field->GetSlotId(), value);
      break;
    }
    case 'D': {
      double value = 0.0;
      if (nullptr != constant) {
        value = std::static_pointer_cast<DoubleConstant>(constant)->value();
      }
      class_ptr->GetStaticVars()->SetDouble(field->GetSlotId(), value);
      break;
    }
    case 'L':
      break;
    case '[':
      //Object* arr = class_ptr->NewArray(class_ptr->GetLoader(), descriptor);
      //class_ptr->static_vars_[slot_id].mRef = std::static_pointer_cast<StringConstant>(constant)->value();
      //class_ptr->GetStaticVars()->SetRef(field->GetConstValueIndex(), std::static_pointer_cast<FieldRefConstant>(constant)->value());
      break;
    default:
      break;
  }
  if (descriptor == "L/java/lang/String;") {
    std::string str = std::static_pointer_cast<StringConstant>(constant)->value();
    auto j_string = Class::NewJString(str);
    class_ptr->GetStaticVars()->SetRef(field->GetSlotId(), j_string);
  }
}
void ClassLoader::LoadBasicClass() {
  auto jlc_class_ptr = LoadClass("java/lang/Class");
  for (auto& pair : loaded_classes_) {
    //pair.second->setJClass(jlc_class_ptr->NewObject());
    //pair.second->getJClass()->setExtra(pair.second.get());
  }
}

void ClassLoader::LoadPrimitiveClasses() {
  for (auto& pair : Class::primitive_type_map_) {
    auto class_ptr = new Class();
    class_ptr->SetAccessFlags(ACCESS_FLAG::ACC_PUBLIC);
    class_ptr->SetName(pair.first);
    class_ptr->SetClassLoader(GetBootClassLoader(nullptr));
    //class_ptr->StartLoad();
    //class_ptr->setJClass(loaded_classes_["java/lang/Class"]->NewObject());
    //class_ptr->getJClass()->setExtra(class_ptr.get());
    loaded_classes_[pair.first] = class_ptr;
  }
}

std::shared_ptr<ClassLoader> ClassLoader::GetBootClassLoader(
      std::shared_ptr<classpath::ClassPathParser> boot_cls_reader) {
  std::call_once(class_loader_flag, [&]() {
    if (nullptr == boot_cls_reader) {
      LOG(FATAL) << "boot class path is null";
    }
    boot_class_loader_ = std::shared_ptr<ClassLoader>(new ClassLoader(boot_cls_reader));
  });
  
  return boot_class_loader_;
}

std::shared_ptr<ClassLoader> ClassLoader::GetLoader(std::string name) {
  //TODO
  return nullptr;
}

void ClassLoader::RegisterLoader(std::string name, std::shared_ptr<ClassLoader> loader) {
  //TODO
}

}// namespace runtime