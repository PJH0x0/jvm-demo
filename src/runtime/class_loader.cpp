#include "class_loader.h"
#include <classfile/class_parser.h>
#include <classpath/class_reader.h>
#include "oo/field.h"
#include "oo/class.h"
#include "constant_pool.h"
#include <runtime/alloc/heap.h>
#include "oo/object.h"
#include <jvm.h>
#include <memory>
#include <runtime/slots.h>
#include <cstdint>
#include <cassert>
#include <glog/logging.h>
#include <string>
#include <utility>
#include <vector>
#include <mutex>
#include <unordered_map>


namespace runtime {
ClassLoader::ClassLoaderMaps ClassLoader::loaders_{};
const char* ClassLoader::kJlcClassString = "java/lang/Class.class";
static std::once_flag boot_class_loader_flag;
static std::once_flag ext_class_loader_flag;
static std::once_flag base_class_loader_flag;
void ClassLoader::LoadBasicClass() {
  auto jlc_class_ref = LoadClass(kJlcClassString);
  loaded_classes_[kJlcClassString] = jlc_class_ref;
  for (auto& pair : loaded_classes_) {
    pair.second->SetClass(jlc_class_ref);
  }
}

void ClassLoader::LoadPrimitiveClasses() {
  for (auto& pair : Class::GetPrimitiveTypes()) {
    auto class_ref = Class::NewClassObject();
    if (!class_ref) {
      LOG(FATAL) << "Create Class Object failed";
    }
    class_ref->SetAccessFlags(ACCESS_FLAG::ACC_PUBLIC);
    class_ref->SetName(pair.first);
    class_ref->SetClassLoader(this);
    if (loaded_classes_.find(kJlcClassString) != loaded_classes_.end()) {
      class_ref->SetClass(loaded_classes_[kJlcClassString]);
    } else {
      class_ref->SetClass(nullptr);
    }
    loaded_classes_[pair.first] = class_ref;
  }
}

Class* ClassLoader::LoadClass(const std::string& name) {
  //LOG(INFO) << "load class " << name;
  if (loaded_classes_.find(name) != loaded_classes_.end()) {
    return loaded_classes_[name];
  }
  Class* class_ref = nullptr;
  if (name[0] == '[') {
    class_ref = LoadArrayClass(name);
  } else {
    class_ref = LoadNonArrayClass(name);
  }
  auto jlc_class_ref = loaded_classes_[kJlcClassString];
  if (jlc_class_ref != nullptr) {
    class_ref->SetClass(jlc_class_ref);
    //class object's class is java/lang/Class
    //class_ref->setJClass(jlc_class_ref->NewObject());
    //Object's class object's extra is point to Object's class
    //class_ref->getJClass()->setExtra(class_ref.get());
  }
  return class_ref;
}

Class* ClassLoader::LoadArrayClass(const std::string& name) {
  auto* class_ref = Class::NewClassObject();
  class_ref->SetName(name);
  //class_ref->startInit(this);
  class_ref->StartLoadArrayClass();
  loaded_classes_[name] = class_ref;
  return class_ref;
}
Class* ClassLoader::LoadNonArrayClass(const std::string& name) {
  std::shared_ptr<classfile::ClassData> class_data = class_path_reader_->ReadClass(name);
  Class* class_ref = DefineClass(class_data);
  LinkClass(class_ref);
  return class_ref;
}
Class* ClassLoader::DefineClass(const std::shared_ptr<classpath::ClassData>& data) {
  auto class_file = classfile::Parse(data);
  if (class_file == nullptr) {
    LOG(ERROR) << "parse class file failed";
  }
  auto class_ref = Class::NewClassObject();
  class_ref->StartLoad(class_file);
  loaded_classes_[class_ref->GetName()] = class_ref;
  return class_ref;
}

Class* ClassLoader::ResolveSuperClass(Class* class_ref) {
  if (class_ref->GetName() != "java/lang/Object") {
    return LoadClass(class_ref->GetSuperClassName());
  }
  return nullptr;
}

void ClassLoader::ResolveInterfaces(Class* class_ref, std::vector<Class*>* interfaces) {
  auto interfaceCount = class_ref->GetInterfaceNames()->size();
  if (interfaceCount > 0) {
    interfaces->resize(interfaceCount);
    for (int i = 0; i < interfaceCount; i++) {
      interfaces->at(i) = LoadClass(class_ref->GetInterfaceNames()->at(i));
    }
  }
}

void LinkClass(Class* class_ref) {
  VerifyClass(class_ref);
  PrepareClass(class_ref);
}

void VerifyClass(Class* class_ref) {
  // todo
}

void PrepareClass(Class* class_ref) {
  CalcInstanceFieldSlotIds(class_ref);
  CalcStaticFieldSlotIds(class_ref);
  AllocAndInitStaticVars(class_ref);
}

void CalcInstanceFieldSlotIds(Class* class_ref) {
  uint32_t slot_id = 0;
  if (class_ref->GetSuperClass() != nullptr) {
    slot_id = class_ref->GetSuperClass()->GetInstanceSlotCount();
  }
  for (auto field : *(class_ref->GetFields())) {
    if (!field->IsStatic()) {
      //field->GetSlotId() = slot_id;
      field->SetSlotId(slot_id);
      slot_id++;
      if (field->IsLongOrDouble()) {
        slot_id++;
      }
    }
  }
  class_ref->SetInstanceSlotCount(slot_id);
}

void CalcStaticFieldSlotIds(Class* class_ref) {
  int slot_id = 0;
  for (auto field : *(class_ref->GetFields())) {
    if (field->IsStatic()) {
      //field->GetSlotId() = slot_id;
      field->SetSlotId(slot_id);
      slot_id++;
      if (field->IsLongOrDouble()) {
        slot_id++;
      }
    }
  }
    class_ref->SetStaticSlotCount(slot_id);
}

void AllocAndInitStaticVars(Class* class_ref) {
  auto static_vars = new Slots(class_ref->GetStaticSlotCount());
  class_ref->SetStaticVars(static_vars);
  for (auto field : *(class_ref->GetFields())) {
    if (field->IsStatic() && field->IsFinal()) {
      InitStaticFinalVar(class_ref, field);
    }
  }
}

void InitStaticFinalVar(Class* class_ref, const Field* field) {
  // todo
  auto cp = class_ref->GetConstantPool();
  auto constants = cp->GetConstants();
  auto constant = constants[field->GetConstValueIndex()];
  auto slot_id = field->GetSlotId();
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
        value = dynamic_cast<IntegerConstant*>(constant)->GetValue();
      }

