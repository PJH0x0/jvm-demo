#pragma once
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



std::shared_ptr<Instruction> createInstruction(uint8_t opcode);

}