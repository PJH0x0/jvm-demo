#include "include/ConstantPool.h"
namespace JVM {
  std::shared_ptr<ConstantInfo> createConstantInfo(u1 tag) {
    switch (tag) {
      case CONSTANT_Utf8: return std::make_shared<ConstantUtf8Info>();
      case CONSTANT_Integer: return std::make_shared<ConstantIntegerInfo>();
      case CONSTANT_Float: return std::make_shared<ConstantFloatInfo>();
      case CONSTANT_Long: return std::make_shared<ConstantLongInfo>();
      case CONSTANT_Double: return std::make_shared<ConstantDoubleInfo>();
      case CONSTANT_Class: return std::make_shared<ConstantClassInfo>();
      case CONSTANT_String: return std::make_shared<ConstantStringInfo>();
      case CONSTANT_Fieldref: return std::make_shared<ConstantFieldrefInfo>();
      case CONSTANT_Methodref: return std::make_shared<ConstantMethodrefInfo>();
      case CONSTANT_InterfaceMethodref: return std::make_shared<ConstantInterfaceMethodrefInfo>();
      case CONSTANT_NameAndType: return std::make_shared<ConstantNameAndTypeInfo>();
      case CONSTANT_MethodHandle: return std::make_shared<ConstantMethodHandleInfo>();
      case CONSTANT_MethodType: return std::make_shared<ConstantMethodTypeInfo>();
      case CONSTANT_Dynamic: return std::make_shared<ConstantDynamicInfo>();
      case CONSTANT_InvokeDynamic: return std::make_shared<ConstantInvokeDynamicInfo>();
      case CONSTANT_Module: return std::make_shared<ConstantModuleInfo>();
      case CONSTANT_Package: return std::make_shared<ConstantPackageInfo>();
      default: break;
    }
    LOG(FATAL) << "java.lang.ClassFormatError: constant pool tag "<< tag;
  }
  std::shared_ptr<ConstantInfo> parseConstantInfo(std::shared_ptr<ClassData> classData, int& pos) {
    u1 tag = 0;
    parseUint(classData, pos, tag);
    LOG(INFO) << "Constant info tag = " << (int)tag;
    std::shared_ptr<ConstantInfo> constantInfo = createConstantInfo(tag);
    constantInfo->tag = tag;
    constantInfo->parseConstantInfo(classData, pos);
    return constantInfo;
  }
}

