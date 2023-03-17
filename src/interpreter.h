#pragma once
#include "classfile/MemberInfo.h"
#include "classfile/AttributeInfo.h"
#include "instructions/base/Instruction.h"
#include "rtda/Frame.h"
#include "rtda/Thread.h"
#include "instructions/base/BytecodeReader.h"
#include "instructions/InstructionCreator.h"
using namespace classfile;

void loop(std::shared_ptr<rtda::Thread> thread, std::vector<u1>& byteCodes);
void interpret(std::shared_ptr<MemberInfo> methodInfo);