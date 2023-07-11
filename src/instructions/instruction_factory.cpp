#include "instruction_factory.h"
#include "const_insts.h"
#include "control_insts.h"
#include "instructions/reference_insts.h"
#include "load_insts.h"
#include "store_insts.h"
#include "extend_insts.h"
#include "invoke_insts.h"
#include "return_insts.h"
#include <memory>
#include <glog/logging.h>
#include <runtime/oo/object.h>


namespace instructions {

//Only extends NoOperandsInstruction can use static instruction
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
static std::shared_ptr<LOAD_0<runtime::Object*>> aload_0 = std::make_shared<LOAD_0<runtime::Object*>>();
static std::shared_ptr<LOAD_1<runtime::Object*>> aload_1 = std::make_shared<LOAD_1<runtime::Object*>>();
static std::shared_ptr<LOAD_2<runtime::Object*>> aload_2 = std::make_shared<LOAD_2<runtime::Object*>>();
static std::shared_ptr<LOAD_3<runtime::Object*>> aload_3 = std::make_shared<LOAD_3<runtime::Object*>>();
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
static std::shared_ptr<STORE_0<runtime::Object*>> astore_0 = std::make_shared<STORE_0<runtime::Object*>>();
static std::shared_ptr<STORE_1<runtime::Object*>> astore_1 = std::make_shared<STORE_1<runtime::Object*>>();
static std::shared_ptr<STORE_2<runtime::Object*>> astore_2 = std::make_shared<STORE_2<runtime::Object*>>();
static std::shared_ptr<STORE_3<runtime::Object*>> astore_3 = std::make_shared<STORE_3<runtime::Object*>>();
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
static std::shared_ptr<I2R<char16_t>> i2c = std::make_shared<I2R<char16_t>>();//int to char
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

static std::shared_ptr<ARRAY_LENGTH> arraylength = std::make_shared<ARRAY_LENGTH>();
static std::shared_ptr<ALOAD<int8_t>> baload = std::make_shared<ALOAD<int8_t>>();
static std::shared_ptr<ALOAD<char16_t>> caload = std::make_shared<ALOAD<char16_t>>();
static std::shared_ptr<ALOAD<int16_t>> saload = std::make_shared<ALOAD<int16_t>>();
static std::shared_ptr<ALOAD<int32_t>> iaload = std::make_shared<ALOAD<int32_t>>();
static std::shared_ptr<ALOAD<int64_t>> laload = std::make_shared<ALOAD<int64_t>>();
static std::shared_ptr<ALOAD<float>> faload = std::make_shared<ALOAD<float>>();
static std::shared_ptr<ALOAD<double>> daload = std::make_shared<ALOAD<double>>();
static std::shared_ptr<ALOAD<runtime::Object*>> aaload = std::make_shared<ALOAD<runtime::Object*>>();
static std::shared_ptr<ASTORE<int8_t>> bastore = std::make_shared<ASTORE<int8_t>>();
static std::shared_ptr<ASTORE<char16_t>> castore = std::make_shared<ASTORE<char16_t>>();
static std::shared_ptr<ASTORE<int16_t>> sastore = std::make_shared<ASTORE<int16_t>>();
static std::shared_ptr<ASTORE<int32_t>> iastore = std::make_shared<ASTORE<int32_t>>();
static std::shared_ptr<ASTORE<int64_t>> lastore = std::make_shared<ASTORE<int64_t>>();
static std::shared_ptr<ASTORE<float>> fastore = std::make_shared<ASTORE<float>>();
static std::shared_ptr<ASTORE<double>> dastore = std::make_shared<ASTORE<double>>();
static std::shared_ptr<ASTORE<runtime::Object*>> aastore = std::make_shared<ASTORE<runtime::Object*>>();

static std::shared_ptr<ATHROW> athrow = std::make_shared<ATHROW>();


std::shared_ptr<Instruction> createInstruction(uint8_t opcode) {
  switch (opcode) {
    case 0x00:
      return nop;
    case 0x01:
      LOG_IF(INFO, INST_DEBUG) << "aconst_null";
      return aconst_null;
    case 0x02:
      LOG_IF(INFO, INST_DEBUG) << "iconst_m1";
      return iconst_m1;
    case 0x03:
      LOG_IF(INFO, INST_DEBUG) << "iconst_0";
      return iconst_0;
    case 0x04:
      LOG_IF(INFO, INST_DEBUG) << "iconst_1";
      return iconst_1;
    case 0x05:
      LOG_IF(INFO, INST_DEBUG) << "iconst_2";
      return iconst_2;
    case 0x06:
      LOG_IF(INFO, INST_DEBUG) << "iconst_3";
      return iconst_3;
    case 0x07:
      LOG_IF(INFO, INST_DEBUG) << "iconst_4";
      return iconst_4;
    case 0x08:
      LOG_IF(INFO, INST_DEBUG) << "iconst_5";
      return iconst_5;
    case 0x09:
      LOG_IF(INFO, INST_DEBUG) << "lconst_0";
      return lconst_0;
    case 0x0a:
      LOG_IF(INFO, INST_DEBUG) << "lconst_1";
      return lconst_1;
    case 0x0b:
      LOG_IF(INFO, INST_DEBUG) << "fconst_0";
      return fconst_0;
    case 0x0c:
      LOG_IF(INFO, INST_DEBUG) << "fconst_1";
      return fconst_1;
    case 0x0d:
      LOG_IF(INFO, INST_DEBUG) << "fconst_2";
      return fconst_2;
    case 0x0e:
      LOG_IF(INFO, INST_DEBUG) << "dconst_0";
      return dconst_0;
    case 0x0f:
      LOG_IF(INFO, INST_DEBUG) << "dconst_1";
      return dconst_1;
    case 0x10:
      LOG_IF(INFO, INST_DEBUG) << "bipush";
      return std::make_shared<BIPUSH>();
    case 0x11:
      LOG_IF(INFO, INST_DEBUG) << "sipush";
      return std::make_shared<SIPUSH>();
    case 0x12:
      //return &LDC{}
      LOG_IF(INFO, INST_DEBUG) << "ldc";
      return std::make_shared<LDC>();
    case 0x13:
      //return &LDC_W{}
      LOG_IF(INFO, INST_DEBUG) << "ldc_w";
      return std::make_shared<LDC_W>();
    case 0x14:
      //return &LDC2_W2  
      LOG_IF(INFO, INST_DEBUG) << "ldc2_w";   
      return std::make_shared<LDC2_W>();
    case 0x15:
      //return &ILOAD{}
      LOG_IF(INFO, INST_DEBUG) << "iload";
      return std::make_shared<LOAD<int32_t>>();
    case 0x16:
      //return &LLOAD{}
      LOG_IF(INFO, INST_DEBUG) << "lload";
      return std::make_shared<LOAD<int64_t>>();
    case 0x17:
      //return &FLOAD{}
      LOG_IF(INFO, INST_DEBUG) << "fload";
      return std::make_shared<LOAD<float>>();
    case 0x18:
      //return &DLOAD{}
      LOG_IF(INFO, INST_DEBUG) << "dload";
      return std::make_shared<LOAD<double>>();
    case 0x19:
      //return &ALOAD{}
      LOG_IF(INFO, INST_DEBUG) << "aload";
      return std::make_shared<LOAD<runtime::Object*>>();
    case 0x1a:
      LOG_IF(INFO, INST_DEBUG) << "iload_0";
      return iload_0;
    case 0x1b:
      LOG_IF(INFO, INST_DEBUG) << "iload_1";
      return iload_1;
    case 0x1c:
      LOG_IF(INFO, INST_DEBUG) << "iload_2";
      return iload_2;
    case 0x1d:
      LOG_IF(INFO, INST_DEBUG) << "iload_3";
      return iload_3;
    case 0x1e:
      LOG_IF(INFO, INST_DEBUG) << "lload_0";
      return lload_0;
    case 0x1f:
      LOG_IF(INFO, INST_DEBUG) << "lload_1";
      return lload_1;
    case 0x20:
      LOG_IF(INFO, INST_DEBUG) << "lload_2";
      return lload_2;
    case 0x21:
      LOG_IF(INFO, INST_DEBUG) << "lload_3";
      return lload_3;
    case 0x22:
      LOG_IF(INFO, INST_DEBUG) << "fload_0";
      return fload_0;
    case 0x23:
      LOG_IF(INFO, INST_DEBUG) << "fload_1";
      return fload_1;
    case 0x24:
      LOG_IF(INFO, INST_DEBUG) << "fload_2";
      return fload_2;
    case 0x25:
      LOG_IF(INFO, INST_DEBUG) << "fload_3";
      return fload_3;
    case 0x26:
      LOG_IF(INFO, INST_DEBUG) << "dload_0";
      return dload_0;
    case 0x27:
      LOG_IF(INFO, INST_DEBUG) << "dload_1";
      return dload_1;
    case 0x28:
      LOG_IF(INFO, INST_DEBUG) << "dload_2";
      return dload_2;
    case 0x29:
      LOG_IF(INFO, INST_DEBUG) << "dload_3";
      return dload_3;
    case 0x2a:
      LOG_IF(INFO, INST_DEBUG) << "aload_0";
      return aload_0;
    case 0x2b:
      LOG_IF(INFO, INST_DEBUG) << "aload_1";
      return aload_1;
    case 0x2c:
      LOG_IF(INFO, INST_DEBUG) << "aload_2";
      return aload_2;
    case 0x2d:
      LOG_IF(INFO, INST_DEBUG) << "aload_3";
      return aload_3;
    case 0x2e:
      LOG_IF(INFO, INST_DEBUG) << "iaload";
    	return iaload;
    case 0x2f:
      LOG_IF(INFO, INST_DEBUG) << "laload";
    	return laload;
    case 0x30:
      LOG_IF(INFO, INST_DEBUG) << "faload";
    	return faload;
    case 0x31:
      LOG_IF(INFO, INST_DEBUG) << "daload";
    	return daload;
    case 0x32:
      LOG_IF(INFO, INST_DEBUG) << "aaload";
    	return aaload;
    case 0x33:
      LOG_IF(INFO, INST_DEBUG) << "baload";
    	return baload;
    case 0x34:
      LOG_IF(INFO, INST_DEBUG) << "caload";
    	return caload;
    case 0x35:
      LOG_IF(INFO, INST_DEBUG) << "saload";
    	return saload;
    case 0x36:
      //return &ISTORE{}
      LOG_IF(INFO, INST_DEBUG) << "istore";
      return std::make_shared<STORE<int32_t>>();
    case 0x37:
      //return &LSTORE{}
      LOG_IF(INFO, INST_DEBUG) << "lstore";
      return std::make_shared<STORE<int64_t>>();
    case 0x38:
      //return &FSTORE{}
      LOG_IF(INFO, INST_DEBUG) << "fstore";
      return std::make_shared<STORE<float>>();
    case 0x39:
      //return &DSTORE{}
      LOG_IF(INFO, INST_DEBUG) << "dstore";
      return std::make_shared<STORE<double>>();
    case 0x3a:
      //return &ASTORE{}
      LOG_IF(INFO, INST_DEBUG) << "astore";
      return std::make_shared<STORE<runtime::Object*>>();
    case 0x3b:
      LOG_IF(INFO, INST_DEBUG) << "istore_0";
      return istore_0;
    case 0x3c:
      LOG_IF(INFO, INST_DEBUG) << "istore_1";
      return istore_1;
    case 0x3d:
      LOG_IF(INFO, INST_DEBUG) << "istore_2";
      return istore_2;
    case 0x3e:
      LOG_IF(INFO, INST_DEBUG) << "istore_3";
      return istore_3;
    case 0x3f:
      LOG_IF(INFO, INST_DEBUG) << "lstore_0";
      return lstore_0;
    case 0x40:
      LOG_IF(INFO, INST_DEBUG) << "lstore_1";
      return lstore_1;
    case 0x41:
      LOG_IF(INFO, INST_DEBUG) << "lstore_2";
      return lstore_2;
    case 0x42:
      LOG_IF(INFO, INST_DEBUG) << "lstore_3";
      return lstore_3;
    case 0x43:
      LOG_IF(INFO, INST_DEBUG) << "fstore_0";
      return fstore_0;
    case 0x44:
      LOG_IF(INFO, INST_DEBUG) << "fstore_1";
      return fstore_1;
    case 0x45:
      LOG_IF(INFO, INST_DEBUG) << "fstore_2";
      return fstore_2;
    case 0x46:
      LOG_IF(INFO, INST_DEBUG) << "fstore_3";
      return fstore_3;
    case 0x47:
      LOG_IF(INFO, INST_DEBUG) << "dstore_0";
      return dstore_0;
    case 0x48:
      LOG_IF(INFO, INST_DEBUG) << "dstore_1";
      return dstore_1;
    case 0x49:
      LOG_IF(INFO, INST_DEBUG) << "dstore_2";
      return dstore_2;
    case 0x4a:
      LOG_IF(INFO, INST_DEBUG) << "dstore_3";
      return dstore_3;
    case 0x4b:
      LOG_IF(INFO, INST_DEBUG) << "astore_0";
      return astore_0;
    case 0x4c:
      LOG_IF(INFO, INST_DEBUG) << "astore_1";
      return astore_1;
    case 0x4d:
      LOG_IF(INFO, INST_DEBUG) << "astore_2";
      return astore_2;
    case 0x4e:
      LOG_IF(INFO, INST_DEBUG) << "astore_3";
      return astore_3;
    case 0x4f:
      LOG_IF(INFO, INST_DEBUG) << "iastore";
    	return iastore;
    case 0x50:
      LOG_IF(INFO, INST_DEBUG) << "lastore";
    	return lastore;
    case 0x51:
      LOG_IF(INFO, INST_DEBUG) << "fastore";
    	return fastore;
    case 0x52:
      LOG_IF(INFO, INST_DEBUG) << "dastore";
    	return dastore;
    case 0x53:
      LOG_IF(INFO, INST_DEBUG) << "aastore";
    	return aastore;
    case 0x54:
      LOG_IF(INFO, INST_DEBUG) << "bastore";
    	return bastore;
    case 0x55:
      LOG_IF(INFO, INST_DEBUG) << "castore";
    	return castore;
    case 0x56:
      LOG_IF(INFO, INST_DEBUG) << "sastore";
    	return sastore;
    case 0x57:
      LOG_IF(INFO, INST_DEBUG) << "pop";
      return pop;
    case 0x58:
      LOG_IF(INFO, INST_DEBUG) << "pop2";
      return pop2;
    case 0x59:
      LOG_IF(INFO, INST_DEBUG) << "dup";
      return dup;
    case 0x5a:
      LOG_IF(INFO, INST_DEBUG) << "dup_x1";
      return dup_x1;
    case 0x5b:
      LOG_IF(INFO, INST_DEBUG) << "dup_x2";
      return dup_x2;
    case 0x5c:
      LOG_IF(INFO, INST_DEBUG) << "dup2";
      return dup2;
    case 0x5d:
      LOG_IF(INFO, INST_DEBUG) << "dup2_x1";
      return dup2_x1;
    case 0x5e:
      LOG_IF(INFO, INST_DEBUG) << "dup2_x2";
      return dup2_x2;
    case 0x5f:
      LOG_IF(INFO, INST_DEBUG) << "swap";
      return swap;
    case 0x60:
      LOG_IF(INFO, INST_DEBUG) << "iadd";
      return iadd;
    case 0x61:
      LOG_IF(INFO, INST_DEBUG) << "ladd";
      return ladd;
    case 0x62:
      LOG_IF(INFO, INST_DEBUG) << "fadd";
      return fadd;
    case 0x63:
      LOG_IF(INFO, INST_DEBUG) << "dadd";
      return dadd;
    case 0x64:
      LOG_IF(INFO, INST_DEBUG) << "isub";
      return isub;
    case 0x65:
      LOG_IF(INFO, INST_DEBUG) << "lsub";
      return lsub;
    case 0x66:
      LOG_IF(INFO, INST_DEBUG) << "fsub";
      return fsub;
    case 0x67:
      LOG_IF(INFO, INST_DEBUG) << "dsub";
      return dsub;
    case 0x68:
      LOG_IF(INFO, INST_DEBUG) << "imul";
      return imul;
    case 0x69:
      LOG_IF(INFO, INST_DEBUG) << "lmul";
      return lmul;
    case 0x6a:
      LOG_IF(INFO, INST_DEBUG) << "fmul";
      return fmul;
    case 0x6b:
      LOG_IF(INFO, INST_DEBUG) << "dmul";
      return dmul;
    case 0x6c:
      LOG_IF(INFO, INST_DEBUG) << "idiv";
      return idiv;
    case 0x6d:
      LOG_IF(INFO, INST_DEBUG) << "ldiv";
      return ldiv;
    case 0x6e:
      LOG_IF(INFO, INST_DEBUG) << "fdiv";
      return fdiv;
    case 0x6f:
      LOG_IF(INFO, INST_DEBUG) << "ddiv";
      return ddiv;
    case 0x70:
      LOG_IF(INFO, INST_DEBUG) << "irem";
      return irem;
    case 0x71:
      LOG_IF(INFO, INST_DEBUG) << "lrem";
      return lrem;
    case 0x72:
      LOG_IF(INFO, INST_DEBUG) << "frem";
      return frem;
    case 0x73:
      LOG_IF(INFO, INST_DEBUG) << "drem";
      return drem;
    case 0x74:
      LOG_IF(INFO, INST_DEBUG) << "ineg";
      return ineg;
    case 0x75:
      LOG_IF(INFO, INST_DEBUG) << "lneg";
      return lneg;
    case 0x76:
      LOG_IF(INFO, INST_DEBUG) << "fneg";
      return fneg;
    case 0x77:
      LOG_IF(INFO, INST_DEBUG) << "dneg";
      return dneg;
    case 0x78:
      LOG_IF(INFO, INST_DEBUG) << "ishl";
      return ishl;
    case 0x79:
      LOG_IF(INFO, INST_DEBUG) << "lshl";
      return lshl;
    case 0x7a:
      LOG_IF(INFO, INST_DEBUG) << "ishr";
      return ishr;
    case 0x7b:
      LOG_IF(INFO, INST_DEBUG) << "lshr";
      return lshr;
    case 0x7c:
      LOG_IF(INFO, INST_DEBUG) << "iushr";
      return iushr;
    case 0x7d:
      LOG_IF(INFO, INST_DEBUG) << "lushr";
      return lushr;
    case 0x7e:
      LOG_IF(INFO, INST_DEBUG) << "iand";
      return iand;
    case 0x7f:
      LOG_IF(INFO, INST_DEBUG) << "land";
      return land;
    case 0x80:
      LOG_IF(INFO, INST_DEBUG) << "ior";
      return ior;
    case 0x81:
      LOG_IF(INFO, INST_DEBUG) << "lor";
      return lor;
    case 0x82:
      LOG_IF(INFO, INST_DEBUG) << "ixor";
      return ixor;
    case 0x83:
      LOG_IF(INFO, INST_DEBUG) << "lxor";
      return lxor;
    case 0x84:
      //return &IINC{}
      LOG_IF(INFO, INST_DEBUG) << "iinc";
      return std::make_shared<IINC>();
    case 0x85:
      LOG_IF(INFO, INST_DEBUG) << "i2l";
      return i2l;
    case 0x86:
      LOG_IF(INFO, INST_DEBUG) << "i2f";
      return i2f;
    case 0x87:
      LOG_IF(INFO, INST_DEBUG) << "i2d";
      return i2d;
    case 0x88:
      LOG_IF(INFO, INST_DEBUG) << "l2i";
      return l2i;
    case 0x89:
      LOG_IF(INFO, INST_DEBUG) << "l2f";
      return l2f;
    case 0x8a:
      LOG_IF(INFO, INST_DEBUG) << "l2d";
      return l2d;
    case 0x8b:
      LOG_IF(INFO, INST_DEBUG) << "f2i";
      return f2i;
    case 0x8c:
      LOG_IF(INFO, INST_DEBUG) << "f2l";
      return f2l;
    case 0x8d:
      LOG_IF(INFO, INST_DEBUG) << "f2d";
      return f2d;
    case 0x8e:
      LOG_IF(INFO, INST_DEBUG) << "d2i";
      return d2i;
    case 0x8f:
      LOG_IF(INFO, INST_DEBUG) << "d2l";
      return d2l;
    case 0x90:
      LOG_IF(INFO, INST_DEBUG) << "d2f";
      return d2f;
    case 0x91:
      LOG_IF(INFO, INST_DEBUG) << "i2b";
      return i2b;
    case 0x92:
      LOG_IF(INFO, INST_DEBUG) << "i2c";
      return i2c;
    case 0x93:
      LOG_IF(INFO, INST_DEBUG) << "i2s";
      return i2s;
    case 0x94:
      LOG_IF(INFO, INST_DEBUG) << "lcmp";
      return lcmp;
    case 0x95:
      LOG_IF(INFO, INST_DEBUG) << "fcmpl";
      return fcmpl;
    case 0x96:
      LOG_IF(INFO, INST_DEBUG) << "fcmpg";
      return fcmpg;
    case 0x97:
      LOG_IF(INFO, INST_DEBUG) << "dcmpl";
      return dcmpl;
    case 0x98:
      LOG_IF(INFO, INST_DEBUG) << "dcmpg";
      return dcmpg;
    case 0x99:
      //return &IFEQ{}
      LOG_IF(INFO, INST_DEBUG) << "ifeq";
      return std::make_shared<IFEQ>();
    case 0x9a:
      //return &IFNE{}
      LOG_IF(INFO, INST_DEBUG) << "ifne";
      return std::make_shared<IFNE>();
    case 0x9b:
      //return &IFLT{}
      LOG_IF(INFO, INST_DEBUG) << "iflt";
      return std::make_shared<IFLT>();
    case 0x9c:
      //return &IFGE{}
      LOG_IF(INFO, INST_DEBUG) << "ifge";
      return std::make_shared<IFGE>();
    case 0x9d:
      //return &IFGT{}
      LOG_IF(INFO, INST_DEBUG) << "ifgt";
      return std::make_shared<IFGT>();
    case 0x9e:
      //return &IFLE{}
      LOG_IF(INFO, INST_DEBUG) << "ifle";
      return std::make_shared<IFLE>();
    case 0x9f:
      //return &IF_ICMPEQ{}
      LOG_IF(INFO, INST_DEBUG) << "if_icmpeq";
      return std::make_shared<IF_ICMPEQ>();
    case 0xa0:
      //return &IF_ICMPNE{}
      LOG_IF(INFO, INST_DEBUG) << "if_icmpne";
      return std::make_shared<IF_ICMPNE>();
    case 0xa1:
      //return &IF_ICMPLT{}
      LOG_IF(INFO, INST_DEBUG) << "if_icmplt";
      return std::make_shared<IF_ICMPLT>();
    case 0xa2:
      //return &IF_ICMPGE{}
      LOG_IF(INFO, INST_DEBUG) << "if_icmpge";
      return std::make_shared<IF_ICMPGE>();
    case 0xa3:
      //return &IF_ICMPGT{}
      LOG_IF(INFO, INST_DEBUG) << "if_icmpgt";
      return std::make_shared<IF_ICMPGT>();
    case 0xa4:
      //return &IF_ICMPLE{}
      LOG_IF(INFO, INST_DEBUG) << "if_icmple";
      return std::make_shared<IF_ICMPLE>();
    case 0xa5:
      //return &IF_ACMPEQ{}
      LOG_IF(INFO, INST_DEBUG) << "if_acmpeq";
      return std::make_shared<IF_ACMPEQ>();
    case 0xa6:
      //return &IF_ACMPNE{}
      LOG_IF(INFO, INST_DEBUG) << "if_acmpne";
      return std::make_shared<IF_ACMPNE>();
    case 0xa7:
      //return &GOTO{}
      LOG_IF(INFO, INST_DEBUG) << "goto";
      return std::make_shared<GOTO>();
    // case 0xa8:
    // 	return &JSR{}
    // case 0xa9:
    // 	return &RET{}
    case 0xaa:
      //return &TABLE_SWITCH{}
      LOG_IF(INFO, INST_DEBUG) << "tableswitch";
      return std::make_shared<TABLE_SWITCH>();
    case 0xab:
      //return &LOOKUP_SWITCH{}
      LOG_IF(INFO, INST_DEBUG) << "lookupswitch";
      return std::make_shared<LOOKUP_SWITCH>();
    case 0xac:
      LOG_IF(INFO, INST_DEBUG) << "ireturn";
    	return ireturn;
    case 0xad:
      LOG_IF(INFO, INST_DEBUG) << "lreturn";
    	return lreturn;
    case 0xae:
      LOG_IF(INFO, INST_DEBUG) << "freturn";
    	return freturn;
    case 0xaf:
      LOG_IF(INFO, INST_DEBUG) << "dreturn";
    	return dreturn;
    case 0xb0:
      LOG_IF(INFO, INST_DEBUG) << "areturn";
    	return areturn;
    case 0xb1:
      LOG_IF(INFO, INST_DEBUG) << "return";
    	return return_;
    case 0xb2:
    	//return &GET_STATIC{}
      LOG_IF(INFO, INST_DEBUG) << "getstatic";
      return std::make_shared<GET_STATIC>();
    case 0xb3:
    	//return &PUT_STATIC{}
      LOG_IF(INFO, INST_DEBUG) << "putstatic";
      return std::make_shared<PUT_STATIC>();
    case 0xb4:
    	//return &GET_FIELD{}
      LOG_IF(INFO, INST_DEBUG) << "getfield";
      return std::make_shared<GET_FIELD>();
    case 0xb5:
    	//return &PUT_FIELD{}
      LOG_IF(INFO, INST_DEBUG) << "putfield";
      return std::make_shared<PUT_FIELD>();
    case 0xb6:
    	//return &INVOKE_VIRTUAL{}
      LOG_IF(INFO, INST_DEBUG) << "invokevirtual";
      return std::make_shared<INVOKE_VIRTUAL>();
    case 0xb7:
      //return &INVOKE_SPECIAL{}
      LOG_IF(INFO, INST_DEBUG) << "invokespecial";
      return std::make_shared<INVOKE_SPECIAL>();
    case 0xb8:
    	//return &INVOKE_STATIC{}
      LOG_IF(INFO, INST_DEBUG) << "invokestatic";
      return std::make_shared<INVOKE_STATIC>();
    case 0xb9:
    	//return &INVOKE_INTERFACE{}
      LOG_IF(INFO, INST_DEBUG) << "invokeinterface";
      return std::make_shared<INVOKE_INTERFACE>();
    // case 0xba:
    // 	return &INVOKE_DYNAMIC{}
    case 0xbb:
      //return &NEW{}
      LOG_IF(INFO, INST_DEBUG) << "new";
      return std::make_shared<NEW>();
    case 0xbc:
      LOG_IF(INFO, INST_DEBUG) << "newarray";
    	return std::make_shared<NEW_ARRAY>();
    case 0xbd:
      LOG_IF(INFO, INST_DEBUG) << "anewarray";
    	return std::make_shared<ANEW_ARRAY>();
    case 0xbe:
      LOG_IF(INFO, INST_DEBUG) << "arraylength";
    	return arraylength;
    case 0xbf:
      LOG_IF(INFO, INST_DEBUG) << "athrow";
    	return athrow;
    case 0xc0:
    	//return &CHECK_CAST{}
      LOG_IF(INFO, INST_DEBUG) << "checkcast";
      return std::make_shared<CHECK_CAST>();
    case 0xc1:
    	//return &INSTANCE_OF{}
      LOG_IF(INFO, INST_DEBUG) << "instanceof";
      return std::make_shared<INSTANCE_OF>();
    // case 0xc2:
    // 	return monitorenter
    // case 0xc3:
    // 	return monitorexit
    case 0xc4:
      //return &WIDE{}
      LOG_IF(INFO, INST_DEBUG) << "wide";
      return std::make_shared<WIDE>();
    case 0xc5:
      LOG_IF(INFO, INST_DEBUG) << "multianewarray";
    	return std::make_shared<MULTI_ANEW_ARRAY>();
    case 0xc6:
      //return &IFNULL{}
      LOG_IF(INFO, INST_DEBUG) << "ifnull";
      return std::make_shared<IFNULL>();
    case 0xc7:
      //return &IFNONNULL{}
      LOG_IF(INFO, INST_DEBUG) << "ifnonnull";
      return std::make_shared<IFNONNULL>();
    case 0xc8:
      //return &GOTO_W{}
      LOG_IF(INFO, INST_DEBUG) << "goto_w";
      return std::make_shared<GOTO_W>();
    // case 0xc9:
    // 	return &JSR_W{}
    // case 0xca: breakpoint
    case 0xfe:
      LOG_IF(INFO, INST_DEBUG) << "invokenative";
      return std::make_shared<INVOKE_NATIVE>();
    // case 0xff: impdep2
    default:
      throw InstNotFoundException(opcode);
      
  }
}
}