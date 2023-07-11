#pragma once

#include <runtime/oo/class.h>
#include <classpath/class_path_parser.h>
#include <memory>
#include <string>
#include <unordered_map>
namespace runtime {

struct ClassLoader {
  private:
  std::shared_ptr<classpath::ClassPathParser> mClsReader;
  std::unordered_map<std::string, Class*> mLoadedClasses;
  static std::unordered_map<std::string, std::shared_ptr<ClassLoader>> mLoaders;
  static std::shared_ptr<ClassLoader> mBootClassLoader;
  ClassLoader(std::shared_ptr<classpath::ClassPathParser> clsReader) : mClsReader(clsReader), mLoadedClasses() {};
  public:
  static std::shared_ptr<ClassLoader> getBootClassLoader(std::shared_ptr<classpath::ClassPathParser>);
  static std::shared_ptr<ClassLoader> getLoader(std::string name);
  static void registerLoader(std::string name, std::shared_ptr<ClassLoader> loader);
  
  Class* loadClass(std::string name);
  Class* loadNonArrayClass(std::string name);
  Class* loadArrayClass(std::string name);
  Class* defineClass(std::shared_ptr<classpath::ClassData> data);
  Class* resolveSuperClass(Class* classPtr);
  void resolveInterfaces(Class*, std::vector<Class*>&);
  void loadBasicClass();
  void loadPrimitiveClasses();
  virtual ~ClassLoader() {
    //LOG(FATAL) << "ClassLoader destructor called";
  }
};

void linkClass(Class* classPtr);
void verifyClass(Class* classPtr);
void prepareClass(Class* classPtr);
void calcInstanceFieldSlotIds(Class* classPtr);
void calcStaticFieldSlotIds(Class* classPtr);
void allocAndInitStaticVars(Class* classPtr);
void initStaticFinalVar(Class* classPtr, std::shared_ptr<Field> field);

}// namespace runtime