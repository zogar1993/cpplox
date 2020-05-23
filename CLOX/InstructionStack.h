#pragma once

#include "common.h"
#include "object.h"
#include "CompilerTypes.h"

class InstructionStack
{
public:
	InstructionStack();
	InstructionStack(FunctionType type, InstructionStack* current);
	ObjFunction* function = NULL;
	FunctionType type;

	Local locals[UINT8_COUNT];
	int localCount = 0;
	int scopeDepth = 0;
};

