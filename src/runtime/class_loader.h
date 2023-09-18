#pragma once

#include <runtime/oo/class.h>
#include <classpath/class_path_parser.h>
#include <memory>
#include <string>
#include <unordered_map>
namespace runtime {

struct ClassLoader {
  private:
  std::shared_ptr<classpath::ClassPathParser> class_path_reader;
  std::unordered_map<std::string, Class*> loaded_classes_;
  static std::unordered_map<std::string, std::shared_ptr<ClassLoader>> loaders_;
  static ClassLoader* boot_class_loader_;
  explicit ClassLoader(std::shared_ptr<classpath::ClassPathParser> class_path_parser)
      : class_path_reader(class_path_parser), loaded_classes_() {};
  public:
  static ClassLoader* GetBootClassLoader(std::shared_ptr<classpath::ClassPathParser> boot_cls_reader);
  static std::shared_ptr<ClassLoader> GetLoader(std::string name);
  static void RegisterLoader(std::string name, std::shared_ptr<ClassLoader> loader);
  
  Class* LoadClass(std::string name);
  Class* LoadNonArrayClass(std::string name);
  Class* LoadArrayClass(std::string name);
  Class* DefineClass(std::shared_ptr<classpath::ClassData> data);
  Class* ResolveSuperClass(Class* class_ptr);
  void ResolveInterfaces(Class* class_ptr, std::vector<Class*>* interfaces);
  void LoadBasicClass();
  void LoadPrimitiveClasses();
  virtual ~ClassLoader() {
    //LOG(FATAL) << "ClassLoader destructor called";
  }
};

void LinkClass(Class* class_ptr);
void VerifyClass(Class* class_ptr);
void PrepareClass(Class* class_ptr);
void CalcInstanceFieldSlotIds(Class* class_ptr);
void CalcStaticFieldSlotIds(Class* class_ptr);
void AllocAndInitStaticVars(Class* class_ptr);
void InitStaticFinalVar(Class* class_ptr, const Field* field);

}// namespace runtime