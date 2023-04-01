#pragma once

#include "class.h"
#include "classpath/class_reader.h"
#include <memory>
#include <string>
#include <unordered_map>
namespace rtda {

struct ClassLoader {
std::shared_ptr<classpath::ClassReader> mClsReader;
std::unordered_map<std::string, std::shared_ptr<Class>> mLoadedClasses;
ClassLoader(std::shared_ptr<classpath::ClassReader> clsReader) : mClsReader(clsReader) {};
std::shared_ptr<Class> loadClass(std::string name);
std::shared_ptr<Class> loadNonArrayClass(std::string name);
std::shared_ptr<Class> defineClass(std::shared_ptr<classpath::ClassData> data);
std::shared_ptr<Class> resolveSuperClass(std::shared_ptr<Class> classPtr);
void resolveInterfaces(std::shared_ptr<Class> classPtr);
};
void linkClass(std::shared_ptr<Class> classPtr);
void verifyClass(std::shared_ptr<Class> classPtr);
void prepareClass(std::shared_ptr<Class> classPtr);
void calcInstanceFieldSlotIds(std::shared_ptr<Class> classPtr);
void calcStaticFieldSlotIds(std::shared_ptr<Class> classPtr);
void allocAndInitStaticVars(std::shared_ptr<Class> classPtr);
void initStaticFinalVar(std::shared_ptr<Class> classPtr, std::shared_ptr<Field> field);
}// namespace rtda