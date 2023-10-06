#pragma once

#include <runtime/oo/class.h>
#include <classpath/class_path_parser.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
namespace runtime {

class ClassLoader {
  using ClassLoaderMaps = std::unordered_map<std::string, ClassLoader*>;
  using ClassCache = std::unordered_map<std::string, Class*>;
private:
  static const char* kJlcClassString;
  //static constexpr std::string jcl_class_string_ = "";
  std::shared_ptr<classpath::ClassReader> class_path_reader_;
  ClassCache loaded_classes_;
  ClassLoader* parent_{};
  static ClassLoaderMaps loaders_;
  explicit ClassLoader(std::shared_ptr<classpath::ClassReader> class_path_reader)
      : class_path_reader_(std::move(class_path_reader)), loaded_classes_() {};
public:
  static void CreateBootClassLoader(std::shared_ptr<classpath::ClassReader> boot_class_reader);
  static ClassLoader* GetBootClassLoader();
  static void CreateExtClassLoader(std::shared_ptr<classpath::ClassReader> ext_class_reader);
  static ClassLoader* GetExtClassLoader();
  static void CreateBaseClassLoader(std::shared_ptr<classpath::ClassReader> app_class_reader);
  static ClassLoader* GetBaseClassLoader();
  static void RegisterLoader(const std::string& name, ClassLoader* loader);

  //Temp get function for test
  ClassCache& GetLoadedClasses() {
    return loaded_classes_;
  }
  
  Class* LoadClass(const std::string& name);
  Class* LoadNonArrayClass(const std::string& name);
  Class* LoadArrayClass(const std::string& name);
  Class* DefineClass(const std::shared_ptr<classpath::ClassData>& data);
  Class* ResolveSuperClass(Class* class_ref);
  void ResolveInterfaces(Class* class_ref, std::vector<Class*>* interfaces);
  void LoadBasicClass();
  void LoadPrimitiveClasses();
  virtual ~ClassLoader() {
    //LOG(FATAL) << "ClassLoader destructor called";
  }
};

void LinkClass(Class* class_ref);
void VerifyClass(Class* class_ptr);
void PrepareClass(Class* class_ref);
void CalcInstanceFieldSlotIds(Class* class_ref);
void CalcStaticFieldSlotIds(Class* class_ref);
void AllocAndInitStaticVars(Class* class_ref);
void InitStaticFinalVar(Class* class_ref, const Field* field);

}// namespace runtime