#pragma once
#include "classfile/member_info.h"
#include "classfile/attr_info.h"
#include "instructions/base/base_instructions.h"
#include "rtda/frame.h"
#include "rtda/thread.h"
#include "instructions/base/bytecode_reader.h"
#include "instructions/instruction_factory.h"
using namespace classfile;

void loop(std::shared_ptr<rtda::Thread> thread, std::vector<u1>& byteCodes);
void interpret(std::shared_ptr<rtda::Method> method);
void invokeMethod(std::shared_ptr<rtda::Frame> frame, std::shared_ptr<rtda::Method> method);