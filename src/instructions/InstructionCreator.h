#include "ArithmeticInstructions.h"
#include "CompareInstructions.h"
#include "ConversionInstructions.h"
#include "StackInstructions.h"
#include "base/Instruction.h"
#include "ConstInstructions.h"
#include "LoadInstructions.h"
#include "StoreInstructions.h"
#include <cmath>
#include <memory>

namespace instructions {
std::shared_ptr<NopInstruction> nop = std::make_shared<NopInstruction>();
std::shared_ptr<ACONST_NULL> aconst_null = std::make_shared<ACONST_NULL>();
std::shared_ptr<ICONST_M1> iconst_m1 = std::make_shared<ICONST_M1>();
std::shared_ptr<ICONST_0> iconst_0 = std::make_shared<ICONST_0>();
std::shared_ptr<ICONST_1> iconst_1 = std::make_shared<ICONST_1>();
std::shared_ptr<ICONST_2> iconst_2 = std::make_shared<ICONST_2>();
std::shared_ptr<ICONST_3> iconst_3 = std::make_shared<ICONST_3>();
std::shared_ptr<ICONST_4> iconst_4 = std::make_shared<ICONST_4>();
std::shared_ptr<ICONST_5> iconst_5 = std::make_shared<ICONST_5>();
std::shared_ptr<LCONST_0> lconst_0 = std::make_shared<LCONST_0>();
std::shared_ptr<LCONST_1> lconst_1 = std::make_shared<LCONST_1>();
std::shared_ptr<FCONST_0> fconst_0 = std::make_shared<FCONST_0>();
std::shared_ptr<FCONST_1> fconst_1 = std::make_shared<FCONST_1>();
std::shared_ptr<FCONST_2> fconst_2 = std::make_shared<FCONST_2>();
std::shared_ptr<LOAD_0<int32_t>> iload_0 = std::make_shared<LOAD_0<int32_t>>();
std::shared_ptr<LOAD_1<int32_t>> iload_1 = std::make_shared<LOAD_1<int32_t>>();
std::shared_ptr<LOAD_2<int32_t>> iload_2 = std::make_shared<LOAD_2<int32_t>>();
std::shared_ptr<LOAD_3<int32_t>> iload_3 = std::make_shared<LOAD_3<int32_t>>();
std::shared_ptr<LOAD_0<int64_t>> lload_0 = std::make_shared<LOAD_0<int64_t>>();
std::shared_ptr<LOAD_1<int64_t>> lload_1 = std::make_shared<LOAD_1<int64_t>>();
std::shared_ptr<LOAD_2<int64_t>> lload_2 = std::make_shared<LOAD_2<int64_t>>();
std::shared_ptr<LOAD_3<int64_t>> lload_3 = std::make_shared<LOAD_3<int64_t>>();
std::shared_ptr<LOAD_0<float>> fload_0 = std::make_shared<LOAD_0<float>>();
std::shared_ptr<LOAD_1<float>> fload_1 = std::make_shared<LOAD_1<float>>();
std::shared_ptr<LOAD_2<float>> fload_2 = std::make_shared<LOAD_2<float>>();
std::shared_ptr<LOAD_3<float>> fload_3 = std::make_shared<LOAD_3<float>>();
std::shared_ptr<LOAD_0<double>> dload_0 = std::make_shared<LOAD_0<double>>();
std::shared_ptr<LOAD_1<double>> dload_1 = std::make_shared<LOAD_1<double>>();
std::shared_ptr<LOAD_2<double>> dload_2 = std::make_shared<LOAD_2<double>>();
std::shared_ptr<LOAD_3<double>> dload_3 = std::make_shared<LOAD_3<double>>();
std::shared_ptr<LOAD_0<void*>> aload_0 = std::make_shared<LOAD_0<void*>>();
std::shared_ptr<LOAD_1<void*>> aload_1 = std::make_shared<LOAD_1<void*>>();
std::shared_ptr<LOAD_2<void*>> aload_2 = std::make_shared<LOAD_2<void*>>();
std::shared_ptr<LOAD_3<void*>> aload_3 = std::make_shared<LOAD_3<void*>>();
std::shared_ptr<STORE_0<int32_t>> istore_0 = std::make_shared<STORE_0<int32_t>>();
std::shared_ptr<STORE_1<int32_t>> istore_1 = std::make_shared<STORE_1<int32_t>>();
std::shared_ptr<STORE_2<int32_t>> istore_2 = std::make_shared<STORE_2<int32_t>>();
std::shared_ptr<STORE_3<int32_t>> istore_3 = std::make_shared<STORE_3<int32_t>>();
std::shared_ptr<STORE_0<int64_t>> lstore_0 = std::make_shared<STORE_0<int64_t>>();
std::shared_ptr<STORE_1<int64_t>> lstore_1 = std::make_shared<STORE_1<int64_t>>();
std::shared_ptr<STORE_2<int64_t>> lstore_2 = std::make_shared<STORE_2<int64_t>>();
std::shared_ptr<STORE_3<int64_t>> lstore_3 = std::make_shared<STORE_3<int64_t>>();
std::shared_ptr<STORE_0<float>> fstore_0 = std::make_shared<STORE_0<float>>();
std::shared_ptr<STORE_1<float>> fstore_1 = std::make_shared<STORE_1<float>>();
std::shared_ptr<STORE_2<float>> fstore_2 = std::make_shared<STORE_2<float>>();
std::shared_ptr<STORE_3<float>> fstore_3 = std::make_shared<STORE_3<float>>();
std::shared_ptr<STORE_0<double>> dstore_0 = std::make_shared<STORE_0<double>>();
std::shared_ptr<STORE_1<double>> dstore_1 = std::make_shared<STORE_1<double>>();
std::shared_ptr<STORE_2<double>> dstore_2 = std::make_shared<STORE_2<double>>();
std::shared_ptr<STORE_3<double>> dstore_3 = std::make_shared<STORE_3<double>>();
std::shared_ptr<STORE_0<void*>> astore_0 = std::make_shared<STORE_0<void*>>();
std::shared_ptr<STORE_1<void*>> astore_1 = std::make_shared<STORE_1<void*>>();
std::shared_ptr<STORE_2<void*>> astore_2 = std::make_shared<STORE_2<void*>>();
std::shared_ptr<STORE_3<void*>> astore_3 = std::make_shared<STORE_3<void*>>();
std::shared_ptr<POP> pop = std::make_shared<POP>();
std::shared_ptr<POP2> pop2 = std::make_shared<POP2>();
std::shared_ptr<DUP> dup = std::make_shared<DUP>();
std::shared_ptr<DUP_X1> dup_x1 = std::make_shared<DUP_X1>();
std::shared_ptr<DUP_X2> dup_x2 = std::make_shared<DUP_X2>();
std::shared_ptr<DUP2> dup2 = std::make_shared<DUP2>();
std::shared_ptr<DUP2_X1> dup2_x1 = std::make_shared<DUP2_X1>();
std::shared_ptr<DUP2_X2> dup2_x2 = std::make_shared<DUP2_X2>();
std::shared_ptr<SWAP> swap = std::make_shared<SWAP>();
std::shared_ptr<ADD<int32_t>> iadd = std::make_shared<ADD<int32_t>>();
std::shared_ptr<ADD<int64_t>> ladd = std::make_shared<ADD<int64_t>>();
std::shared_ptr<ADD<float>> fadd = std::make_shared<ADD<float>>();
std::shared_ptr<ADD<double>> dadd = std::make_shared<ADD<double>>();
std::shared_ptr<SUB<int32_t>> isub = std::make_shared<SUB<int32_t>>();
std::shared_ptr<SUB<int64_t>> lsub = std::make_shared<SUB<int64_t>>();
std::shared_ptr<SUB<float>> fsub = std::make_shared<SUB<float>>();
std::shared_ptr<SUB<double>> dsub = std::make_shared<SUB<double>>();
std::shared_ptr<MUL<int32_t>> imul = std::make_shared<MUL<int32_t>>();
std::shared_ptr<MUL<int64_t>> lmul = std::make_shared<MUL<int64_t>>();
std::shared_ptr<MUL<float>> fmul = std::make_shared<MUL<float>>();
std::shared_ptr<MUL<double>> dmul = std::make_shared<MUL<double>>();
std::shared_ptr<DIV<int32_t>> idiv = std::make_shared<DIV<int32_t>>();
std::shared_ptr<DIV<int64_t>> ldiv = std::make_shared<DIV<int64_t>>();
std::shared_ptr<DIV<float>> fdiv = std::make_shared<DIV<float>>();
std::shared_ptr<DIV<double>> ddiv = std::make_shared<DIV<double>>();
std::shared_ptr<IREM<int32_t>> irem = std::make_shared<IREM<int32_t>>();
std::shared_ptr<IREM<int64_t>> lrem = std::make_shared<IREM<int64_t>>();
std::shared_ptr<FREM<float>> frem = std::make_shared<FREM<float>>();
std::shared_ptr<FREM<double>> drem = std::make_shared<FREM<double>>();
std::shared_ptr<NEG<int32_t>> ineg = std::make_shared<NEG<int32_t>>();
std::shared_ptr<NEG<int64_t>> lneg = std::make_shared<NEG<int64_t>>();
std::shared_ptr<NEG<float>> fneg = std::make_shared<NEG<float>>();
std::shared_ptr<NEG<double>> dneg = std::make_shared<NEG<double>>();
std::shared_ptr<SHL<int32_t>> ishl = std::make_shared<SHL<int32_t>>();
std::shared_ptr<SHL<int64_t>> lshl = std::make_shared<SHL<int64_t>>();
std::shared_ptr<SHR<int32_t>> ishr = std::make_shared<SHR<int32_t>>();
std::shared_ptr<SHR<int64_t>> lshr = std::make_shared<SHR<int64_t>>();
std::shared_ptr<USHR<int32_t>> iushr = std::make_shared<USHR<int32_t>>();
std::shared_ptr<USHR<int64_t>> lushr = std::make_shared<USHR<int64_t>>();
std::shared_ptr<AND<int32_t>> iand = std::make_shared<AND<int32_t>>();
std::shared_ptr<AND<int64_t>> land = std::make_shared<AND<int64_t>>();
std::shared_ptr<OR<int32_t>> ior = std::make_shared<OR<int32_t>>();
std::shared_ptr<OR<int64_t>> lor = std::make_shared<OR<int64_t>>();
std::shared_ptr<XOR<int32_t>> ixor = std::make_shared<XOR<int32_t>>();
std::shared_ptr<XOR<int64_t>> lxor = std::make_shared<XOR<int64_t>>();
std::shared_ptr<T2R<int32_t, int64_t>> i2l = std::make_shared<T2R<int32_t, int64_t>>();
std::shared_ptr<T2R<int32_t, float>> i2f = std::make_shared<T2R<int32_t, float>>();
std::shared_ptr<T2R<int32_t, double>> i2d = std::make_shared<T2R<int32_t, double>>();
std::shared_ptr<T2R<int64_t, int32_t>> l2i = std::make_shared<T2R<int64_t, int32_t>>();
std::shared_ptr<T2R<int64_t, float>> l2f = std::make_shared<T2R<int64_t, float>>();
std::shared_ptr<T2R<int64_t, double>> l2d = std::make_shared<T2R<int64_t, double>>();
std::shared_ptr<T2R<float, int32_t>> f2i = std::make_shared<T2R<float, int32_t>>();
std::shared_ptr<T2R<float, int64_t>> f2l = std::make_shared<T2R<float, int64_t>>();
std::shared_ptr<T2R<float, double>> f2d = std::make_shared<T2R<float, double>>();
std::shared_ptr<T2R<double, int32_t>> d2i = std::make_shared<T2R<double, int32_t>>();
std::shared_ptr<T2R<double, int64_t>> d2l = std::make_shared<T2R<double, int64_t>>();
std::shared_ptr<T2R<double, float>> d2f = std::make_shared<T2R<double, float>>();
std::shared_ptr<I2R<int8_t>> i2b = std::make_shared<I2R<int8_t>>();//int to byte
std::shared_ptr<I2R<uint16_t>> i2c = std::make_shared<I2R<uint16_t>>();//int to char
std::shared_ptr<I2R<int16_t>> i2s = std::make_shared<I2R<int16_t>>();//int to short
std::shared_ptr<LCMP> lcmp = std::make_shared<LCMP>();
std::shared_ptr<FCMPL<float>> fcmpl = std::make_shared<FCMPL<float>>();
std::shared_ptr<FCMPG<float>> fcmpg = std::make_shared<FCMPG<float>>();
std::shared_ptr<FCMPL<double>> dcmpl = std::make_shared<FCMPL<double>>();
std::shared_ptr<FCMPG<double>> dcmpg = std::make_shared<FCMPG<double>>();


std::shared_ptr<Instruction> createInstruction(uint8_t opcode);

}