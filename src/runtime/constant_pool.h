#pragma once


#include <stdint.h>
#include <codecvt>
#include <locale>
#include <memory>
#include <string>
#include <vector>
#include <classfile/constant_pool.h>
namespace runtime {
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
  private:
  uint8_t tag_;
  public:
  explicit Constant(uint8_t tag) : tag_(tag){}
  uint8_t Tag() const {
    return tag_;
  }
  virtual ~Constant() = default;
};
class ConstantPool {
  private:
  Class* class_ptr_;
  std::vector<std::shared_ptr<Constant>> constants_;
  public:
  ConstantPool(Class* class_ptr, std::shared_ptr<classfile::ConstantPool> cf_constant_pool);
  std::shared_ptr<Constant> GetConstant(uint32_t index);
  Class* GetClass() {
    return class_ptr_;
  }
  const std::vector<std::shared_ptr<Constant>>& GetConstants() {
    return constants_;
  }
};

struct IntegerConstant : public Constant {
  private:
  int32_t value_;
  public:
  explicit IntegerConstant(std::shared_ptr<classfile::ConstantIntegerInfo> constant_integer_info)
    : value_(constant_integer_info->GetValue()), Constant(CONSTANT_Integer){}
  int32_t value() const {
    return value_;
  }
};
struct FloatConstant : public Constant {
  private:
  float value_;
  public:
  explicit FloatConstant(std::shared_ptr<classfile::ConstantFloatInfo> constant_float_info)
    : value_(constant_float_info->GetValue()), Constant(CONSTANT_Float){}
  float value() const {
    return value_;
  }
};
struct LongConstant : public Constant {
  private:
  int64_t value_;
  public:
  explicit LongConstant(std::shared_ptr<classfile::ConstantLongInfo> constant_long_info)
    : value_(constant_long_info->GetValue()), Constant(CONSTANT_Long){}
  int64_t value() const {
    return value_;
  }
};
struct DoubleConstant : public Constant {
  private:
  double value_;
  public:
  DoubleConstant(std::shared_ptr<classfile::ConstantDoubleInfo> constant_double_info)
    : value_(constant_double_info->GetValue()), Constant(CONSTANT_Double){}
  double value() const {
    return value_;
  }
};
struct StringConstant : public Constant {
  private:
  std::string mString;
  public:
  StringConstant(std::string str) : mString(str), Constant(CONSTANT_String){}
  static std::u16string DecodeMutf8(const char* bytes, int len);
  const std::string& value() {
    return mString;
  }
  static std::u16string utf8ToUtf16(std::string str) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    //std::u16string u16str = convert.from_bytes(DecodeMutf8(str.c_str(), str.size()));
    std::u16string u16str = DecodeMutf8(str.c_str(), str.size());
    //LOG(INFO) << "utf8ToUtf16: " << u16str.size();
    return u16str;
  }
  static std::string utf16ToUtf8(const char16_t* str) {
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> convert;
    return convert.to_bytes(str);
  }
};
struct SymRefConstant : public Constant {
  private:
  std::shared_ptr<ConstantPool> constant_pool_;
  std::string class_name_;
  Class* class_ptr_;
  public:
  SymRefConstant(std::shared_ptr<ConstantPool> constant_pool, std::string class_name, uint8_t tag)
    : constant_pool_(constant_pool), class_name_(class_name), Constant(tag), class_ptr_(nullptr) {}
  Class* ResolveClass();
  std::string GetClassName() const {
    return class_name_;
  }
  Class* GetClass() const {
    return class_ptr_;
  }
  std::shared_ptr<ConstantPool> GetConstantPool() const {
    return constant_pool_;
  }

};
struct ClassRefConstant : public SymRefConstant {
  public:
  ClassRefConstant(std::shared_ptr<ConstantPool> constant_pool, std::string class_name)
    : SymRefConstant(constant_pool, class_name, CONSTANT_Class) {}
};
struct MemberRefConstant : public SymRefConstant {
  private:
  std::string name_;
  std::string descriptor_;
  public:
  MemberRefConstant(std::shared_ptr<ConstantPool> constant_pool,
    std::string className, std::string name, std::string descriptor, uint8_t tag) 
      : SymRefConstant(constant_pool, className, tag), name_(name), descriptor_(descriptor) {}
  std::string GetName() const {
    return name_;
  }
  std::string GetDescriptor() const {
    return descriptor_;
  }
};
struct FieldRefConstant : public MemberRefConstant {
  private:
  std::shared_ptr<Field> field_;
  public:
  FieldRefConstant(std::shared_ptr<ConstantPool> constant_pool,
    std::string className, std::string name, std::string descriptor)
    : MemberRefConstant(constant_pool, className, name, descriptor, CONSTANT_Fieldref), field_(nullptr) {}
  std::shared_ptr<Field> ResolveField();
};
struct MethodRefConstant : public MemberRefConstant {
  private:
  std::shared_ptr<Method> method_;
  public:
  MethodRefConstant(std::shared_ptr<ConstantPool> constant_pool,
    std::string className, std::string name, std::string descriptor) 
    : MemberRefConstant(constant_pool, className, name, descriptor, CONSTANT_Methodref), method_(nullptr) {}
  std::shared_ptr<Method> ResolveMethod();
};
class InterfaceMethodRefConstant : public MemberRefConstant {
  private:
  std::shared_ptr<Method> interface_method_;
  public:
  InterfaceMethodRefConstant(std::shared_ptr<ConstantPool> constant_pool,
    std::string className, std::string name, std::string descriptor)
    : MemberRefConstant(constant_pool, className, name, descriptor, CONSTANT_InterfaceMethodref), interface_method_(nullptr) {}
  std::shared_ptr<Method> ResolveInterfaceMethod();
};

// class NameAndTypeConstant : public Constant {
//   std::shared_ptr<Constant> name_;
//   std::shared_ptr<Constant> descriptor_;
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
//   std::shared_ptr<Constant> descriptor_;
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