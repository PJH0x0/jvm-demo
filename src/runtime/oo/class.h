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
struct Class : public Object {
public:
  static std::unordered_map<std::string, std::string> primitive_type_map_;
  explicit Class(std::shared_ptr<classfile::ClassFile> classfile);//used for normal class
  explicit Class(std::string name);//used for array class
  Class(){} //used for primitive class
  std::shared_ptr<classfile::ClassFile> GetClassFile() {
    return class_file_;
  }
  std::string GetName() {
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
  void SetName(std::string name) {
      name_ = name;
  }
  std::string GetSuperClassName() {
    return super_class_name_;
  }
  std::string GetPackageName() {
    return package_name_;
  }
  std::shared_ptr<ConstantPool> GetConstantPool() {
    return constant_pool_;
  }
  std::vector<std::shared_ptr<Field>> GetFields() {
    return fields_;
  }
  std::shared_ptr<Field> GetField(std::string name, std::string descriptor, bool is_static);
  std::vector<std::shared_ptr<Method>> getMethods() {
    return methods_;
  }
  std::shared_ptr<Method> GetMethod(std::string name, std::string descriptor, bool is_static);
  std::shared_ptr<Method> GetStaticMethod(std::string name, std::string descriptor) {
    return GetMethod(name, descriptor, true);
  }
  std::shared_ptr<ClassLoader> GetClassLoader() {
    return loader_;
  }
  void SetClassLoader(std::shared_ptr<ClassLoader> loader) {
      loader_ = loader;
  }
  Class* GetSuperClass() {
    return super_class_;
  }
  const std::vector<std::string>& GetInterfaceNames() {
    return interface_names_;
  }
  const std::vector<Class*>& GetInterfaces() {
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
  std::shared_ptr<Slots> GetStaticVars() {
    return static_vars_;
  }
  void SetStaticVars(std::shared_ptr<Slots> vars) {
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
  void StartLoad();
  void StartLoadArrayClass();
  
  std::shared_ptr<Field> LookupField(std::string name, std::string descriptor);
  std::shared_ptr<Method> LookupMethod(std::string name, std::string descriptor);
  std::shared_ptr<Method> LookupMethodInInterfaces(std::string name, std::string descriptor);
  std::shared_ptr<Method> LookupMethodInClass(std::string name, std::string descriptor);
  Object* NewObject();
  
  std::shared_ptr<Method> GetMainMethod();
  std::shared_ptr<Method> GetClinitMethod();
  std::shared_ptr<Method> GetInitMethod();
  Object* NewArray(uint32_t count);
  bool IsArrayClass() {
    return name_[0] == '[';
  }
  Class* GetComponentClass();
  Class* GetArrayClass();
  bool IsPrimitive() {
    return primitive_type_map_.find(name_) != primitive_type_map_.end();
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
  static void InitClass(std::shared_ptr<Thread> thread, Class* klass);
  static void ScheduleClinit(std::shared_ptr<Thread> thread, Class* klass);
  static void InitSuperClass(std::shared_ptr<Thread> thread, Class* klass);
  static Object* NewJString(std::string str);
  static void CreateMethods(Class*, const std::vector<std::shared_ptr<classfile::MemberInfo>>&, std::vector<std::shared_ptr<Method>>&);
  static void CreateFields(Class*, const std::vector<std::shared_ptr<classfile::MemberInfo>>&, std::vector<std::shared_ptr<Field>>&);

private:
    std::shared_ptr<classfile::ClassFile> class_file_;
    bool loaded_;
    bool clinit_started_;
    uint16_t access_flags_;
    std::string name_;
    std::string super_class_name_;
    std::string package_name_;
    std::vector<std::string> interface_names_;
    std::shared_ptr<ConstantPool> constant_pool_;
    std::vector<std::shared_ptr<Field>> fields_;
    std::vector<std::shared_ptr<Method>> methods_;
    std::shared_ptr<ClassLoader> loader_;
    Class* super_class_;
    std::vector<Class*> interfaces_;
    uint32_t instance_slot_count_;
    uint32_t static_slot_count_;
    std::shared_ptr<Slots> static_vars_;
    std::string source_file_;
    //Object* mJClass;
};

}