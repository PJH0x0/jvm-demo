#pragma once
#include "classfile/member_info.h"
#include "classfile/attr_info.h"
#include "instructions/base/base_insts.h"
#include "runtime/frame.h"
#include "runtime/thread.h"
#include "instructions/base/bytecode_reader.h"
#include "instructions/instruction_factory.h"
#include <memory>
using namespace classfile;

void Loop(std::shared_ptr<runtime::Thread> thread, std::vector<u1>& byteCodes);
void Interpret(std::shared_ptr<runtime::Method> method, const std::vector<std::string>& args);
void Interpret(runtime::Thread* thread);
runtime::Object* CreateArgsArray(const std::vector<std::string>& args);
void InvokeMethod(std::shared_ptr<runtime::Frame> frame, std::shared_ptr<runtime::Method> method);