      class_ref->GetStaticVars()->SetInt(field->GetSlotId(), value);
      break;
    }
    case 'F': {
      float value = 0.0f;
      if (nullptr != constant) {
        value = dynamic_cast<FloatConstant*>(constant)->GetValue();
      }
      class_ref->GetStaticVars()->SetFloat(field->GetSlotId(), value);
      break;
    }
    case 'J': {
      int64_t value = 0;
      if (nullptr != constant) {
        value = dynamic_cast<LongConstant*>(constant)->GetValue();
      }
      class_ref->GetStaticVars()->SetLong(field->GetSlotId(), value);
      break;
    }
    case 'D': {
      double value = 0.0;
      if (nullptr != constant) {
        value = dynamic_cast<DoubleConstant*>(constant)->GetValue();
      }
      class_ref->GetStaticVars()->SetDouble(field->GetSlotId(), value);
      break;
    }
    case 'L':
      break;
    case '[':
      //Object* arr = class_ref->NewArray(class_ref->GetLoader(), descriptor);
      //class_ref->static_vars_[slot_id].mRef = std::static_pointer_cast<StringConstant>(constant)->value();
      //class_ref->GetStaticVars()->SetRef(field->GetConstValueIndex(), std::static_pointer_cast<FieldRefConstant>(constant)->value());
      break;
    default:
      break;
  }
  if (descriptor == "L/java/lang/String;") {
    std::string str = dynamic_cast<StringConstant*>(constant)->GetValue();
    auto j_string = Class::NewJString(str);
    class_ref->GetStaticVars()->SetRef(field->GetSlotId(), j_string);
  }
}

ClassLoader* ClassLoader::GetBootClassLoader() {
  if (loaders_.find("") == loaders_.end()) {
    LOG(FATAL) << "Not found boot class loader";
  }
  return loaders_[""];
}



ClassLoader* ClassLoader::GetExtClassLoader() {
  if (loaders_.find("ext") == loaders_.end()) {
    LOG(FATAL) << "Not found ext class loader";
  }
  return loaders_["ext"];
}

ClassLoader* ClassLoader::GetBaseClassLoader() {
  if (loaders_.find("base") == loaders_.end()) {
    LOG(FATAL) << "Not found app base class loader";
  }
  return loaders_["base"];
}

void ClassLoader::RegisterLoader(const std::string& name, ClassLoader* loader) {
  //TODO
}

void ClassLoader::CreateBootClassLoader(std::shared_ptr<classpath::ClassReader> boot_class_reader) {
  std::call_once(boot_class_loader_flag, [&]() {
    if (nullptr == boot_class_reader) {
      LOG(FATAL) << "boot class path is null";
    }
    loaders_[""] = new ClassLoader(boot_class_reader);;
  });
}

void ClassLoader::CreateExtClassLoader(std::shared_ptr<classpath::ClassReader> ext_class_reader) {
  std::call_once(ext_class_loader_flag, [&]() {
    if (nullptr == ext_class_reader) {
      LOG(FATAL) << "class path is null";
    }

    auto ext_class_loader = new ClassLoader(ext_class_reader);
    ext_class_loader->parent_ = GetBootClassLoader();
    loaders_["ext"] = ext_class_loader;
  });
}

void ClassLoader::CreateBaseClassLoader(std::shared_ptr<classpath::ClassReader> app_class_reader) {
  std::call_once(base_class_loader_flag, [&]() {
    if (nullptr == app_class_reader) {
      LOG(FATAL) << "class path is null";
    }

    auto base_class_loader = new ClassLoader(app_class_reader);
    base_class_loader->parent_ = GetExtClassLoader();
    loaders_["base"] = base_class_loader;
  });
}

}// namespace runtime