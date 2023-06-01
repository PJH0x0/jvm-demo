#pragma once

#include "rtda/thread.h"
#include <memory>
#include <string>
#include <vector>
#include <classfile/class_parser.h>
#include <rtda/local_vars.h>
#include <unordered_map>
#include <algorithm>

namespace rtda {
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
class Object;
class Thread;
struct Class {
  private:
  std::shared_ptr<classfile::ClassFile> mClassfile;
  bool mLoaded;
  bool mClinitStarted;
  uint16_t mAccessFlags;
  std::string mName;
  std::string mSuperClassName;
  std::string mPackageName;
  std::vector<std::string> mInterfaceNames;
  std::shared_ptr<ConstantPool> mConstantPool;
  std::vector<std::shared_ptr<Field>> mFields;
  std::vector<std::shared_ptr<Method>> mMethods;
  std::shared_ptr<ClassLoader> mLoader;
  std::shared_ptr<Class> mSuperClass;
  std::vector<std::shared_ptr<Class>> mInterfaces;
  uint32_t mInstanceSlotCount;
  uint32_t mStaticSlotCount;
  std::shared_ptr<Slots> mStaticVars;
  Object* mJClass;
  public:
  static std::unordered_map<std::string, std::string> mPrimitiveTypes;
  Class(std::shared_ptr<classfile::ClassFile> classfile);//used for normal class
  Class(std::string name);//used for array class
  Class(){} //used for primitive class
  std::shared_ptr<classfile::ClassFile> getClassFile() {
    return mClassfile;
  }
  std::string getName() {
    return mName;
  }
  std::string getJavaName() {
    //return replace_all(mName, "/", ".");
    std::string name = mName;
    std::replace(name.begin(), name.end(), '/', '.');
    return name;
  }
  void setName(std::string name) {
    mName = name;
  }
  std::string getSuperClassName() {
    return mSuperClassName;
  }
  std::string getPackageName() {
    return mPackageName;
  }
  std::shared_ptr<ConstantPool> getConstantPool() {
    return mConstantPool;
  }
  std::vector<std::shared_ptr<Field>> getFields() {
    return mFields;
  }
  std::shared_ptr<Field> getField(std::string name, std::string descriptor, bool isStatic);
  std::vector<std::shared_ptr<Method>> getMethods() {
    return mMethods;
  }
  std::shared_ptr<ClassLoader> getClassLoader() {
    return mLoader;
  }
  void setClassLoader(std::shared_ptr<ClassLoader> loader) {
    mLoader = loader;
  }
  std::shared_ptr<Class> getSuperClass() {
    return mSuperClass;
  }
  const std::vector<std::string>& getInterfaceNames() {
    return mInterfaceNames;
  }
  const std::vector<std::shared_ptr<Class>>& getInterfaces() {
    return mInterfaces;
  }
  uint32_t getInstanceSlotCount() {
    return mInstanceSlotCount;
  }
  void setInstanceSlotCount(uint32_t count) {
    mInstanceSlotCount = count;
  }
  uint32_t getStaticSlotCount() {
    return mStaticSlotCount;
  }
  void setStaticSlotCount(uint32_t count) {
    mStaticSlotCount = count;
  }
  std::shared_ptr<Slots> getStaticVars() {
    return mStaticVars;
  }
  void setStaticVars(std::shared_ptr<Slots> vars) {
    mStaticVars = vars;
  }

  void setAccessFlags(ACCESS_FLAG flags) {
    mAccessFlags = flags;
  }
  
