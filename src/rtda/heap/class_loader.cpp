#include "class_loader.h"
#include "classfile/class_parser.h"

namespace rtda {
std::shared_ptr<Class> ClassLoader::loadClass(std::string name) {
  if (mLoadedClasses.find(name) != mLoadedClasses.end()) {
    return mLoadedClasses[name];
  }
  return loadNonArrayClass(name);
}
std::shared_ptr<Class> ClassLoader::loadNonArrayClass(std::string name) {
  std::shared_ptr<classfile::ClassData> classData = mClsReader->readClass(name);
  return defineClass(classData);  
}
std::shared_ptr<Class> ClassLoader::defineClass(std::shared_ptr<classpath::ClassData> data) {
  std::shared_ptr<Class> classPtr = std::make_shared<Class>(classfile::parse(data));
  classPtr->mSuperClass = resolveSuperClass(classPtr);
  resolveInterfaces(classPtr);
  mLoadedClasses[classPtr->mName] = classPtr;
  return classPtr;
}
std::shared_ptr<Class> ClassLoader::resolveSuperClass(std::shared_ptr<Class> classPtr) {
  if (classPtr->mName != "java/lang/Object") {
    return loadClass(classPtr->mSuperClassName);
  }
  return nullptr;
}
void ClassLoader::resolveInterfaces(std::shared_ptr<Class> classPtr) {
  int interfaceCount = classPtr->mInterfaceNames.size();
  if (interfaceCount > 0) {
    classPtr->mInterfaces.resize(interfaceCount);
    for (int i = 0; i < interfaceCount; i++) {
      classPtr->mInterfaces[i] = loadClass(classPtr->mInterfaceNames[i]);
    }
  }
}
}