#pragma once

#include "runtime/thread.h"
#include <memory>
#include <string>
#include <vector>
#include <classfile/class_parser.h>
#include <runtime/local_vars.h>
#include <unordered_map>
#include <algorithm>
#include "object.h"

namespace runtime {
enum ACCESS_FLAG {
  ACC_PUBLIC       = 0x0001, // class field method
	ACC_PRIVATE      = 0x0002, //       field method
	ACC_PROTECTED    = 0x0004, //       field method
	ACC_STATIC       = 0x0008, //       field method
	ACC_FINAL        = 0x0010, // class field method
	ACC_SUPER        = 0x0020, // class
	ACC_SYNCHRONIZED = 0x0020, //             method
	ACC_VOLATILE     = 0x0040, //       field
	ACC_BRIDGE       = 0x0040, //             method
	ACC_TRANSIENT    = 0x0080, //       field
	ACC_VARARGS      = 0x0080, //             method
	ACC_NATIVE       = 0x0100, //             method
	ACC_INTERFACE    = 0x0200, // class
	ACC_ABSTRACT     = 0x0400, // class       method
	ACC_STRICT       = 0x0800, //             method
	ACC_SYNTHETIC    = 0x1000, // class field method
	ACC_ANNOTATION   = 0x2000, // class
	ACC_ENUM         = 0x4000, // class field
};

class ClassLoader;
class ConstantPool;
class Field;
class Method;
class Thread;
class Class final : public Object {

public:
  using PrimitiveTypes = std::unordered_map<std::string, std::string>;
  Class();//used for normal and primitive class
  //explicit Class(std::string name);//used for array class
  void Init();
  //Class(){} //used for primitive class

  void SetName(const std::string& name) {
    name_ = name;
  }
  std::string GetName() const {
    return name_;
  }
  
  uint32_t ObjectSize() const {
    return instance_slot_count_ * sizeof(Slot);
  }
  std::string GetJavaName() {
    //return replace_all(name_, "/", ".");
    std::string name = name_;
    std::replace(name.begin(), name.end(), '/', '.');
    return name;
  }
  std::string GetSourceFile() {
    return source_file_;
  }

  std::string GetSuperClassName() {
    return super_class_name_;
  }
  std::string GetPackageName() {
    return package_name_;
  }
  const ConstantPool* GetConstantPool() const {
    return constant_pool_;
  }
  const std::vector<Field*>* GetFields() const {
    return fields_;
  }
  const Field* GetField(std::string name, std::string descriptor, bool is_static);
  const std::vector<Method*>* GetMethods() const {
    return methods_;
  }
  const Method* GetMethod(const std::string& name, const std::string& descriptor, bool is_static);
  const Method* GetStaticMethod(const std::string& name, const std::string& descriptor) {
    return GetMethod(name, descriptor, true);
  }
  ClassLoader* GetClassLoader() const {
    return loader_;
  }
  void SetClassLoader(ClassLoader* loader) {
      loader_ = loader;
  }
  Class* GetSuperClass() const {
    return super_class_;
  }
  const std::vector<std::string>* GetInterfaceNames() const {
    return interface_names_;
  }
  const std::vector<Class*>* GetInterfaces() const {
    return interfaces_;
  }
  uint32_t GetInstanceSlotCount() const {
    return instance_slot_count_;
  }
  void SetInstanceSlotCount(uint32_t count) {
      instance_slot_count_ = count;
  }
  uint32_t GetStaticSlotCount() const {
    return static_slot_count_;
  }
  void SetStaticSlotCount(uint32_t count) {
      static_slot_count_ = count;
  }
  Slots* GetStaticVars() {
    return static_vars_;
  }
  void SetStaticVars(Slots* vars) {
      static_vars_ = vars;
  }

  void SetAccessFlags(ACCESS_FLAG flags) {
      access_flags_ = flags;
  }
  
