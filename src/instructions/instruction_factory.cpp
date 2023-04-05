#include "instruction_factory.h"
#include "const_instructions.h"
#include "control_instructions.h"
#include "instructions/reference_instructions.h"
#include "load_instructions.h"
#include "store_instructions.h"
#include "extend_instructions.h"
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


std::shared_ptr<Instruction> createInstruction(uint8_t opcode) {
  switch (opcode) {
    case 0x00:
      return nop;
    case 0x01:
      return aconst_null;
    case 0x02:
      return iconst_m1;
    case 0x03:
      LOG(INFO) << "iconst_0";
      return iconst_0;
    case 0x04:
      LOG(INFO) << "iconst_1";
      return iconst_1;
    case 0x05:
      return iconst_2;
    case 0x06:
      return iconst_3;
    case 0x07:
      return iconst_4;
    case 0x08:
      return iconst_5;
    case 0x09:
      return lconst_0;
    case 0x0a:
      return lconst_1;
    case 0x0b:
      return fconst_0;
    case 0x0c:
      return fconst_1;
    case 0x0d:
      return fconst_2;
    case 0x0e:
      return dconst_0;
    case 0x0f:
      return dconst_1;
    case 0x10:
      LOG(INFO) << "BIPUSH";
      return std::make_shared<BIPUSH>();
    case 0x11:
      return std::make_shared<SIPUSH>();
    case 0x12:
      //return &LDC{}
      return std::make_shared<LDC>();
    case 0x13:
      //return &LDC_W{}
      return std::make_shared<LDC_W>();
    case 0x14:
      //return &LDC2_W2     
      return std::make_shared<LDC2_W>();
    case 0x15:
      //return &ILOAD{}
      return std::make_shared<LOAD<int32_t>>();
    case 0x16:
      //return &LLOAD{}
      return std::make_shared<LOAD<int64_t>>();
    case 0x17:
      //return &FLOAD{}
      return std::make_shared<LOAD<float>>();
    case 0x18:
      //return &DLOAD{}
      return std::make_shared<LOAD<double>>();
    case 0x19:
      //return &ALOAD{}
      return std::make_shared<LOAD<void*>>();
    case 0x1a:
      return iload_0;
    case 0x1b:
      LOG(INFO) << "iload_1";
      return iload_1;
    case 0x1c:
      LOG(INFO) << "iload_2";
      return iload_2;
    case 0x1d:
      return iload_3;
    case 0x1e:
      return lload_0;
    case 0x1f:
      return lload_1;
    case 0x20:
      return lload_2;
    case 0x21:
      return lload_3;
    case 0x22:
      return fload_0;
    case 0x23:
      return fload_1;
    case 0x24:
      return fload_2;
    case 0x25:
      return fload_3;
    case 0x26:
      return dload_0;
    case 0x27:
      return dload_1;
    case 0x28:
      return dload_2;
    case 0x29:
      return dload_3;
    case 0x2a:
      return aload_0;
    case 0x2b:
      return aload_1;
    case 0x2c:
      return aload_2;
    case 0x2d:
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
      return std::make_shared<STORE<int32_t>>();
    case 0x37:
      //return &LSTORE{}
      return std::make_shared<STORE<int64_t>>();
    case 0x38:
      //return &FSTORE{}
      return std::make_shared<STORE<float>>();
    case 0x39:
      //return &DSTORE{}
      return std::make_shared<STORE<double>>();
    case 0x3a:
      //return &ASTORE{}
      return std::make_shared<STORE<void*>>();
    case 0x3b:
      return istore_0;
    case 0x3c:
      LOG(INFO) << "istore_1";
      return istore_1;
    case 0x3d:
      LOG(INFO) << "istore_2";
      return istore_2;
    case 0x3e:
      return istore_3;
    case 0x3f:
      return lstore_0;
    case 0x40:
      return lstore_1;
    case 0x41:
      return lstore_2;
    case 0x42:
      return lstore_3;
    case 0x43:
      return fstore_0;
    case 0x44:
      return fstore_1;
    case 0x45:
      return fstore_2;
    case 0x46:
      return fstore_3;
    case 0x47:
      return dstore_0;
    case 0x48:
      return dstore_1;
    case 0x49:
      return dstore_2;
    case 0x4a:
      return dstore_3;
    case 0x4b:
      return astore_0;
    case 0x4c:
      return astore_1;
    case 0x4d:
      return astore_2;
    case 0x4e:
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
      return pop;
    case 0x58:
      return pop2;
    case 0x59:
      return dup;
    case 0x5a:
      return dup_x1;
    case 0x5b:
      return dup_x2;
    case 0x5c:
      return dup2;
    case 0x5d:
      return dup2_x1;
    case 0x5e:
      return dup2_x2;
    case 0x5f:
      return swap;
    case 0x60:
      LOG(INFO) << "iadd";
      return iadd;
    case 0x61:
      return ladd;
    case 0x62:
      return fadd;
    case 0x63:
      return dadd;
    case 0x64:
      return isub;
    case 0x65:
      return lsub;
    case 0x66:
      return fsub;
    case 0x67:
      return dsub;
    case 0x68:
      return imul;
    case 0x69:
      return lmul;
    case 0x6a:
      return fmul;
    case 0x6b:
      return dmul;
    case 0x6c:
      return idiv;
    case 0x6d:
      return ldiv;
    case 0x6e:
      return fdiv;
    case 0x6f:
      return ddiv;
    case 0x70:
      return irem;
    case 0x71:
      return lrem;
    case 0x72:
      return frem;
    case 0x73:
      return drem;
    case 0x74:
      return ineg;
    case 0x75:
      return lneg;
    case 0x76:
      return fneg;
    case 0x77:
      return dneg;
    case 0x78:
      return ishl;
    case 0x79:
      return lshl;
    case 0x7a:
      return ishr;
    case 0x7b:
      return lshr;
    case 0x7c:
      return iushr;
    case 0x7d:
      return lushr;
    case 0x7e:
      return iand;
    case 0x7f:
      return land;
    case 0x80:
      return ior;
    case 0x81:
      return lor;
    case 0x82:
      return ixor;
    case 0x83:
      return lxor;
    case 0x84:
      //return &IINC{}
      LOG(INFO) << "IINC";
      return std::make_shared<IINC>();
    case 0x85:
      return i2l;
    case 0x86:
      return i2f;
    case 0x87:
      return i2d;
    case 0x88:
      return l2i;
    case 0x89:
      return l2f;
    case 0x8a:
      return l2d;
    case 0x8b:
      return f2i;
    case 0x8c:
      return f2l;
    case 0x8d:
      return f2d;
    case 0x8e:
      return d2i;
    case 0x8f:
      return d2l;
    case 0x90:
      return d2f;
    case 0x91:
      return i2b;
    case 0x92:
      return i2c;
    case 0x93:
      return i2s;
    case 0x94:
      return lcmp;
    case 0x95:
      return fcmpl;
    case 0x96:
      return fcmpg;
    case 0x97:
      return dcmpl;
    case 0x98:
      return dcmpg;
    case 0x99:
      //return &IFEQ{}
      return std::make_shared<IFEQ>();
    case 0x9a:
      //return &IFNE{}
      return std::make_shared<IFNE>();
    case 0x9b:
      //return &IFLT{}
      return std::make_shared<IFLT>();
    case 0x9c:
      //return &IFGE{}
      return std::make_shared<IFGE>();
    case 0x9d:
      //return &IFGT{}
      return std::make_shared<IFGT>();
    case 0x9e:
      //return &IFLE{}
      return std::make_shared<IFLE>();
    case 0x9f:
      //return &IF_ICMPEQ{}
      return std::make_shared<IF_ICMPEQ>();
    case 0xa0:
      //return &IF_ICMPNE{}
      return std::make_shared<IF_ICMPNE>();
    case 0xa1:
      //return &IF_ICMPLT{}
      return std::make_shared<IF_ICMPLT>();
    case 0xa2:
      //return &IF_ICMPGE{}
      return std::make_shared<IF_ICMPGE>();
    case 0xa3:
      //return &IF_ICMPGT{}
      LOG(INFO) << "IF_ICMPGT";
      return std::make_shared<IF_ICMPGT>();
    case 0xa4:
      //return &IF_ICMPLE{}
      return std::make_shared<IF_ICMPLE>();
    case 0xa5:
      //return &IF_ACMPEQ{}
      return std::make_shared<IF_ACMPEQ>();
    case 0xa6:
      //return &IF_ACMPNE{}
      return std::make_shared<IF_ACMPNE>();
    case 0xa7:
      //return &GOTO{}
      LOG(INFO) << "GOTO";
      return std::make_shared<GOTO>();
    // case 0xa8:
    // 	return &JSR{}
    // case 0xa9:
    // 	return &RET{}
    case 0xaa:
      //return &TABLE_SWITCH{}
      return std::make_shared<TABLE_SWITCH>();
    case 0xab:
      //return &LOOKUP_SWITCH{}
      return std::make_shared<LOOKUP_SWITCH>();
    // case 0xac:
    // 	return ireturn
    // case 0xad:
    // 	return lreturn
    // case 0xae:
    // 	return freturn
    // case 0xaf:
    // 	return dreturn
    // case 0xb0:
    // 	return areturn
    // case 0xb1:
    // 	return _return
    case 0xb2:
    	//return &GET_STATIC{}
      return std::make_shared<GET_STATIC>();
    case 0xb3:
    	//return &PUT_STATIC{}
      return std::make_shared<PUT_STATIC>();
    case 0xb4:
    	//return &GET_FIELD{}
      return std::make_shared<GET_FIELD>();
    case 0xb5:
    	//return &PUT_FIELD{}
      return std::make_shared<PUT_FIELD>();
    case 0xb6:
    	//return &INVOKE_VIRTUAL{}
      return std::make_shared<INVOKE_VIRTUAL>();
    case 0xb7:
      //return &INVOKE_SPECIAL{}
      return std::make_shared<INVOKE_SPECIAL>();
    // case 0xb8:
    // 	return &INVOKE_STATIC{}
    // case 0xb9:
    // 	return &INVOKE_INTERFACE{}
    // case 0xba:
    // 	return &INVOKE_DYNAMIC{}
    case 0xbb:
      //return &NEW{}
      return std::make_shared<NEW>();
    // case 0xbc:
    // 	return &NEW_ARRAY{}
    // case 0xbd:
    // 	return &ANEW_ARRAY{}
    // case 0xbe:
    // 	return arraylength
    // case 0xbf:
    // 	return athrow
    // case 0xc0:
    // 	return &CHECK_CAST{}
    // case 0xc1:
    // 	return &INSTANCE_OF{}
    // case 0xc2:
    // 	return monitorenter
    // case 0xc3:
    // 	return monitorexit
    case 0xc4:
      //return &WIDE{}
      return std::make_shared<WIDE>();
    // case 0xc5:
    // 	return &MULTI_ANEW_ARRAY{}
    case 0xc6:
      //return &IFNULL{}
      return std::make_shared<IFNULL>();
    case 0xc7:
      //return &IFNONNULL{}
      return std::make_shared<IFNONNULL>();
    case 0xc8:
      //return &GOTO_W{}
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