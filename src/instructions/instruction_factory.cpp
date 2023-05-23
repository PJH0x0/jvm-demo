#include "instruction_factory.h"
#include "const_instructions.h"
#include "control_instructions.h"
#include "instructions/reference_instructions.h"
#include "load_instructions.h"
#include "store_instructions.h"
#include "extend_instructions.h"
#include "invoke_instructions.h"
#include "return_instructions.h"
#include <memory>
#include <glog/logging.h>

namespace instructions {
static std::shared_ptr<NopInstruction> nop = std::make_shared<NopInstruction>();
static std::shared_ptr<ACONST_NULL> aconst_null = std::make_shared<ACONST_NULL>();
static std::shared_ptr<ICONST_M1> iconst_m1 = std::make_shared<ICONST_M1>();
static std::shared_ptr<ICONST_0> iconst_0 = std::make_shared<ICONST_0>();
static std::shared_ptr<ICONST_1> iconst_1 = std::make_shared<ICONST_1>();
static std::shared_ptr<ICONST_2> iconst_2 = std::make_shared<ICONST_2>();
static std::shared_ptr<ICONST_3> iconst_3 = std::make_shared<ICONST_3>();
static std::shared_ptr<ICONST_4> iconst_4 = std::make_shared<ICONST_4>();
static std::shared_ptr<ICONST_5> iconst_5 = std::make_shared<ICONST_5>();
static std::shared_ptr<LCONST_0> lconst_0 = std::make_shared<LCONST_0>();
static std::shared_ptr<LCONST_1> lconst_1 = std::make_shared<LCONST_1>();
static std::shared_ptr<FCONST_0> fconst_0 = std::make_shared<FCONST_0>();
static std::shared_ptr<FCONST_1> fconst_1 = std::make_shared<FCONST_1>();
static std::shared_ptr<FCONST_2> fconst_2 = std::make_shared<FCONST_2>();
static std::shared_ptr<DCONST_0> dconst_0 = std::make_shared<DCONST_0>();
static std::shared_ptr<DCONST_1> dconst_1 = std::make_shared<DCONST_1>();
static std::shared_ptr<LOAD_0<int32_t>> iload_0 = std::make_shared<LOAD_0<int32_t>>();
static std::shared_ptr<LOAD_1<int32_t>> iload_1 = std::make_shared<LOAD_1<int32_t>>();
static std::shared_ptr<LOAD_2<int32_t>> iload_2 = std::make_shared<LOAD_2<int32_t>>();
static std::shared_ptr<LOAD_3<int32_t>> iload_3 = std::make_shared<LOAD_3<int32_t>>();
static std::shared_ptr<LOAD_0<int64_t>> lload_0 = std::make_shared<LOAD_0<int64_t>>();
static std::shared_ptr<LOAD_1<int64_t>> lload_1 = std::make_shared<LOAD_1<int64_t>>();
static std::shared_ptr<LOAD_2<int64_t>> lload_2 = std::make_shared<LOAD_2<int64_t>>();
static std::shared_ptr<LOAD_3<int64_t>> lload_3 = std::make_shared<LOAD_3<int64_t>>();
static std::shared_ptr<LOAD_0<float>> fload_0 = std::make_shared<LOAD_0<float>>();
static std::shared_ptr<LOAD_1<float>> fload_1 = std::make_shared<LOAD_1<float>>();
static std::shared_ptr<LOAD_2<float>> fload_2 = std::make_shared<LOAD_2<float>>();
static std::shared_ptr<LOAD_3<float>> fload_3 = std::make_shared<LOAD_3<float>>();
static std::shared_ptr<LOAD_0<double>> dload_0 = std::make_shared<LOAD_0<double>>();
static std::shared_ptr<LOAD_1<double>> dload_1 = std::make_shared<LOAD_1<double>>();
static std::shared_ptr<LOAD_2<double>> dload_2 = std::make_shared<LOAD_2<double>>();
static std::shared_ptr<LOAD_3<double>> dload_3 = std::make_shared<LOAD_3<double>>();
static std::shared_ptr<LOAD_0<void*>> aload_0 = std::make_shared<LOAD_0<void*>>();
static std::shared_ptr<LOAD_1<void*>> aload_1 = std::make_shared<LOAD_1<void*>>();
static std::shared_ptr<LOAD_2<void*>> aload_2 = std::make_shared<LOAD_2<void*>>();
static std::shared_ptr<LOAD_3<void*>> aload_3 = std::make_shared<LOAD_3<void*>>();
static std::shared_ptr<STORE_0<int32_t>> istore_0 = std::make_shared<STORE_0<int32_t>>();
static std::shared_ptr<STORE_1<int32_t>> istore_1 = std::make_shared<STORE_1<int32_t>>();
static std::shared_ptr<STORE_2<int32_t>> istore_2 = std::make_shared<STORE_2<int32_t>>();
static std::shared_ptr<STORE_3<int32_t>> istore_3 = std::make_shared<STORE_3<int32_t>>();
static std::shared_ptr<STORE_0<int64_t>> lstore_0 = std::make_shared<STORE_0<int64_t>>();
static std::shared_ptr<STORE_1<int64_t>> lstore_1 = std::make_shared<STORE_1<int64_t>>();
static std::shared_ptr<STORE_2<int64_t>> lstore_2 = std::make_shared<STORE_2<int64_t>>();
static std::shared_ptr<STORE_3<int64_t>> lstore_3 = std::make_shared<STORE_3<int64_t>>();
static std::shared_ptr<STORE_0<float>> fstore_0 = std::make_shared<STORE_0<float>>();
static std::shared_ptr<STORE_1<float>> fstore_1 = std::make_shared<STORE_1<float>>();
static std::shared_ptr<STORE_2<float>> fstore_2 = std::make_shared<STORE_2<float>>();
static std::shared_ptr<STORE_3<float>> fstore_3 = std::make_shared<STORE_3<float>>();
static std::shared_ptr<STORE_0<double>> dstore_0 = std::make_shared<STORE_0<double>>();
static std::shared_ptr<STORE_1<double>> dstore_1 = std::make_shared<STORE_1<double>>();
static std::shared_ptr<STORE_2<double>> dstore_2 = std::make_shared<STORE_2<double>>();
static std::shared_ptr<STORE_3<double>> dstore_3 = std::make_shared<STORE_3<double>>();
static std::shared_ptr<STORE_0<void*>> astore_0 = std::make_shared<STORE_0<void*>>();
static std::shared_ptr<STORE_1<void*>> astore_1 = std::make_shared<STORE_1<void*>>();
static std::shared_ptr<STORE_2<void*>> astore_2 = std::make_shared<STORE_2<void*>>();
static std::shared_ptr<STORE_3<void*>> astore_3 = std::make_shared<STORE_3<void*>>();
static std::shared_ptr<POP> pop = std::make_shared<POP>();
static std::shared_ptr<POP2> pop2 = std::make_shared<POP2>();
static std::shared_ptr<DUP> dup = std::make_shared<DUP>();
static std::shared_ptr<DUP_X1> dup_x1 = std::make_shared<DUP_X1>();
static std::shared_ptr<DUP_X2> dup_x2 = std::make_shared<DUP_X2>();
static std::shared_ptr<DUP2> dup2 = std::make_shared<DUP2>();
static std::shared_ptr<DUP2_X1> dup2_x1 = std::make_shared<DUP2_X1>();
static std::shared_ptr<DUP2_X2> dup2_x2 = std::make_shared<DUP2_X2>();
static std::shared_ptr<SWAP> swap = std::make_shared<SWAP>();
static std::shared_ptr<ADD<int32_t>> iadd = std::make_shared<ADD<int32_t>>();
static std::shared_ptr<ADD<int64_t>> ladd = std::make_shared<ADD<int64_t>>();
static std::shared_ptr<ADD<float>> fadd = std::make_shared<ADD<float>>();
static std::shared_ptr<ADD<double>> dadd = std::make_shared<ADD<double>>();
static std::shared_ptr<SUB<int32_t>> isub = std::make_shared<SUB<int32_t>>();
static std::shared_ptr<SUB<int64_t>> lsub = std::make_shared<SUB<int64_t>>();
static std::shared_ptr<SUB<float>> fsub = std::make_shared<SUB<float>>();
static std::shared_ptr<SUB<double>> dsub = std::make_shared<SUB<double>>();
static std::shared_ptr<MUL<int32_t>> imul = std::make_shared<MUL<int32_t>>();
static std::shared_ptr<MUL<int64_t>> lmul = std::make_shared<MUL<int64_t>>();
static std::shared_ptr<MUL<float>> fmul = std::make_shared<MUL<float>>();
static std::shared_ptr<MUL<double>> dmul = std::make_shared<MUL<double>>();
static std::shared_ptr<DIV<int32_t>> idiv = std::make_shared<DIV<int32_t>>();
static std::shared_ptr<DIV<int64_t>> ldiv = std::make_shared<DIV<int64_t>>();
static std::shared_ptr<DIV<float>> fdiv = std::make_shared<DIV<float>>();
static std::shared_ptr<DIV<double>> ddiv = std::make_shared<DIV<double>>();
static std::shared_ptr<IREM<int32_t>> irem = std::make_shared<IREM<int32_t>>();
static std::shared_ptr<IREM<int64_t>> lrem = std::make_shared<IREM<int64_t>>();
static std::shared_ptr<FREM<float>> frem = std::make_shared<FREM<float>>();
static std::shared_ptr<FREM<double>> drem = std::make_shared<FREM<double>>();
static std::shared_ptr<NEG<int32_t>> ineg = std::make_shared<NEG<int32_t>>();
static std::shared_ptr<NEG<int64_t>> lneg = std::make_shared<NEG<int64_t>>();
static std::shared_ptr<NEG<float>> fneg = std::make_shared<NEG<float>>();
static std::shared_ptr<NEG<double>> dneg = std::make_shared<NEG<double>>();
static std::shared_ptr<SHL<int32_t>> ishl = std::make_shared<SHL<int32_t>>();
static std::shared_ptr<SHL<int64_t>> lshl = std::make_shared<SHL<int64_t>>();
static std::shared_ptr<SHR<int32_t>> ishr = std::make_shared<SHR<int32_t>>();
static std::shared_ptr<SHR<int64_t>> lshr = std::make_shared<SHR<int64_t>>();
static std::shared_ptr<USHR<int32_t>> iushr = std::make_shared<USHR<int32_t>>();
static std::shared_ptr<USHR<int64_t>> lushr = std::make_shared<USHR<int64_t>>();
static std::shared_ptr<AND<int32_t>> iand = std::make_shared<AND<int32_t>>();
static std::shared_ptr<AND<int64_t>> land = std::make_shared<AND<int64_t>>();
static std::shared_ptr<OR<int32_t>> ior = std::make_shared<OR<int32_t>>();
static std::shared_ptr<OR<int64_t>> lor = std::make_shared<OR<int64_t>>();
static std::shared_ptr<XOR<int32_t>> ixor = std::make_shared<XOR<int32_t>>();
static std::shared_ptr<XOR<int64_t>> lxor = std::make_shared<XOR<int64_t>>();
static std::shared_ptr<T2R<int32_t, int64_t>> i2l = std::make_shared<T2R<int32_t, int64_t>>();
static std::shared_ptr<T2R<int32_t, float>> i2f = std::make_shared<T2R<int32_t, float>>();
static std::shared_ptr<T2R<int32_t, double>> i2d = std::make_shared<T2R<int32_t, double>>();
static std::shared_ptr<T2R<int64_t, int32_t>> l2i = std::make_shared<T2R<int64_t, int32_t>>();
static std::shared_ptr<T2R<int64_t, float>> l2f = std::make_shared<T2R<int64_t, float>>();
static std::shared_ptr<T2R<int64_t, double>> l2d = std::make_shared<T2R<int64_t, double>>();
static std::shared_ptr<T2R<float, int32_t>> f2i = std::make_shared<T2R<float, int32_t>>();
static std::shared_ptr<T2R<float, int64_t>> f2l = std::make_shared<T2R<float, int64_t>>();
static std::shared_ptr<T2R<float, double>> f2d = std::make_shared<T2R<float, double>>();
static std::shared_ptr<T2R<double, int32_t>> d2i = std::make_shared<T2R<double, int32_t>>();
static std::shared_ptr<T2R<double, int64_t>> d2l = std::make_shared<T2R<double, int64_t>>();
static std::shared_ptr<T2R<double, float>> d2f = std::make_shared<T2R<double, float>>();
static std::shared_ptr<I2R<int8_t>> i2b = std::make_shared<I2R<int8_t>>();//int to byte
static std::shared_ptr<I2R<uint16_t>> i2c = std::make_shared<I2R<uint16_t>>();//int to char
static std::shared_ptr<I2R<int16_t>> i2s = std::make_shared<I2R<int16_t>>();//int to short
static std::shared_ptr<LCMP> lcmp = std::make_shared<LCMP>();
static std::shared_ptr<FCMPL<float>> fcmpl = std::make_shared<FCMPL<float>>();
static std::shared_ptr<FCMPG<float>> fcmpg = std::make_shared<FCMPG<float>>();
static std::shared_ptr<FCMPL<double>> dcmpl = std::make_shared<FCMPL<double>>();
static std::shared_ptr<FCMPG<double>> dcmpg = std::make_shared<FCMPG<double>>();
static std::shared_ptr<RETURN> return_ = std::make_shared<RETURN>();
static std::shared_ptr<IRETURN> ireturn = std::make_shared<IRETURN>();
static std::shared_ptr<LRETURN> lreturn = std::make_shared<LRETURN>();
static std::shared_ptr<FRETURN> freturn = std::make_shared<FRETURN>();
static std::shared_ptr<DRETURN> dreturn = std::make_shared<DRETURN>();
static std::shared_ptr<ARETURN> areturn = std::make_shared<ARETURN>();


std::shared_ptr<Instruction> createInstruction(uint8_t opcode) {
  switch (opcode) {
    case 0x00:
      return nop;
    case 0x01:
      LOG(INFO) << "aconst_null";
      return aconst_null;
    case 0x02:
      LOG(INFO) << "iconst_m1";
      return iconst_m1;
    case 0x03:
      LOG(INFO) << "iconst_0";
      return iconst_0;
    case 0x04:
      LOG(INFO) << "iconst_1";
      return iconst_1;
    case 0x05:
      LOG(INFO) << "iconst_2";
      return iconst_2;
    case 0x06:
      LOG(INFO) << "iconst_3";
      return iconst_3;
    case 0x07:
      LOG(INFO) << "iconst_4";
      return iconst_4;
    case 0x08:
      LOG(INFO) << "iconst_5";
      return iconst_5;
    case 0x09:
      LOG(INFO) << "lconst_0";
      return lconst_0;
    case 0x0a:
      LOG(INFO) << "lconst_1";
      return lconst_1;
    case 0x0b:
      LOG(INFO) << "fconst_0";
      return fconst_0;
    case 0x0c:
      LOG(INFO) << "fconst_1";
      return fconst_1;
    case 0x0d:
      LOG(INFO) << "fconst_2";
      return fconst_2;
    case 0x0e:
      LOG(INFO) << "dconst_0";
      return dconst_0;
    case 0x0f:
      LOG(INFO) << "dconst_1";
      return dconst_1;
    case 0x10:
      LOG(INFO) << "bipush";
      return std::make_shared<BIPUSH>();
    case 0x11:
      LOG(INFO) << "sipush";
      return std::make_shared<SIPUSH>();
    case 0x12:
      //return &LDC{}
      LOG(INFO) << "ldc";
      return std::make_shared<LDC>();
    case 0x13:
      //return &LDC_W{}
      LOG(INFO) << "ldc_w";
      return std::make_shared<LDC_W>();
    case 0x14:
      //return &LDC2_W2  
      LOG(INFO) << "ldc2_w";   
      return std::make_shared<LDC2_W>();
    case 0x15:
      //return &ILOAD{}
      LOG(INFO) << "iload";
      return std::make_shared<LOAD<int32_t>>();
    case 0x16:
      //return &LLOAD{}
      LOG(INFO) << "lload";
      return std::make_shared<LOAD<int64_t>>();
    case 0x17:
      //return &FLOAD{}
      LOG(INFO) << "fload";
      return std::make_shared<LOAD<float>>();
    case 0x18:
      //return &DLOAD{}
      LOG(INFO) << "dload";
      return std::make_shared<LOAD<double>>();
    case 0x19:
      //return &ALOAD{}
      LOG(INFO) << "aload";
      return std::make_shared<LOAD<void*>>();
    case 0x1a:
      LOG(INFO) << "iload_0";
      return iload_0;
    case 0x1b:
      LOG(INFO) << "iload_1";
      return iload_1;
    case 0x1c:
      LOG(INFO) << "iload_2";
      return iload_2;
    case 0x1d:
      LOG(INFO) << "iload_3";
      return iload_3;
    case 0x1e:
      LOG(INFO) << "lload_0";
      return lload_0;
    case 0x1f:
      LOG(INFO) << "lload_1";
      return lload_1;
    case 0x20:
      LOG(INFO) << "lload_2";
      return lload_2;
    case 0x21:
      LOG(INFO) << "lload_3";
      return lload_3;
    case 0x22:
      LOG(INFO) << "fload_0";
      return fload_0;
    case 0x23:
      LOG(INFO) << "fload_1";
      return fload_1;
    case 0x24:
      LOG(INFO) << "fload_2";
      return fload_2;
    case 0x25:
      LOG(INFO) << "fload_3";
      return fload_3;
    case 0x26:
      LOG(INFO) << "dload_0";
      return dload_0;
    case 0x27:
      LOG(INFO) << "dload_1";
      return dload_1;
    case 0x28:
      LOG(INFO) << "dload_2";
      return dload_2;
    case 0x29:
      LOG(INFO) << "dload_3";
      return dload_3;
    case 0x2a:
      LOG(INFO) << "aload_0";
      return aload_0;
    case 0x2b:
      LOG(INFO) << "aload_1";
      return aload_1;
    case 0x2c:
      LOG(INFO) << "aload_2";
      return aload_2;
    case 0x2d:
      LOG(INFO) << "aload_3";
      return aload_3;
    // case 0x2e:
    // 	return iaload
    // case 0x2f:
    // 	return laload
    // case 0x30:
    // 	return faload
    // case 0x31:
    // 	return daload
    // case 0x32:
    // 	return aaload
    // case 0x33:
    // 	return baload
    // case 0x34:
    // 	return caload
    // case 0x35:
    // 	return saload
    case 0x36:
      //return &ISTORE{}
      LOG(INFO) << "istore";
      return std::make_shared<STORE<int32_t>>();
    case 0x37:
      //return &LSTORE{}
      LOG(INFO) << "lstore";
      return std::make_shared<STORE<int64_t>>();
    case 0x38:
      //return &FSTORE{}
      LOG(INFO) << "fstore";
      return std::make_shared<STORE<float>>();
    case 0x39:
      //return &DSTORE{}
      LOG(INFO) << "dstore";
      return std::make_shared<STORE<double>>();
    case 0x3a:
      //return &ASTORE{}
      LOG(INFO) << "astore";
      return std::make_shared<STORE<void*>>();
    case 0x3b:
      LOG(INFO) << "istore_0";
      return istore_0;
    case 0x3c:
      LOG(INFO) << "istore_1";
      return istore_1;
    case 0x3d:
      LOG(INFO) << "istore_2";
      return istore_2;
    case 0x3e:
      LOG(INFO) << "istore_3";
      return istore_3;
    case 0x3f:
      LOG(INFO) << "lstore_0";
      return lstore_0;
    case 0x40:
      LOG(INFO) << "lstore_1";
      return lstore_1;
    case 0x41:
      LOG(INFO) << "lstore_2";
      return lstore_2;
    case 0x42:
      LOG(INFO) << "lstore_3";
      return lstore_3;
    case 0x43:
      LOG(INFO) << "fstore_0";
      return fstore_0;
    case 0x44:
      LOG(INFO) << "fstore_1";
      return fstore_1;
    case 0x45:
      LOG(INFO) << "fstore_2";
      return fstore_2;
    case 0x46:
      LOG(INFO) << "fstore_3";
      return fstore_3;
    case 0x47:
      LOG(INFO) << "dstore_0";
      return dstore_0;
    case 0x48:
      LOG(INFO) << "dstore_1";
      return dstore_1;
    case 0x49:
      LOG(INFO) << "dstore_2";
      return dstore_2;
    case 0x4a:
      LOG(INFO) << "dstore_3";
      return dstore_3;
    case 0x4b:
      LOG(INFO) << "astore_0";
      return astore_0;
    case 0x4c:
      LOG(INFO) << "astore_1";
      return astore_1;
    case 0x4d:
      LOG(INFO) << "astore_2";
      return astore_2;
    case 0x4e:
      LOG(INFO) << "astore_3";
      return astore_3;
    // case 0x4f:
    // 	return iastore
    // case 0x50:
    // 	return lastore
    // case 0x51:
    // 	return fastore
    // case 0x52:
    // 	return dastore
    // case 0x53:
    // 	return aastore
    // case 0x54:
    // 	return bastore
    // case 0x55:
    // 	return castore
    // case 0x56:
    // 	return sastore
    case 0x57:
      LOG(INFO) << "pop";
      return pop;
    case 0x58:
      LOG(INFO) << "pop2";
      return pop2;
    case 0x59:
      LOG(INFO) << "dup";
      return dup;
    case 0x5a:
      LOG(INFO) << "dup_x1";
      return dup_x1;
    case 0x5b:
      LOG(INFO) << "dup_x2";
      return dup_x2;
    case 0x5c:
      LOG(INFO) << "dup2";
      return dup2;
    case 0x5d:
      LOG(INFO) << "dup2_x1";
      return dup2_x1;
    case 0x5e:
      LOG(INFO) << "dup2_x2";
      return dup2_x2;
    case 0x5f:
      LOG(INFO) << "swap";
      return swap;
    case 0x60:
      LOG(INFO) << "iadd";
      return iadd;
    case 0x61:
      LOG(INFO) << "ladd";
      return ladd;
    case 0x62:
      LOG(INFO) << "fadd";
      return fadd;
    case 0x63:
      LOG(INFO) << "dadd";
      return dadd;
    case 0x64:
      LOG(INFO) << "isub";
      return isub;
    case 0x65:
      LOG(INFO) << "lsub";
      return lsub;
    case 0x66:
      LOG(INFO) << "fsub";
      return fsub;
    case 0x67:
      LOG(INFO) << "dsub";
      return dsub;
    case 0x68:
      LOG(INFO) << "imul";
      return imul;
    case 0x69:
      LOG(INFO) << "lmul";
      return lmul;
    case 0x6a:
      LOG(INFO) << "fmul";
      return fmul;
    case 0x6b:
      LOG(INFO) << "dmul";
      return dmul;
    case 0x6c:
      LOG(INFO) << "idiv";
      return idiv;
    case 0x6d:
      LOG(INFO) << "ldiv";
      return ldiv;
    case 0x6e:
      LOG(INFO) << "fdiv";
      return fdiv;
    case 0x6f:
      LOG(INFO) << "ddiv";
      return ddiv;
    case 0x70:
      LOG(INFO) << "irem";
      return irem;
    case 0x71:
      LOG(INFO) << "lrem";
      return lrem;
    case 0x72:
      LOG(INFO) << "frem";
      return frem;
    case 0x73:
      LOG(INFO) << "drem";
      return drem;
    case 0x74:
      LOG(INFO) << "ineg";
      return ineg;
    case 0x75:
      LOG(INFO) << "lneg";
      return lneg;
    case 0x76:
      LOG(INFO) << "fneg";
      return fneg;
    case 0x77:
      LOG(INFO) << "dneg";
      return dneg;
    case 0x78:
      LOG(INFO) << "ishl";
      return ishl;
    case 0x79:
      LOG(INFO) << "lshl";
      return lshl;
    case 0x7a:
      LOG(INFO) << "ishr";
      return ishr;
    case 0x7b:
      LOG(INFO) << "lshr";
      return lshr;
    case 0x7c:
      LOG(INFO) << "iushr";
      return iushr;
    case 0x7d:
      LOG(INFO) << "lushr";
      return lushr;
    case 0x7e:
      LOG(INFO) << "iand";
      return iand;
    case 0x7f:
      LOG(INFO) << "land";
      return land;
    case 0x80:
      LOG(INFO) << "ior";
      return ior;
    case 0x81:
      LOG(INFO) << "lor";
      return lor;
    case 0x82:
      LOG(INFO) << "ixor";
      return ixor;
    case 0x83:
      LOG(INFO) << "lxor";
      return lxor;
    case 0x84:
      //return &IINC{}
      LOG(INFO) << "iinc";
      return std::make_shared<IINC>();
    case 0x85:
      LOG(INFO) << "i2l";
      return i2l;
    case 0x86:
      LOG(INFO) << "i2f";
      return i2f;
    case 0x87:
      LOG(INFO) << "i2d";
      return i2d;
    case 0x88:
      LOG(INFO) << "l2i";
      return l2i;
    case 0x89:
      LOG(INFO) << "l2f";
      return l2f;
    case 0x8a:
      LOG(INFO) << "l2d";
      return l2d;
    case 0x8b:
      LOG(INFO) << "f2i";
      return f2i;
    case 0x8c:
      LOG(INFO) << "f2l";
      return f2l;
    case 0x8d:
      LOG(INFO) << "f2d";
      return f2d;
    case 0x8e:
      LOG(INFO) << "d2i";
      return d2i;
    case 0x8f:
      LOG(INFO) << "d2l";
      return d2l;
    case 0x90:
      LOG(INFO) << "d2f";
      return d2f;
    case 0x91:
      LOG(INFO) << "i2b";
      return i2b;
    case 0x92:
      LOG(INFO) << "i2c";
      return i2c;
    case 0x93:
      LOG(INFO) << "i2s";
      return i2s;
    case 0x94:
      LOG(INFO) << "lcmp";
      return lcmp;
    case 0x95:
      LOG(INFO) << "fcmpl";
      return fcmpl;
    case 0x96:
      LOG(INFO) << "fcmpg";
      return fcmpg;
    case 0x97:
      LOG(INFO) << "dcmpl";
      return dcmpl;
    case 0x98:
      LOG(INFO) << "dcmpg";
      return dcmpg;
    case 0x99:
      //return &IFEQ{}
      LOG(INFO) << "ifeq";
      return std::make_shared<IFEQ>();
    case 0x9a:
      //return &IFNE{}
      LOG(INFO) << "ifne";
      return std::make_shared<IFNE>();
    case 0x9b:
      //return &IFLT{}
      LOG(INFO) << "iflt";
      return std::make_shared<IFLT>();
    case 0x9c:
      //return &IFGE{}
      LOG(INFO) << "ifge";
      return std::make_shared<IFGE>();
    case 0x9d:
      //return &IFGT{}
      LOG(INFO) << "ifgt";
      return std::make_shared<IFGT>();
    case 0x9e:
      //return &IFLE{}
      LOG(INFO) << "ifle";
      return std::make_shared<IFLE>();
    case 0x9f:
      //return &IF_ICMPEQ{}
      LOG(INFO) << "if_icmpeq";
      return std::make_shared<IF_ICMPEQ>();
    case 0xa0:
      //return &IF_ICMPNE{}
      LOG(INFO) << "if_icmpne";
      return std::make_shared<IF_ICMPNE>();
    case 0xa1:
      //return &IF_ICMPLT{}
      LOG(INFO) << "if_icmplt";
      return std::make_shared<IF_ICMPLT>();
    case 0xa2:
      //return &IF_ICMPGE{}
      LOG(INFO) << "if_icmpge";
      return std::make_shared<IF_ICMPGE>();
    case 0xa3:
      //return &IF_ICMPGT{}
      LOG(INFO) << "if_icmpgt";
      return std::make_shared<IF_ICMPGT>();
    case 0xa4:
      //return &IF_ICMPLE{}
      LOG(INFO) << "if_icmple";
      return std::make_shared<IF_ICMPLE>();
    case 0xa5:
      //return &IF_ACMPEQ{}
      LOG(INFO) << "if_acmpeq";
      return std::make_shared<IF_ACMPEQ>();
    case 0xa6:
      //return &IF_ACMPNE{}
      LOG(INFO) << "if_acmpne";
      return std::make_shared<IF_ACMPNE>();
    case 0xa7:
      //return &GOTO{}
      LOG(INFO) << "goto";
      return std::make_shared<GOTO>();
    // case 0xa8:
    // 	return &JSR{}
    // case 0xa9:
    // 	return &RET{}
    case 0xaa:
      //return &TABLE_SWITCH{}
      LOG(INFO) << "table_switch";
      return std::make_shared<TABLE_SWITCH>();
    case 0xab:
      //return &LOOKUP_SWITCH{}
      LOG(INFO) << "lookup_switch";
      return std::make_shared<LOOKUP_SWITCH>();
    case 0xac:
      LOG(INFO) << "ireturn";
    	return ireturn;
    case 0xad:
      LOG(INFO) << "lreturn";
    	return lreturn;
    case 0xae:
      LOG(INFO) << "freturn";
    	return freturn;
    case 0xaf:
      LOG(INFO) << "dreturn";
    	return dreturn;
    case 0xb0:
      LOG(INFO) << "areturn";
    	return areturn;
    case 0xb1:
      LOG(INFO) << "return";
    	return return_;
    case 0xb2:
    	//return &GET_STATIC{}
      LOG(INFO) << "get_static";
      return std::make_shared<GET_STATIC>();
    case 0xb3:
    	//return &PUT_STATIC{}
      LOG(INFO) << "put_static";
      return std::make_shared<PUT_STATIC>();
    case 0xb4:
    	//return &GET_FIELD{}
      LOG(INFO) << "get_field";
      return std::make_shared<GET_FIELD>();
    case 0xb5:
    	//return &PUT_FIELD{}
      LOG(INFO) << "put_field";
      return std::make_shared<PUT_FIELD>();
    case 0xb6:
    	//return &INVOKE_VIRTUAL{}
      LOG(INFO) << "invoke_virtual";
      return std::make_shared<INVOKE_VIRTUAL>();
    case 0xb7:
      //return &INVOKE_SPECIAL{}
      LOG(INFO) << "invoke_special";
      return std::make_shared<INVOKE_SPECIAL>();
    case 0xb8:
    	//return &INVOKE_STATIC{}
      LOG(INFO) << "invoke_static";
      return std::make_shared<INVOKE_STATIC>();
    case 0xb9:
    	//return &INVOKE_INTERFACE{}
      LOG(INFO) << "invoke_interface";
      return std::make_shared<INVOKE_INTERFACE>();
    // case 0xba:
    // 	return &INVOKE_DYNAMIC{}
    case 0xbb:
      //return &NEW{}
      LOG(INFO) << "new";
      return std::make_shared<NEW>();
    // case 0xbc:
    // 	return &NEW_ARRAY{}
    // case 0xbd:
    // 	return &ANEW_ARRAY{}
    // case 0xbe:
    // 	return arraylength
    // case 0xbf:
    // 	return athrow
    case 0xc0:
    	//return &CHECK_CAST{}
      LOG(INFO) << "check_cast";
      return std::make_shared<CHECK_CAST>();
    case 0xc1:
    	//return &INSTANCE_OF{}
      LOG(INFO) << "instance_of";
      return std::make_shared<INSTANCE_OF>();
    // case 0xc2:
    // 	return monitorenter
    // case 0xc3:
    // 	return monitorexit
    case 0xc4:
      //return &WIDE{}
      LOG(INFO) << "wide";
      return std::make_shared<WIDE>();
    // case 0xc5:
    // 	return &MULTI_ANEW_ARRAY{}
    case 0xc6:
      //return &IFNULL{}
      LOG(INFO) << "ifnull";
      return std::make_shared<IFNULL>();
    case 0xc7:
      //return &IFNONNULL{}
      LOG(INFO) << "ifnonnull";
      return std::make_shared<IFNONNULL>();
    case 0xc8:
      //return &GOTO_W{}
      LOG(INFO) << "goto_w";
      return std::make_shared<GOTO_W>();
    // case 0xc9:
    // 	return &JSR_W{}
    // case 0xca: breakpoint
    // case 0xfe: impdep1
    // case 0xff: impdep2
    default:
      throw InstNotFoundException(opcode);
      
  }
}
}