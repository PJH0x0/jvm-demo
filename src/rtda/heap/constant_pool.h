#pragma once


#include <stdint.h>
#include <codecvt>
#include <locale>
#include <memory>
#include <string>
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
struct Constant {
  private:
  uint8_t mTag;
  public:
  Constant(uint8_t tag) : mTag(tag){}
  uint8_t tag() const {
    return mTag;
  }
  virtual ~Constant() {}
};
struct ConstantPool {
  private:
  std::shared_ptr<Class> mClsPtr;
  std::vector<std::shared_ptr<Constant>> mConstants;
  public:
  ConstantPool(std::shared_ptr<Class> clsPtr, std::shared_ptr<classfile::ConstantPool> cfConstantPool);
  std::shared_ptr<Constant> getConstant(uint32_t index);
  std::shared_ptr<Class> getClass() {
    return mClsPtr;
  }
  const std::vector<std::shared_ptr<Constant>>& constants() {
    return mConstants;
  }
};

struct IntegerConstant : public Constant {
  private:
  int32_t mValue;
  public:
  IntegerConstant(std::shared_ptr<classfile::ConstantIntegerInfo> cfConstant) 
    : mValue(cfConstant->value), Constant(CONSTANT_Integer){}
  int32_t value() const {
    return mValue;
  }
};
struct FloatConstant : public Constant {
  private:
  float mValue;
  public:
  FloatConstant(std::shared_ptr<classfile::ConstantFloatInfo> cfConstant) 
    : mValue(cfConstant->value), Constant(CONSTANT_Float){}
  float value() const {
    return mValue;
  }
};
struct LongConstant : public Constant {
  private:
  int64_t mValue;
  public:
  LongConstant(std::shared_ptr<classfile::ConstantLongInfo> cfConstant) 
    : mValue(cfConstant->value), Constant(CONSTANT_Long){}
  int64_t value() const {
    return mValue;
  }
};
struct DoubleConstant : public Constant {
  private:
  double mValue;
  public:
  DoubleConstant(std::shared_ptr<classfile::ConstantDoubleInfo> cfConstant) 
    : mValue(cfConstant->value), Constant(CONSTANT_Double){}
  double value() const {
    return mValue;
  }
};
struct StringConstant : public Constant {
  private:
  std::string mString;
  public:
  StringConstant(std::string str) : mString(str), Constant(CONSTANT_String){}
  const std::string& value() {
    return mString;
  }
  static std::u16string utf8ToUtf16(std::string str) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.from_bytes(str);
  }
  static std::string utf16ToUtf8(const char16_t* str) {
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> convert;
    return convert.to_bytes(str);
  }
};
struct SymRefConstant : public Constant {
  private:
  std::shared_ptr<ConstantPool> mConstantPool;
  std::string mClassName;
  std::shared_ptr<Class> mClsPtr;
  public:
  SymRefConstant(std::shared_ptr<ConstantPool> constantPool, std::string className, uint8_t tag) 
    : mConstantPool(constantPool), mClassName(className), Constant(tag) {}
  std::shared_ptr<Class> resolveClass();
  std::string className() const {
    return mClassName;
  }
  std::shared_ptr<Class> getClass() const {
    return mClsPtr;
  }
  std::shared_ptr<ConstantPool> constantPool() const {
    return mConstantPool;
  }

};
struct ClassRefConstant : public SymRefConstant {
  public:
  ClassRefConstant(std::shared_ptr<ConstantPool> constantPool, std::string className) 
    : SymRefConstant(constantPool, className, CONSTANT_Class) {}
};
struct MemberRefConstant : public SymRefConstant {
  private:
  std::string mName;
  std::string mDescriptor;
  public:
  MemberRefConstant(std::shared_ptr<ConstantPool> constantPool, 
    std::string className, std::string name, std::string descriptor, uint8_t tag) 
      : SymRefConstant(constantPool, className, tag), mName(name), mDescriptor(descriptor) {}
  std::string name() const {
    return mName;
  }
  std::string descriptor() const {
    return mDescriptor;
  }
};
struct FieldRefConstant : public MemberRefConstant {
  private:
  std::shared_ptr<Field> mFieldPtr;
  public:
  FieldRefConstant(std::shared_ptr<ConstantPool> constantPool, 
    std::string className, std::string name, std::string descriptor)
    : MemberRefConstant(constantPool, className, name, descriptor, CONSTANT_Fieldref), mFieldPtr(nullptr) {}
  std::shared_ptr<Field> resolveField();
};
struct MethodRefConstant : public MemberRefConstant {
  private:
  std::shared_ptr<Method> mMethodPtr;
  public:
  MethodRefConstant(std::shared_ptr<ConstantPool> constantPool, 
    std::string className, std::string name, std::string descriptor) 
    : MemberRefConstant(constantPool, className, name, descriptor, CONSTANT_Methodref), mMethodPtr(nullptr) {}
  std::shared_ptr<Method> resolveMethod();
};
class InterfaceMethodRefConstant : public MemberRefConstant {
  private:
  std::shared_ptr<Method> mMethodPtr;
  public:
  InterfaceMethodRefConstant(std::shared_ptr<ConstantPool> constantPool, 
    std::string className, std::string name, std::string descriptor)
    : MemberRefConstant(constantPool, className, name, descriptor, CONSTANT_InterfaceMethodref), mMethodPtr(nullptr) {}
  std::shared_ptr<Method> resolveInterfaceMethod();
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