  bool IsPublic() const {
    return (access_flags_ & ACC_PUBLIC) != 0;
  }
  bool IsFinal() const {
    return (access_flags_ & ACC_FINAL) != 0;
  }
  bool IsSuper() const {
    return (access_flags_ & ACC_SUPER) != 0;
  }
  bool IsInterface() const {
    return (access_flags_ & ACC_INTERFACE) != 0;
  }
  bool IsAbstract() const {
    return (access_flags_ & ACC_ABSTRACT) != 0;
  }
  bool IsSynthetic() const {
    return (access_flags_ & ACC_SYNTHETIC) != 0;
  }
  bool IsAnnotation() const {
    return (access_flags_ & ACC_ANNOTATION) != 0;
  }
  bool IsEnum() const {
    return (access_flags_ & ACC_ENUM) != 0;
  }
  bool IsAccessibleTo(Class* other) {
    return IsPublic() || package_name_ == other->package_name_;
  }
  void StartClinit() {
      clinit_started_ = true;
  }
  bool IsClinitStarted() const {
    return clinit_started_;
  }
  void StartLoad(const std::shared_ptr<const classfile::ClassFile>& class_file);
  void StartLoadArrayClass();
  
  const Field* LookupField(std::string name, std::string descriptor);
  const Method* LookupMethod(const std::string& name, const std::string& descriptor);
  const Method* LookupMethodInInterfaces(const std::string& name, const std::string& descriptor);
  const Method* LookupMethodInClass(const std::string& name, const std::string& descriptor);
  Object* NewObject();
  
  const Method* GetMainMethod();
  const Method* GetClinitMethod();
  //const Method* GetInitMethod();
  Object* NewArray(uint32_t count);
  bool IsArrayClass() {
    return name_[0] == '[';
  }
  Class* GetComponentClass();
  Class* GetArrayClass();
  bool IsPrimitive() {
    return primitive_types_.find(name_) != primitive_types_.end();
  }


  static Class* GetPrimitiveArrayClass(uint8_t);
  //Check whether s is a subclass of t, class s extends class t
  static bool IsSubClassOf(Class* s, Class* t);
  //Check whether s is a superclass of t, class t extends class s
  static bool IsSuperClassOf(Class* s, Class* t);
  //Check whether s can assign from t, s = t
  static bool IsAssignableFrom(Class* s, Class* t);
  //Check whether s implements t
  static bool IsImplements(Class* s, Class* t);
  //Check whether s is a subinterface of t, interface s extends interface t
  static bool IsSubInterfaceOf(Class* s, Class* t);
  //Check whether s is a superinterface of t, interface t extends interface s
  static bool IsSuperInterfaceOf(Class* s, Class* t);
  static bool IsJlObject(Class* c);
  static bool IsJlCloneable(Class* c);
  static bool IsJioSerializable(Class* c);
  static std::string GetArrayClassName(std::string);
  static std::string ToDescriptor(std::string);
  static std::string ToClassName(std::string);
  static std::string GetComponentClassName(std::string);
  static void InitClass(Class* klass);
  static void ScheduleClinit(Class* klass);
  static void InitSuperClass(Class* klass);
  static Object* NewJString(std::string str);
  static Class* NewClassObject();
  static void CreateMethods(Class*, const std::vector<std::shared_ptr<classfile::MemberInfo>>&, std::vector<Method*>*);
  static void CreateFields(Class*, const std::vector<std::shared_ptr<classfile::MemberInfo>>&, std::vector<Field*>*);
  static const PrimitiveTypes& GetPrimitiveTypes();

private:
    //const classfile::ClassFile* class_file_;
    bool loaded_{};
    bool clinit_started_{};
    uint16_t access_flags_{0};
    std::string name_{};
    std::string super_class_name_{};
    std::string package_name_{};
    std::vector<std::string>* interface_names_{};
    const ConstantPool* constant_pool_{};
    std::vector<Field*>* fields_{};
    std::vector<Method*>* methods_{};
    ClassLoader* loader_{};
    Class* super_class_{};
    std::vector<Class*>* interfaces_{};
    uint32_t instance_slot_count_{0};
    uint32_t static_slot_count_{0};
    Slots* static_vars_{nullptr};
    std::string source_file_{};
    static PrimitiveTypes primitive_types_;
    //Object* mJClass;
};

}