  bool isPublic() {
    return (mAccessFlags & ACC_PUBLIC) != 0;
  }
  bool isFinal() {
    return (mAccessFlags & ACC_FINAL) != 0;
  }
  bool isSuper() {
    return (mAccessFlags & ACC_SUPER) != 0;
  }
  bool isInterface() {
    return (mAccessFlags & ACC_INTERFACE) != 0;
  }
  bool isAbstract() {
    return (mAccessFlags & ACC_ABSTRACT) != 0;
  }
  bool isSythetic() {
    return (mAccessFlags & ACC_SYNTHETIC) != 0;
  }
  bool isAnnotation() {
    return (mAccessFlags & ACC_ANNOTATION) != 0;
  }
  bool isEnum() {
    return (mAccessFlags & ACC_ENUM) != 0;
  }
  bool isAccessibleTo(std::shared_ptr<Class> other) {
    return isPublic() || mPackageName == other->mPackageName;
  }
  void startClinit() {
    mClinitStarted = true;
  }
  bool isClinitStarted() {
    return mClinitStarted;
  }
  void startLoad();
  void startLoadArrayClass();
  
  std::shared_ptr<Field> lookupField(std::string name, std::string descriptor);
  std::shared_ptr<Method> lookupMethod(std::string name, std::string descriptor);
  std::shared_ptr<Method> lookupMethodInInterfaces(std::string name, std::string descriptor);
  std::shared_ptr<Method> lookupMethodInClass(std::string name, std::string descriptor);
  Object* newObject();
  
  std::shared_ptr<Method> getMainMethod();
  std::shared_ptr<Method> getClinitMethod();
  std::shared_ptr<Method> getInitMethod();
  std::shared_ptr<Method> getStaticMethod(std::string name, std::string descriptor);
  Object* newArray(uint32_t count);
  bool isArrayClass() {
    return mName[0] == '[';
  }
  std::shared_ptr<Class> getComponentClass();
  std::shared_ptr<Class> getArrayClass();
  bool isPrimitive() {
    return mPrimitiveTypes.find(mName) != mPrimitiveTypes.end();
  }

  void setJClass(Object* jClass) {
    mJClass = jClass;
  }
  Object* getJClass() {
    return mJClass;
  }

  static std::string replace_all(std::string str, const std::string& from, const std::string& to);

  static std::shared_ptr<Class> getPrimitiveArrayClass(uint8_t);
  //Check whether s is a subclass of t, class s extends class t
  static bool isSubClassOf(std::shared_ptr<Class> s, std::shared_ptr<Class> t); 
  //Check whether s is a superclass of t, class t extends class s
  static bool isSuperClassOf(std::shared_ptr<Class> s, std::shared_ptr<Class> t);
  //Check whether s can assign from t, s = t
  static bool isAssignableFrom(std::shared_ptr<Class> s, std::shared_ptr<Class> t);
  //Check whether s implements t
  static bool isImplements(std::shared_ptr<Class> s, std::shared_ptr<Class> t);
  //Check whether s is a subinterface of t, interface s extends interface t
  static bool isSubInterfaceOf(std::shared_ptr<Class> s, std::shared_ptr<Class> t);
  //Check whether s is a superinterface of t, interface t extends interface s
  static bool isSuperInterfaceOf(std::shared_ptr<Class> s, std::shared_ptr<Class> t);
  static bool isJlObject(std::shared_ptr<Class> c);
  static bool isJlCloneable(std::shared_ptr<Class> c);
  static bool isJioSerializable(std::shared_ptr<Class> c);
  static std::string getArrayClassName(std::string);
  static std::string toDescriptor(std::string);
  static std::string toClassName(std::string);
  static std::string getComponentClassName(std::string);
  static void initClass(std::shared_ptr<Thread> thread, std::shared_ptr<Class> klass);
  static void scheduleClinit(std::shared_ptr<Thread> thread, std::shared_ptr<Class> klass);
  static void initSuperClass(std::shared_ptr<Thread> thread, std::shared_ptr<Class> klass);
  static Object* newJString(std::string str);
  static void createMethods(std::shared_ptr<Class>, std::vector<std::shared_ptr<classfile::MemberInfo>>&, std::vector<std::shared_ptr<Method>>&);
  static void createFields(std::shared_ptr<Class>, std::vector<std::shared_ptr<classfile::MemberInfo>>&, std::vector<std::shared_ptr<Field>>&);
  
};

}