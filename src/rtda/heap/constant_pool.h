#pragma once


#include <_types/_uint32_t.h>
#include <_types/_uint8_t.h>
#include <memory>
#include <string>
#include <sys/_types/_int32_t.h>
#include <sys/_types/_int64_t.h>
#include <vector>
#include <classfile/constant_pool.h>
namespace rtda {
class Class;
class Method;
class Field;
enum {
  CONSTANT_Utf8 = 1,
  CONSTANT_Integer = 3,
  CONSTANT_Float = 4,
  CONSTANT_Long = 5,
  CONSTANT_Double = 6,
  CONSTANT_Class = 7,
  CONSTANT_String = 8,
  CONSTANT_Fieldref = 9,
  CONSTANT_Methodref = 10,
  CONSTANT_InterfaceMethodref = 11,
  CONSTANT_NameAndType = 12,
  CONSTANT_MethodHandle = 15,
  CONSTANT_MethodType = 16,
  CONSTANT_Dynamic = 17,
  CONSTANT_InvokeDynamic = 18,
  CONSTANT_Module = 19,
  CONSTANT_Package = 20,
};
class Constant {
  protected:
  uint8_t tag;
};
struct ConstantPool {
  std::shared_ptr<Class> mClsPtr;
  std::vector<std::shared_ptr<Constant>> mConstants;
  ConstantPool(std::shared_ptr<Class> clsPtr, std::shared_ptr<classfile::ConstantPool> cfConstantPool);
  std::shared_ptr<Constant> getConstant(uint32_t index);
};

class IntegerConstant : public Constant {
  int32_t mValue;
  public:
  IntegerConstant(std::shared_ptr<classfile::ConstantIntegerInfo> cfConstant) : mValue(cfConstant->value){}
  int32_t value() const {
    return mValue;
  }
};
class FloatConstant : public Constant {
  float mValue;
  public:
  FloatConstant(std::shared_ptr<classfile::ConstantFloatInfo> cfConstant) : mValue(cfConstant->value){}
  float value() const {
    return mValue;
  }
};
class LongConstant : public Constant {
  int64_t mValue;
  public:
  LongConstant(std::shared_ptr<classfile::ConstantLongInfo> cfConstant) : mValue(cfConstant->value){}
  int64_t value() const {
    return mValue;
  }
};
class DoubleConstant : public Constant {
  double mValue;
  public:
  DoubleConstant(std::shared_ptr<classfile::ConstantDoubleInfo> cfConstant) : mValue(cfConstant->value){}
  double value() const {
    return mValue;
  }
};
class StringConstant : public Constant {
  std::string mString;
  public:
  StringConstant(std::string str) : mString(str){}
  const char* value() const {
    return mString.c_str();
  }
};
class SymRefConstant : public Constant {
  std::shared_ptr<ConstantPool> mConstantPool;
  std::string mClassName;
  static const std::shared_ptr<Class> mClsPtr;
  public:
  SymRefConstant(std::shared_ptr<ConstantPool> constantPool, std::string className) : mConstantPool(constantPool), mClassName(className) {}
};
class ClassRefConstant : public SymRefConstant {
  public:
  ClassRefConstant(std::shared_ptr<ConstantPool> constantPool, std::string className) : SymRefConstant(constantPool, className) {}
};
class MemberRefConstant : public SymRefConstant {
  std::string mName;
  std::string mDescriptor;
  public:
  MemberRefConstant(std::shared_ptr<ConstantPool> constantPool, 
    std::string className, std::string name, std::string descriptor) : SymRefConstant(constantPool, className), mName(name), mDescriptor(descriptor) {}
};
class FieldRefConstant : public MemberRefConstant {
  std::shared_ptr<Field> mFieldPtr;
  public:
  FieldRefConstant(std::shared_ptr<ConstantPool> constantPool, 
    std::string className, std::string name, std::string descriptor)
    : MemberRefConstant(constantPool, className, name, descriptor), mFieldPtr(nullptr) {}
};
class MethodRefConstant : public MemberRefConstant {
  std::shared_ptr<Method> mMethodPtr;
  public:
  MethodRefConstant(std::shared_ptr<ConstantPool> constantPool, 
    std::string className, std::string name, std::string descriptor) 
    : MemberRefConstant(constantPool, className, name, descriptor), mMethodPtr(nullptr) {}
};
class InterfaceMethodRefConstant : public MemberRefConstant {
  std::shared_ptr<Method> mMethodPtr;
  public:
  InterfaceMethodRefConstant(std::shared_ptr<ConstantPool> constantPool, 
    std::string className, std::string name, std::string descriptor)
    : MemberRefConstant(constantPool, className, name, descriptor), mMethodPtr(nullptr) {}
};
// class NameAndTypeConstant : public Constant {
//   std::shared_ptr<Constant> mName;
//   std::shared_ptr<Constant> mDescriptor;
//   public:
//   NameAndTypeConstant(std::shared_ptr<classfile::ConstantNameAndTypeInfo> cfConstant);
// };
// class MethodHandleConstant : public Constant {
//   uint8_t mReferenceKind;
//   std::shared_ptr<Constant> mReference;
//   public:
//   MethodHandleConstant(std::shared_ptr<classfile::ConstantMethodHandleInfo> cfConstant);
// };
// class MethodTypeConstant : public Constant {
//   std::shared_ptr<Constant> mDescriptor;
//   public:
//   MethodTypeConstant(std::shared_ptr<classfile::ConstantMethodTypeInfo> cfConstant);
// };
// class InvokeDynamicConstant : public Constant {
//   uint16_t mBootstrapMethodAttrIndex;
//   std::shared_ptr<Constant> mNameAndType;
//   public:
//   InvokeDynamicConstant(std::shared_ptr<classfile::ConstantInvokeDynamicInfo> cfConstant);
// };

}