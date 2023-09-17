#pragma once


#include <stdint.h>
#include <codecvt>
#include <locale>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <classfile/constant_pool.h>
namespace runtime {
class Class;
class Method;
class Field;
using classfile::ConstantType;
using classfile::kConstantUtf8;
using classfile::kConstantInteger;
using classfile::kConstantFloat;
using classfile::kConstantLong;
using classfile::kConstantDouble;
using classfile::kConstantClass;
using classfile::kConstantString;
using classfile::kConstantFieldRef;
using classfile::kConstantMethodRef;
using classfile::kConstantInterfaceMethodRef;
using classfile::kConstantNameAndType;
using classfile::kConstantMethodHandle;
using classfile::kConstantMethodType;
using classfile::kConstantDynamic;
using classfile::kConstantInvokeDynamic;
using classfile::kConstantModule;
using classfile::kConstantPackage;

class Constant {
  private:
  ConstantType type_;
  public:
  explicit Constant(ConstantType type) : type_(type){}
  ConstantType GetConstantType() const {
    return type_;
  }
  virtual ~Constant() = default;
};
class ConstantPool {
private:
  Class* class_ptr_;
  std::vector<Constant*> constants_;
public:
  ConstantPool(Class* class_ptr, std::shared_ptr<classfile::ConstantPool> cf_constant_pool);
  Constant* GetConstant(uint32_t index) const;
  Class* GetClass() const {
    return class_ptr_;
  }
  const std::vector<Constant*>& GetConstants() {
    return constants_;
  }
};

class IntegerConstant : public Constant {
public:
  explicit IntegerConstant(int32_t value)
    : value_(value), Constant(kConstantInteger){}
  int32_t GetValue() const {
    return value_;
  }
private:
  int32_t value_;
};
class FloatConstant : public Constant {
public:
  explicit FloatConstant(float value)
    : value_(value), Constant(kConstantFloat){}
  float GetValue() const {
    return value_;
  }
private:
  float value_;
};
class LongConstant : public Constant {
public:
  explicit LongConstant(int64_t value)
    : value_(value), Constant(kConstantLong){}
  int64_t GetValue() const {
    return value_;
  }
private:
  int64_t value_;
};
class DoubleConstant : public Constant {
public:
  explicit DoubleConstant(double value)
    : value_(value), Constant(kConstantDouble){}
  double GetValue() const {
    return value_;
  }
private:
  double value_;
};
struct StringConstant : public Constant {
public:
  explicit StringConstant(std::string str) : value_(std::move(str)), Constant(kConstantUtf8){}
  static std::u16string DecodeMutf8(const char* bytes, int len);
  const std::string& GetValue() const {
    return value_;
  }
  static std::u16string Utf8ToUtf16(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    std::u16string u16str = DecodeMutf8(str.c_str(), str.size());
    return u16str;
  }
  static std::string Utf16ToUtf8(const char16_t* str) {
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> convert;
    return convert.to_bytes(str);
  }
private:
  const std::string value_;
};
struct SymRefConstant : public Constant {
public:
  SymRefConstant(const ConstantPool* constant_pool, std::string  class_name, ConstantType type)
    : constant_pool_(constant_pool), class_name_(std::move(class_name)), Constant(type), class_ptr_(nullptr) {}
  Class* ResolveClass();
  std::string GetClassName() const {
    return class_name_;
  }
  Class* GetClass() const {
    return class_ptr_;
  }
  const ConstantPool* GetConstantPool() const {
    return constant_pool_;
  }
private:
  const ConstantPool* constant_pool_;
  const std::string class_name_;
  Class* class_ptr_;
};
class ClassRefConstant : public SymRefConstant {
public:
  ClassRefConstant(const ConstantPool* constant_pool, const std::string& class_name)
    : SymRefConstant(constant_pool, class_name, kConstantClass) {}
};
class MemberRefConstant : public SymRefConstant {
  private:
  const std::string name_;
  const std::string descriptor_;
  public:
  MemberRefConstant(const ConstantPool* constant_pool,
                    std::string class_name, std::string  name,
                    std::string  descriptor, ConstantType type)
      : SymRefConstant(constant_pool, std::move(class_name), type), name_(std::move(name)), descriptor_(std::move(descriptor)) {}
  std::string GetName() const {
    return name_;
  }
  std::string GetDescriptor() const {
    return descriptor_;
  }
};
class FieldRefConstant : public MemberRefConstant {
public:
  FieldRefConstant(const ConstantPool* constant_pool,
                   const std::string& class_name, const std::string& name,
                   const std::string& descriptor)
    : MemberRefConstant(constant_pool, class_name, name, descriptor, kConstantFieldRef), field_(nullptr) {}
  const Field* ResolveField();
private:
  const Field* field_;
};
class MethodRefConstant : public MemberRefConstant {
public:
  MethodRefConstant(const ConstantPool* constant_pool,
                    const std::string& class_name, const std::string& name,
                    const std::string& descriptor)
    : MemberRefConstant(constant_pool, class_name, name, descriptor, kConstantMethodRef), method_(nullptr) {}
  const Method* ResolveMethod();
private:
  const Method* method_;
};
class InterfaceMethodRefConstant : public MemberRefConstant {
public:
  InterfaceMethodRefConstant(const ConstantPool* constant_pool,
                             std::string class_name, std::string name,
                             std::string descriptor)
    : MemberRefConstant(constant_pool, std::move(class_name),
                        std::move(name), std::move(descriptor),
                        kConstantMethodRef), interface_method_(nullptr) {}
  const Method* ResolveInterfaceMethod();
private:
  const Method* interface_method_;
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